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
 *  @file ujsonpars.h
 *  @brief Utility to parse JSON data.
 *  @author Frédéric Bourgeois
 *  @version 1.0
 *  @date 5 Aug 2014
 */

#ifndef _UJSONPARS_H_
#define _UJSONPARS_H_

/**
 * @addtogroup UTILITIES
 * @{
 */
 
/**
 * @defgroup JSONPARSER JSON Parser
 * @brief Generic JSON parser.
 *
 * Parses generic JSON data and calls different user-defined callback functions.
 *
 * Callback functions:
 *   - \b UJSON_GetChar     Called to get the next character from a stream (file or buffer)
 *   - \b UJSON_startObject Called when a new object was found ("{")
 *   - \b UJSON_endObject   Called when an object was closed ("}")
 *   - \b UJSON_startPair   Called when the beginning of a pair was found
 *   - \b UJSON_value       Called when a value/pair was found
 *   - \b UJSON_startArray  Called when a new array was found ("[")
 *   - \b UJSON_endArray    Called when an object was closed ("]")
 *
 * @see http://json.org/
 * @{
 */
 
/** Callback function type to get the next character */
typedef int  (*UJSON_GetChar)(void* pJson);
/** Callback function type when an object is starting, i.e. '{' was detected */
typedef void (*UJSON_startObject)(void* pJson);
/** Callback function type when an object is ending, i.e. '}' was detected */
typedef void (*UJSON_endObject)(void* pJson);
/** Callback function type when a pair was detected */
typedef void (*UJSON_startPair)(void* pJson, char* pPair);
/** Callback function type when a pair/value was detected */
typedef void (*UJSON_value)(void* pJson, char* pPair, char* pValue);
/** Callback function type when an array is starting, i.e. '[' was detected */
typedef void (*UJSON_startArray)(void* pJson);
/** Callback function type when an array is ending, i.e. ']' was detected */
typedef void (*UJSON_endArray)(void* pJson);

/** @brief JSON parsing structure */
typedef struct t_uJson
{
    void* fp;                       /**< any type of handler */
    int   eof;                      /**< end of file / buffer flag (0/1) */
    int   state;                    /**< any user-defined state machine */
    int   unget_c;                  /**< unget character */
    int   objectDepth;              /**< object depth */
    int   pairSize;                 /**< buffer size of pPair */
    int   valueSize;                /**< buffer size of pValue */
    long  maxReadCnt;               /**< set here the maximal characters to read */
    long  readCnt;                  /**< number of characters read */
    char  *pPair;                   /**< buffer for the pair string */
    char  *pValue;                  /**< buffer for the value string */
    void  *pObject;                 /**< any user-defined object */
    UJSON_GetChar getChar;          /**< callback function to get the next character */
    UJSON_startObject startObject;  /**< callback function when an object is starting */
    UJSON_endObject endObject;      /**< callback function when an object is ending */
    UJSON_startPair startPair;      /**< callback function when a pair was detected */
    UJSON_value value;              /**< callback function when a pair/value was detected */
    UJSON_startArray startArray;    /**< callback function when an array is starting */
    UJSON_endArray endArray;        /**< callback function when an array is ending */
} T_uJson, *PT_uJson;

/**
 * @brief Initialize the JSON parser.
 * @param pJson Parsing structure
 */
void UJSON_init(PT_uJson pJson);

/**
 * @brief Start the JSON parser.
 * @param pJson Parsing structure
 * @return 0 on success
 */
int UJSON_parse(PT_uJson pJson);

/** @} JSONPARSER */

/** @} UTILITIES */

#endif //_UJSONPARS_H_
