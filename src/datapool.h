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
 *  @file datapool.h
 *  @brief Functions for the data-pool module.
 *  @author Frédéric Bourgeois
 *  @version 1.1
 *  @date 5 Aug 2014
 */

#ifndef _DATAPOOL_H_
#define _DATAPOOL_H_

#include "release.h"

/**
 * @defgroup DATAPOOL Data-pool
 * @brief Data-pool module.
 *
 * The data-pool is a simple chained list of variable-value pairs.
 * The values are stored as string and are size limited by DP_VALUE_LENGTH_MAX.
 *
 * On initialization a file can be specified to initialize variables.
 * The file can contain variable-value pairs like this (one per line):
 *    variable=value
 *
 * Furthermore it can be specified if variables should be added on the fly.
 * In this mode if a variable is not found in the list while calling
 * DP_getValue() or DP_setValue() it will be added to the list.
 *
 * Additionally if OSC_EN is enabled, every time a variable starting with
 * the prefix OSC_PREFIX is written to, this new value is propagated via OSC.
 * @{
 */
 
/** Maximal length of a value */
#ifndef DP_VALUE_LENGTH_MAX
  #define DP_VALUE_LENGTH_MAX           256
#endif

/**
 * @brief Initialize the data-pool module.
 * Call this function before any DP_getValue() or DP_setValue().
 * @param pFileName Initialize variables from this file, can be NULL
 * @param onTheFlyAllocation Specify if new variables should be added if not found
 * @return Number of variables added to the data-pool
 */
int DP_init(const char *pFileName, int onTheFlyAllocation);

/**
 * @brief De-initialize the data-pool module.
 * This will free all allocated memory.
 */
void DP_deinit(void);

/**
 * @brief Called periodically with an undefined period (about 100ms).
 */
void DP_refresh(void);

/**
 * @brief Get the value of the corresponding variable.
 * @param pVariable Variable name
 * @return Value of the variable or empty string if not found
 */
const char* DP_getValue(const char *pVariable);

/**
 * @brief Set a new value of the corresponding variable.
 * @param pVariable Variable name
 * @param pValue New value
 */
void DP_setValue(const char *pVariable, const char *pValue);


/**
 * @defgroup DATAPOOL_SYSTEM System Data-pool
 * @brief Data-pool for system variables.
 * This data-pool defines some system variables.
 *
 * <b>System Variables:</b>
 * - APP_NAME: application name (read-only)
 * - APP_VERSION: application version (read-only)
 * - SERVER_IP: IP address of the server (read-only)
 * - SERVER_PORT: port number of the server (read-only)
 * - USER_PREFIX: prefix of user variables (read-only)
 * - OSC_HOST: host name of OSC host (read-only)
 * - OSC_PORT: port of OSC host (read-only)
 * - OSC_PREFIX: prefix of variables routed to the OSC host (read-only)
 * @{
 */
 
/**
 * @brief Initialize the system data-pool module.
 * @note Called by DP_init().
 */
void DPSYSTEM_init(void);

/**
 * @brief De-initialize the system data-pool module.
 * @note Called by DP_deinit().
 */
void DPSYSTEM_deinit(void);

/**
 * @brief Called periodically with an undefined period (about 100ms).
 * @note Called by DP_refresh().
 */
void DPSYSTEM_refresh(void);

/**
 * @brief Get the value of the corresponding variable.
 * @note Call the generic function DP_getValue() instead.
 * @param pVariable Variable name
 * @return Value of the variable or NULL if not found
 */
const char* DPSYSTEM_getValue(const char *pVariable);

/**
 * @brief Set a new value of the corresponding variable.
 * @note Call the generic function DP_setValue() instead.
 * @param pVariable Variable name
 * @param pValue New value
 * @return 1 if variable is a system variable
 */
int DPSYSTEM_setValue(const char *pVariable, const char *pValue);

/** @} DATAPOOL_SYSTEM */


/**
 * @defgroup DATAPOOL_USER User Data-pool
 * @brief Data-pool for user variables.
 * User defined data-pool variables.
 * @{
 */
 
/**
 * @brief Initialize the user data-pool module.
 * @note Called by DP_init().
 */
void DPUSER_init(void);

/**
 * @brief De-initialize the user data-pool module.
 * @note Called by DP_deinit().
 */
void DPUSER_deinit(void);

/**
 * @brief Called periodically with an undefined period (about 100ms).
 * This function can be used to do some non-critical actions.
 * @note Called by DP_refresh().
 */
void DPUSER_refresh(void);

/**
 * @brief Get the value of the corresponding variable.
 * @note Call the generic function DP_getValue() instead.
 * @param pVariable Variable name
 * @return Value of the variable or empty string if not found
 */
const char* DPUSER_getValue(const char *pVariable);

/**
 * @brief Set a new value of the corresponding variable.
 * @note Call the generic function DP_setValue() instead.
 * @param pVariable Variable name
 * @param pValue New value
 */
void DPUSER_setValue(const char *pVariable, const char *pValue);

/** @} DATAPOOL_USER */


/** @} DATAPOOL */

#endif // _DATAPOOL_H_
