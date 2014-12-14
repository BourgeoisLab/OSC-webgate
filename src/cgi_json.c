/****************************************************************************
 *   Copyright (c) 2014 Frédéric Bourgeois <bourgeoislab@gmail.com>         *
 *                                                                          *
 *   This file is part of OSC-webgate.                                      *
 *                                                                          *
 *   OSC-webgate is free software: you can redistribute it and/or           *
 *   modify it under the terms of the GNU General Public License as         *
 *   published bythe Free Software Foundation, either version 3 of the      *
 *   License, or (at your option) any later version.                        *
 *                                                                          *
 *   OSC-webgate is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with OSC-webgate. If not, see <http://www.gnu.org/licenses/>.    *
 ****************************************************************************/

#include <string.h>
#include "datapool.h"
#include "ujsonpars.h"
#include "cgi.h"

/****************************************************************************/

/** Buffer size for parsing variables */
#define JPARSE_BUFFER_SIZE          256

/** Used by the JSON parser to store a pair */
static char gpPair[JPARSE_BUFFER_SIZE];

/** Used by the JSON parser to store a value */
static char gpValue[JPARSE_BUFFER_SIZE];

/** Used by this module to temporary store a variable name */
static char gpVariable[JPARSE_BUFFER_SIZE];

/****************************************************************************/

/**
 * @brief Get the next character in the incoming buffer.
 * @param ptr Pointer to JSON parsing structure
 * @return Character
 */
static int getChar(void* ptr)
{
    PT_uJson pJson = (PT_uJson)ptr;
    struct mg_connection *conn = pJson->fp;
    return conn->content[pJson->readCnt];
}

/**
 * @brief Callback for "start of array".
 * @param ptr Pointer to JSON parsing structure
 * @param pPair Pair
 */
static void startPair(void* ptr, char* pPair)
{
    PT_uJson pJson = (PT_uJson)ptr;
    if (pJson->objectDepth == 1)
    {
        if (strcmp("read", pPair) == 0)
        {
            pJson->state = 10; // read variable
            mg_send_data(pJson->fp, "\"read\":", 7);
        }
        else if (strcmp("write", pPair) == 0)
        {
            pJson->state = 20; // write variable
            mg_send_data(pJson->fp, "\"write\":", 8);
        }
    }
}

/**
 * @brief Callback for "end of array".
 * @param ptr Pointer to JSON parsing structure
 * @param pPair Pair
 * @param pValue Value
 */
static void newValue(void* ptr, char* pPair, char* pValue)
{
    PT_uJson pJson = (PT_uJson)ptr;
    switch (pJson->state)
    {
        case 0:
            if (strcmp("version", pPair) == 0)
            {
                // check version
                if (strcmp("1", pValue) == 0)
                {
                    mg_send_data(pJson->fp, "\"version\":\"1\",", 14);
                }
                else
                {
                    pJson->eof = 1; // exit parser
                }
            }
            break;
        case 11: // read first variable
        case 12: // read other variables -> append "," first
            if (strcmp("var", pPair) == 0)
            {
                const char *val = DP_getValue(pValue);
                if (pJson->state == 12)
                    mg_send_data(pJson->fp, ",", 1);
                pJson->state = 12;
                mg_printf_data(pJson->fp, "{\"var\":\"%s\",\"val\":\"%s\"}", pValue, val);
            }
            break;
        case 21: // write first variable
        case 22: // write other variables -> append "," first
            if (strcmp("var", pPair) == 0)
            {
                strncpy(gpVariable, pValue, JPARSE_BUFFER_SIZE - 1);
            }
            else if (strcmp("val", pPair) == 0)
            {
                const char *val;
                DP_setValue(gpVariable, pValue);
                val = DP_getValue(gpVariable);
                if (pJson->state == 22)
                    mg_send_data(pJson->fp, ",", 1);
                pJson->state = 22;
                mg_printf_data(pJson->fp, "{\"var\":\"%s\",\"val\":\"%s\"}", gpVariable, val);
            }
            break;
    }
}

/**
 * @brief Callback for "start of array".
 * @param ptr Pointer to JSON parsing structure
 */
static void startArray(void* ptr)
{
    PT_uJson pJson = (PT_uJson)ptr;
    if (pJson->objectDepth == 1)
    {
        if (pJson->state > 0)
        {
            pJson->state++; // --> 11 or 21
            mg_send_data(pJson->fp, "[", 1);
        }
    }
}

/**
 * @brief Callback for "end of array".
 * @param ptr Pointer to JSON parsing structure
 */
static void endArray(void* ptr)
{
    PT_uJson pJson = (PT_uJson)ptr;
    if (pJson->objectDepth == 1)
    {
        mg_send_data(pJson->fp, "]", 1);
        pJson->state = 0; // reset state
    }
}

/****************************************************************************/

/**
 */
void CGI_processJSON(struct mg_connection *conn)
{
    T_uJson uJson;

    // initialize JSON parser
    UJSON_init(&uJson);
    uJson.fp = (void*)conn;
    uJson.maxReadCnt = conn->content_len;
    uJson.getChar  = getChar;
    uJson.startPair = startPair;
    uJson.value = newValue;
    uJson.startArray = startArray;
    uJson.endArray = endArray;
    uJson.pPair = gpPair;
    uJson.pairSize = JPARSE_BUFFER_SIZE;
    uJson.pValue = gpValue;
    uJson.valueSize = JPARSE_BUFFER_SIZE;

    // set HTTP header for the response
    mg_send_header(conn, "Content-Type", "application/json");

    // start parsing incoming JSON
    mg_send_data(conn, "{", 1);
    UJSON_parse(&uJson);
    mg_send_data(conn, "}", 1);
}
