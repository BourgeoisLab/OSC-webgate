/****************************************************************************
 *   Copyright (c) 2014 - 2015 Frédéric Bourgeois <bourgeoislab@gmail.com>  *
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

#include <string.h>
#include "datapool.h"
#include "utils.h"
#include "cgi.h"

/****************************************************************************/

/**
* @brief Replaces hex code in format "%xx" to the corresponding character.
* @param pStr String
* @return The String
*/
static char* str_decode(char* pStr)
{
    size_t len = strlen(pStr);

    if (len > 2)
        if (strchr(pStr, '%'))
        {
        unsigned int i = 0, g = 0, val = 0, scanFailed;
        char inByte[] = { '0', 'x', '0', '0', 0 };

        while (i < len)
        {
            //if ( value[i] == '%' && value[i+1] == '%' )
            //{
            //    value[g++] = value [i++];
            //    value[g] = value [i];
            //}
            //else
            if (pStr[i] == '%' && (i + 2) < len)
            {
                scanFailed = 1;
                //Ex. %2C -> 0x2C -> ','
                inByte[2] = pStr[++i];
                inByte[3] = pStr[++i];
                if (sscanf(inByte, "%i", &val) == 1)
                {
                    if (val != 0)
                    {
                        pStr[g] = (char)(val);
                        scanFailed = 0;
                    }
                }
                if (scanFailed) // failed !
                {
                    pStr[g++] = '%';
                    pStr[g++] = inByte[2];
                    pStr[g] = inByte[3];
                }
            }
            else
            {
                pStr[g] = pStr[i];
            }
            i++;
            g++;
        }
        pStr[g] = '\0'; // End of string
        }

    return pStr;
}

/****************************************************************************/

/**
 */
void CGI_processGetValue(struct mg_connection *conn)
{
    if (conn->query_string)
    {
        // get value
        const char *pValue;
        str_decode((char*)conn->query_string);
        pValue = DP_getValue(conn->query_string);
        mg_send_header(conn, "Content-Type", "text/plain");
        mg_send_data(conn, pValue, strlen(pValue));
    }
    else
    {
        mg_send_status(conn, 400);
        mg_send_data(conn, "", 0);
    }
}

/**
 */
void CGI_processSetValue(struct mg_connection *conn)
{
    if (conn->query_string)
    {
        char *pStr = (char*)conn->query_string;
        char *pVariable, *pValue;
        char *pEqual, *pAmp;

        while (1)
        {
            pVariable = pStr;
            pEqual = strchr(pStr, '=');
            if (!pEqual)
                break;
            *pEqual = '\0';
            pStr = pEqual + 1;
            pValue = pStr;
            pAmp = strchr(pStr, '&');
            if (pAmp)
            {
                *pAmp = '\0';
                pStr = pAmp + 1;
            }
            else
            {
                pStr = NULL;
            }
            str_decode(pVariable);
            str_decode(pValue);
            str_replaceChar(pValue, '+', ' ');
            DP_setValue(pVariable, pValue);
            if (!pStr)
                break;
        }

        mg_send_status(conn, 200);
        mg_send_data(conn, "", 0);
    }
    else
    {
        mg_send_status(conn, 400);
        mg_send_data(conn, "", 0);
    }
}
