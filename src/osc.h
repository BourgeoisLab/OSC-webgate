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
 *  @file osc.h
 *  @brief Functions for the OSC module.
 *  @author Frédéric Bourgeois
 *  @version 1.0
 *  @date 5 Aug 2014 
 */

#ifndef _OSC_H_
#define _OSC_H_

#include "release.h"

/**
 * @defgroup OSC OSC
 * @brief Wrapper module for the OSCx library.
 * @see http://puredata.info/downloads/oscx
 * @{
 */
 
/** @brief Structure for an OSC argument */
typedef struct t_OSC_ArgType
{
    enum {
        OSC_INT,            /**< integer */
        OSC_FLOAT,          /**< float */
        OSC_STRING          /**< string */
    } type;                 /**< argument type */
    union {
        int i;              /**< integer value */
        float f;            /**< float value */
        const char *s;      /**< string value (pointer) */
    } datum;                /**< argument value */
} T_OSC_ArgType, *PT_OSC_ArgType;

/**
 * @brief Initialize a new message buffer.
 * @param bundle 1 if more than one messages will be appended
 * @return 0 on success
 */
int OSC_initMessages(int bundle);

/**
 * @brief Append a message to the bundle.
 * @param address OSC address
 * @param numArgs Number of arguments
 * @param args Argument list
 * @return 0 on success
 */
int OSC_appendMessage(const char *address, int numArgs, PT_OSC_ArgType args);

/**
 * @brief Send the message bundle.
 * @param host Host where OSC messages should be sent
 * @param port Port number of the host
 * @return 0 on success
 */
int OSC_sendMessages(const char *host, int port);

/**
 * @brief Convert an argument string to an argument structure.
 * @param pStr String containing the parameter
 * @return A structure with the converted argument.
 */
T_OSC_ArgType OSC_getArgType(const char *pStr);

/** @} OSC */

#endif // _OSC_H_
