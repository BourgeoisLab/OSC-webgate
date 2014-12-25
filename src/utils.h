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

/**
 *  @file utils.h
 *  @brief Various utility functions.
 *  @author Frédéric Bourgeois
 *  @version 1.0
 *  @date 6 Aug 2014
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>

/**
 * @defgroup UTILITIES Utilities
 * @brief Utility functions.
 *
 * Various utility functions. See function prototype for detailed 
 * information about a specific function.
 *
 * Define the preprocessor WIN32 for Windows or LINUX for linux platform.
 * @{
 */

/**
 * @brief Delay a task a certain amount of time.
 * @param ms Time in milliseconds
 */
void SYS_sleep(unsigned long ms);

/**
 * @brief Read one line of a file and put it in a buffer. Support only ANSI file.
 * @param buffer Buffer where line will be stored.
 * @param max Buffer size.
 * @param fp File handler.
 * @param pEOF Is set to 1 if end of file is reached. Can be NULL.
 * @return Number of characters read.
 */
size_t freadln(char *buffer, size_t max, FILE *fp, int* pEOF);

/**
 * @brief Get the configuration from a file.
 * A callback function is called for every line in the file.
 * Additionally a section name can be passed. Only the lines within the section
 * will invoke the callback.
 * @param pFileName Configuration file
 * @param pSection Optional section name. If NULL every line will invoke the callback
 * @param cb Callback function
 * @return Number of valid lines or -1 on error.
 */
int getConfigFromFile(const char *pFileName, const char *pSection, int(*cb)(char*,char*));

/**
 * @brief Remove leading and trailing spaces.
 * @param pStr String
 * @return Cleaned string
 */
char *str_trimLeadingTrailingSpaces(char *pStr);

/**
 * @brief Replaces a character by another one.
 * @param pStr String
 * @param c1 Character to replace
 * @param c2 Replacement character
 */
void str_replaceChar(char* pStr, char c1, char c2);

/** @} UTILITIES */

#endif // _UTILS_H_
