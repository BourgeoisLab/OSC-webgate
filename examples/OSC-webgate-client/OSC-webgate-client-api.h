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

/**
 *  @file OSC-webgate-client-api.h
 *  @brief API for client applications to communicate with OSC-webgate.
 *
 *  This application uses HTTPClient by Eitan Michaelson but you can use
 *  any HTTP client you want.
 * 
 *  @author Frédéric Bourgeois
 *  @version 1.0
 *  @date 21 Dez 2014
 */

#ifndef _OSC_WEBGATE_CLIENT_API_H_
#define _OSC_WEBGATE_CLIENT_API_H_

/**
 * @brief Initializes the API.
 */
void OSCC_init(void);

/**
 * @brief De-initializes the API.
 */
void OSCC_deinit(void);

/**
 * @brief Gets the value of the corresponding variable from OSC-webgate.
 * @param pHostAddress OSC-webgate address including port, e.g. localhost:8080
 * @param pVariable Variable name
 * @param pValue Value of the variable or empty string if not found
 * @param valueMaxSize Maximal size of pValue (buffers size)
 * @return 1 on success
 */
int OSCC_getValue(const char *pHostAddress, const char *pVariable, char *pValue, unsigned int valueMaxSize);

/**
 * @brief Sets a new value of the corresponding variable from OSC-webgate.
 * @param pHostAddress OSC-webgate address including port, e.g. localhost:8080
 * @param pVariable Variable name
 * @param pValue New value
 * @return 1 on success
 */
int OSCC_setValue(const char *pHostAddress, const char *pVariable, const char *pValue);

#endif // _OSC_WEBGATE_CLIENT_API_H_
