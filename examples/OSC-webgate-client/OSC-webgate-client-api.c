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
 
#include "HTTPClient/HTTPClient.h"
#include "OSC-webgate-client-api.h"

/** Input and output buffer size */
#define HTTP_CLIENT_BUFFER_SIZE         2048

/** Output buffer */
static char pBufferOut[HTTP_CLIENT_BUFFER_SIZE];

/** Input buffer */
static char pBufferIn[HTTP_CLIENT_BUFFER_SIZE];

/**
 * @brief Sends a GET request to the server and stores the response in pReceived.
 * @param uri URI with GET request
 * @param pReceived Buffer where response is stored
 * @param receiveMax Buffer size of pReceived
 * @return 1 on success
 */
static int requestGET(char *uri, char *pReceived, int receiveMax)
{
    INT32                   nRetCode;
    UINT32                  nSize, nRead = 0;
    HTTP_SESSION_HANDLE     pHTTP;

    // Open the HTTP request handle
    pHTTP = HTTPClientOpenRequest(0);
    if (!pHTTP)
        return 0;

    // Set the Verb
    if (HTTPClientSetVerb(pHTTP, VerbGet) != HTTP_CLIENT_SUCCESS)
    {
        goto Exception;
    }

    // Send a request for the home page
    if (HTTPClientSendRequest(pHTTP, uri, NULL, 0, FALSE, 0, 0) != HTTP_CLIENT_SUCCESS)
    {
        goto Exception;
    }

    // Retrieve the headers and analyse them
    if ((nRetCode = HTTPClientRecvResponse(pHTTP, 3)) != HTTP_CLIENT_SUCCESS)
    {
        goto Exception;
    }

    // Get the data until we get an error or end of stream code
    while (nRetCode == HTTP_CLIENT_SUCCESS || nRetCode != HTTP_CLIENT_EOS)
    {
        // Set the size of our buffer
        nSize = receiveMax - nRead - 1;

        // Get the data
        nRetCode = HTTPClientReadData(pHTTP, pReceived + nRead, nSize, 0, &nSize);

        nRead += nSize;
    }

    HTTPClientCloseRequest(&pHTTP);
    return 1;

Exception:
    HTTPClientCloseRequest(&pHTTP);
    return 0;
}

/**
 * @brief Replaces character c1 by character c2 in a string
 * @param pStr String
 * @param c1 Character to replace
 * @param c2 Replacement character
 */
static void str_replaceChar(char* pStr, char c1, char c2)
{
    while (*pStr)
    {
        if (*pStr == c1)
            *pStr = c2;
        ++pStr;
    }
}

/**
 * @brief Inserts the specified character at the beginning of a string
 * @param pStr String
 * @param c Character to insert
 */
static void str_insertChar(char* pStr, char c)
{
    char * src, *dst;
    int size = strlen(pStr);
    src = pStr + size - 1;
    dst = pStr + size;
    *(dst + 1) = '\0';
    while (size--)
        *dst-- = *src--;
    *dst = c;
}

/**
 * @brief Replaces the specified character by %xx where xx is the hex code of the character.
 * @param pStr String
 * @param c Character to encode
 */
static void str_encodeChar(char* pStr, int c)
{
    while ((pStr = strchr(pStr, c)) != NULL)
    {
        char sHexCode[4];
        sprintf(sHexCode, "%2.2x", *pStr);
        *pStr++ = '%';
        str_insertChar(pStr, sHexCode[1]);
        str_insertChar(pStr, sHexCode[0]);
        pStr += 2;
    }
}

/**
 * @brief Replaces non printable character by %xx where xx is the hex code of the character.
 * @param pStr String
 */
static void str_encodeNonPrintableChar(char* pStr)
{
    while (*pStr)
    {
        if (*pStr < 32 || *pStr > 0x7F)
        {
            char sHexCode[4];
            sprintf(sHexCode, "%2.2x", ((int)*pStr));
            *pStr++ = '%';
            str_insertChar(pStr, sHexCode[1]);
            str_insertChar(pStr, sHexCode[0]);
            pStr += 2;
        }
        else
        {
            pStr++;
        }
    }
}

/**
 * @brief Encodes a string for a HTTP GET request
 * @param pStr String
 */
static void str_encode(char* pStr)
{
    str_encodeChar(pStr, '%'); // '%' must be the first one
    str_encodeChar(pStr, '/');
    str_encodeChar(pStr, '\\');
    str_encodeChar(pStr, '=');
    str_encodeChar(pStr, '?');
    str_encodeChar(pStr, '&');
    str_encodeChar(pStr, ';');
    str_encodeChar(pStr, '+');
    str_encodeChar(pStr, ' ');
    str_encodeNonPrintableChar(pStr);
    str_replaceChar(pStr, ' ', '+');
}

/**
 */
void OSCC_init(void)
{
  #ifdef _WIN32
    // Windows Specific - Sockets initialization 
    unsigned short      wVersionRequested;
    WSADATA             wsaData;
    UINT32              nErr = 0;

    // We want to use Winsock v 1.2 (can be higher)
    wVersionRequested = MAKEWORD(1, 2);

    nErr = WSAStartup(wVersionRequested, &wsaData);
  #endif
}

/**
 */
void OSCC_deinit(void)
{
  #ifdef _WIN32
    WSACleanup();
  #endif
}

/**
 */
int OSCC_getValue(const char *pHostAddress, const char *pVariable, char *pValue, unsigned int valueMaxSize)
{
    char* s = pBufferOut;
    strcpy(pBufferOut, "http://");
    strcat(pBufferOut, pHostAddress);
    strcat(pBufferOut, "/cgi-bin/getValue.cgi?");
    while (*s) s++;
    strcat(pBufferOut, pVariable);
    str_encode(s);

    if (requestGET(pBufferOut, pBufferIn, HTTP_CLIENT_BUFFER_SIZE))
    {
        strncpy(pValue, pBufferIn, valueMaxSize);
        return 1;
    }
    else
    {
        *pValue = '\0';
        return 0;
    }
}

/**
 */
int OSCC_setValue(const char *pHostAddress, const char *pVariable, const char *pValue)
{
    char* s = pBufferOut;
    strcpy(pBufferOut, "http://");
    strcat(pBufferOut, pHostAddress);
    strcat(pBufferOut, "/cgi-bin/setValue.cgi?");
    while (*s) s++;
    strcat(pBufferOut, pVariable);
    str_encode(s);
    strcat(pBufferOut, "=");
    while (*s) s++;
    strcat(pBufferOut, pValue);
    str_encode(s);

    return requestGET(pBufferOut, pBufferIn, HTTP_CLIENT_BUFFER_SIZE);
}
