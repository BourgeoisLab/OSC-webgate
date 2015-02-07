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
 *  @file release.h
 *  @brief Main configuration for all modules.
 *  @author Frédéric Bourgeois
 *  @version 1.0
 *  @date 5 Aug 2014
 */

#ifndef _RELEASE_H_
#define _RELEASE_H_

/****************************************************************************/

/** Make sure a platform is defined */
#if !defined(WIN32) && !defined(LINUX)
  #error Define WIN32 for Windows or LINUX for linux platform
#endif

/****************************************************************************/

/**
 * @defgroup CFG Configuration
 * @brief OSC-webgate configuration
 * @{
 */

/** Application name */
#define APP_NAME                            "OSC-webgate"

/** Application version */
#define APP_VERSION                         "1.1.0"

/** Application file name*/
#define APP_FILENAME                        "OSC-webgate"

/** Configuration file */
#define APP_CONFIG_FILE                     "OSC-webgate.conf"

/** Section in configuration file for the application */
#define CONFIG_SECTION_APP                  "config"

/** Section in configuration file for the data-pool */
#define CONFIG_SECTION_DATAPOOL             "data-pool"

/** Buffer size of some configuration members */
#define CONFIG_BUFFER_SIZE                  128

/** Configuration structure for the Application */
typedef struct t_AppConfig
{
    int  port;                              /**< Port of the web-server*/
    char root[CONFIG_BUFFER_SIZE];          /**< Root folder of the web-server */
    int  onTheFlyAllocation;                /**< Allocate variables in data-pool on the fly */
    char user_prefix[CONFIG_BUFFER_SIZE];   /**< Prefix of variables routed to the user data-pool */
    int  osc_port;                          /**< Port of the OSC host */
    char osc_host[CONFIG_BUFFER_SIZE];      /**< OSC host name or IP */
    char osc_prefix[CONFIG_BUFFER_SIZE];    /**< Prefix of variables routed to the OSC host */
} T_AppConfig, *PT_AppConfig;

extern T_AppConfig app;

/****************************************************************************/

/**
 * @defgroup CFG_MONGOOSE Mongoose
 * @brief Web-server module
 * @{
 */

/** Disable MD5 authorization support */
#define MONGOOSE_NO_AUTH

/** Disable CGI support */
#define MONGOOSE_NO_CGI

/** Disable SSI support */
#define MONGOOSE_NO_SSI

/** Disable WebDAV support */
#define MONGOOSE_NO_DAV

/** Disable directory listing */
#define MONGOOSE_NO_DIRECTORY_LISTING

/** Disable access/error logging */
#define MONGOOSE_NO_LOGGING

/** Disable WebSocket support */
#define MONGOOSE_NO_WEBSOCKET

/** Default root folder of the web-server */
#define MONGOOSE_DEFAULT_ROOT               "/var/www"

/** Default port number of the web-server */
#define MONGOOSE_DEFAULT_PORT               80

/** Use the global malloc function */
#define NS_MALLOC                           SYS_malloc

/** Use the global realloc function */
#define NS_REALLOC                          SYS_realloc

/** Use the global free function */
#define NS_FREE                             SYS_free

/** @} CFG_MONGOOSE */

/****************************************************************************/

/**
 * @defgroup CFG_DATAPOOL Data-pool
 * @brief Data-pool module
 * @{
 */

/** Maximal length of a value in the data-pool */
#define DP_VALUE_LENGTH_MAX                 256

/** Default variable prefix for user data-pool variables */
#define DPU_DEFAULT_PREFIX                  "DPU."

/** @} CFG_DATAPOOL */

/****************************************************************************/

/**
 * @defgroup CFG_OSC OSC
 * @brief OSC module
 * @{
 */

/** Enable/disable code for the Open Sound Control (OSC) */
#define OSC_EN                              1

/** Default OSC host where OSC commands are send to */
#define OSC_DEFAULT_HOST                    "localhost"

/** Default OSC port number of the listen socket of the OSC host */
#define OSC_DEFAULT_PORT                    3001

/** All variables with this default prefix are propagated to the OSC host */
#define OSC_DEFAULT_PREFIX                  ""

/** @} CFG_OSC */

/****************************************************************************/

/**
 * @defgroup CFG_MEMORY Memory Management
 * @brief Memory management
 * @{
 */
 
/** Global function/macro for memory allocation */
#define SYS_malloc          malloc

/** Global function/macro for memory block allocation */
#define SYS_calloc          calloc

/** Global function/macro for memory reallocation */
#define SYS_realloc         realloc

/** Global function/macro for memory de-allocation */
#define SYS_free            free

/** @} CFG_MEMORY */

/****************************************************************************/

/** @} CFG */

#endif // _RELEASE_H_
