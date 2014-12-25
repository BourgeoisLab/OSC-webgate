/****************************************************************************
 *   Copyright (c) 2014 Frédéric Bourgeois <bourgeoislab@gmail.com>         *
 *                                                                          *
 *   This file is part of OSC-webgate.                                      *
 *                                                                          *
 *   OSC-webgate is free software: you can redistribute it and/or           *
 *   modify it under the terms of the GNU General Public License as         *
 *   published by the Free Software Foundation, either version 3 of the     *
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

#include <ctype.h>
#include <string.h>
#include "ujsonpars.h"

/****************************************************************************/

/** Parsing state of a JSON file */
typedef enum t_UJSON_State
{
    UJSON_ST_NONE,                      /**< parsing nothing */
    UJSON_ST_VALUE,                     /**< parsing a value */
    UJSON_ST_ARRAY                      /**< parsing an array */
} T_UJSON_State, *PT_UJSON_State;

/****************************************************************************/

/**
 * @brief Get the next character by calling the callback function getChar().
 * If a character was "unget" the function will return this character.
 * @param pJson JSON parsing structure
 * @return Next character
 */
static int getChar(PT_uJson pJson)
{
    int c;

    // check if end of file/buffer
    if (pJson->eof)
        return -1;

    // check for an "unget" character
    if (pJson->unget_c != -1)
    {
        c = pJson->unget_c;
        pJson->unget_c = -1;
        return c;
    }

    // check if the maximal read count is reached
    if (pJson->readCnt == pJson->maxReadCnt)
    {
        pJson->eof = 1;
        return -1;
    }

    // get next character from callback function
    c = pJson->getChar(pJson);
    pJson->readCnt++;

    // check if end of file/buffer
    if (c == -1)
        pJson->eof = 1;

    return c;
}

/**
 * @brief "Unget" a character. See getChar().
 * @param pJson JSON parsing structure
 * @param c Character to "unget"
 */
static void ungetChar(PT_uJson pJson, int c)
{
    if (pJson->eof)
        return;
    pJson->unget_c = c;
}

/**
 * @brief Read until quote '"' found.
 * @param pJson JSON parsing structure
 * @param pStr String where read characters are stored
 * @param size Maximal size of the string
 * @return Last character, in case it is -1
 */
static int readString(PT_uJson pJson, char *pStr, int size)
{
    int c, c_last = -1, n = 0;
    while (1)
    {
        c = getChar(pJson);
        if (c < 0)
            break;

        if (c == '"' && c_last != '\\')
            break;

        if (c == '/' && c_last == '\\')
        {
            *(pStr - 1) = c;
        }
        else
        {
            n++;
            if (n < size - 1)
                *pStr++ = c;
        }

        c_last = c;
    }
    *pStr = '\0';
    return c;
}

/**
 * @brief Read until '{','}','[' or ']' is found.
 * @param pJson JSON parsing structure
 * @param pVal Value string where read characters are stored
 * @param size Maximal size of the value string
 * @param c_first Last read character
 * @return Last character, in case it is -1
 */
static int readValue(PT_uJson pJson, char *pVal, int size, int c_first)
{
    int c;
    int n = 0;

    n++;
    *pVal++ = c_first;

    while (1)
    {
        c = getChar(pJson);
        if (c < 0)
            break;

        if (c == '{' || c == '}' || c == '[' || c == ']' || c == ',')
        {
            ungetChar(pJson, c);
            break;
        }

        n++;
        if (n < size - 1)
            *pVal++ = c;
    }
    *pVal = '\0';
    return c;
}

/****************************************************************************/

/**
 */
void UJSON_init(PT_uJson pJson)
{
    if (pJson)
    {
        memset(pJson, 0, sizeof(T_uJson));
        pJson->unget_c = -1;
        pJson->maxReadCnt = -1;
    }
}

/**
 */
int UJSON_parse(PT_uJson pJson)
{
    int c;
    T_UJSON_State state = UJSON_ST_NONE;

    if (!pJson || !pJson->getChar || !pJson->pPair || !pJson->pValue)
        return -1;

    while (1)
    {
        // read next character
        c = getChar(pJson);
        if (c < 0)
            break;

        // skip spaces
        if (isspace(c))
            continue;

        switch (c)
        {
            // start of object
            case '{':
                state = UJSON_ST_NONE;
                pJson->objectDepth++;
                if (pJson->startObject)
                    pJson->startObject(pJson);
                break;

            // end of object
            case '}':
                pJson->objectDepth--;
                if (pJson->endObject)
                    pJson->endObject(pJson);
                break;

            // start of array
            case '[':
                state = UJSON_ST_ARRAY;
                if (pJson->startArray)
                    pJson->startArray(pJson);
                break;

            // end of array
            case ']':
                if (pJson->endArray)
                    pJson->endArray(pJson);
                state = UJSON_ST_NONE;
                break;

            // start of a pair or value
            case '"':
                if (state == UJSON_ST_NONE)
                {
                    // start of pair
                    c = readString(pJson, pJson->pPair, pJson->pairSize);
                    if (pJson->startPair)
                        pJson->startPair(pJson, pJson->pPair);
                }
                else if (state == UJSON_ST_VALUE || state == UJSON_ST_ARRAY)
                {
                    // start of value
                    c = readString(pJson, pJson->pValue, pJson->valueSize);
                    if (pJson->value)
                        pJson->value(pJson, pJson->pPair, pJson->pValue);
                    // if we are in an array there may be more values coming
                    if (state != UJSON_ST_ARRAY)
                        state = UJSON_ST_NONE;
                }
                break;

            // value coming
            case ':':
                state = UJSON_ST_VALUE;
                break;

            // next value inside an array coming
            case ',':
                if (state != UJSON_ST_ARRAY)
                    state = UJSON_ST_NONE;
                break;

            default:
                if (state == UJSON_ST_VALUE || state == UJSON_ST_ARRAY)
                {
                    c = readValue(pJson, pJson->pValue, pJson->valueSize, c);
                    if (pJson->value)
                        pJson->value(pJson, pJson->pPair, pJson->pValue);
                }
                break;
        }

        if (c < 0)
            break;
    }

    return 0;
}
