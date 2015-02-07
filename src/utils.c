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

#include "utils.h"
#include <string.h>
#include <ctype.h>
#if defined(WIN32)
  #include <windows.h>
#elif defined(LINUX)
  #include <sys/time.h>
  #include <unistd.h>
#endif

/****************************************************************************/

/**
 */
void SYS_sleep(unsigned long ms)
{
  #if defined(WIN32)
    Sleep(ms);
  #elif defined(LINUX)
   	usleep(ms*1000);
  #endif
}

/**
 */
size_t freadln(char *buffer, size_t max, FILE *fp, int* pEOF)
{
    int c;
    size_t nread = 0;

    if (fp == NULL || buffer == NULL)
        return 0;

    if (max > 0)
        max--;

    while (1)
    {
        c = fgetc(fp);
        if (c == -1)
        {
            if (pEOF != NULL)
                *pEOF = 1;   // EOF found
            break;
        }
        if (c == '\n')
            break;      // EOL found

        if (nread < max)
            buffer[nread++] = (char)(c & 0xFF); // low byte
    }

    if (nread && buffer[nread-1] == '\r')
        nread--;

    buffer[nread] = '\0'; // EOS

    return nread;
}

/**
 */
int getConfigFromFile(const char *pFileName, const char *pSection, int(*cb)(char*,char*))
{
    FILE* fp;
    int numItems = 0;

    if (!pFileName || !cb)
        return -1;

    fp = fopen(pFileName, "r");
    if (fp)
    {
        static char buffer[256];
        char* line = buffer;
        char *pValue;
        int inSection = 0;
        int len, eof = 0;
        if (!pSection)
            inSection = 1;
        do {
            len  = freadln(line, 256, fp, &eof);
            if (len > 0 && *line != ';')
            {
                line = str_trimLeadingTrailingSpaces(line);
                if (pSection)
                {
                    if (*line == '[')
                    {
                        if (strcmp(line, pSection) == 0)
                            inSection = 1;
                        else
                            inSection = 0;
                        continue;
                    }
                }
                
                if (inSection)
                {
                    pValue = strchr(line, '=');
                    if (pValue)
                    {
                        *pValue++ = 0;
                        line = str_trimLeadingTrailingSpaces(line);
                        pValue = str_trimLeadingTrailingSpaces(pValue);
                        if (*pValue == '"')
                            pValue++;
                        len = strlen(pValue);
                        if (*(pValue+len-1) == '"')
                            *(pValue+len-1) = '\0';
                        if (cb(line, pValue))
                            numItems++;
                    }
                }
            }
        } while (!eof);

        fclose(fp);
        return numItems;
    }
    return -1;
}

/**
 */
char *str_trimLeadingTrailingSpaces(char *pStr)
{
    char *pEnd;

    // trim leading space
    while(isspace(*pStr))
        pStr++;

    // all spaces?
    if(*pStr == 0)
        return pStr;

    // trim trailing space
    pEnd = pStr + strlen(pStr) - 1;
    while(pEnd > pStr && isspace(*pEnd))
        pEnd--;

    // write new null terminator
    *(pEnd+1) = 0;

    return pStr;
}

/**
 */
void str_replaceChar(char* pStr, char c1, char c2)
{
    while (*pStr)
    {
        if (*pStr == c1)
            *pStr = c2;
        pStr++;
    }
}
