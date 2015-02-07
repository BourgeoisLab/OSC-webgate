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
 * @mainpage OSC-webgate Documentation
 *
 * @section WhatIs What is OSC-webgate?
 * Open Sound Control (OSC) is a protocol used to control synthesizers, or
 * other multimedia devices. The idea behind OSC-webgate is to provide the 
 * possibility to control these devices with a standard web-browser.
 *
 * Technically OSC-webgate is a web-server that forwards web requests to
 * an OSC host. Additionally it keeps track of the variables (= control values)
 * sent to the host. In this way it can keep all his clients synchronized.
 *
 * OSC-webgate was designed to run on a Raspberry Pi Linux machine. But it
 * was successfully tested on a Windows machine.
 *
 * @image html OSC-webgate.png
 *
 * @section License License
 * OSC-webgate is free software and licensed under GPL version 3. 
 *
 * @section install Install, Configure and Run (Linux)
 *
 * @subsection sources Get Sources
 * Get the sources from github to your local machine with git clone:
 * <pre>
 * git clone http://github.com/bourgeoislab/osc-webgate
 * </pre>
 *
 * @subsection compile Compile
 * Compile OSC-webgate:
 * <pre>
 * cd OSC-webgate
 * make
 * </pre>
 *
 * @subsection config1 Configure I
 * Edit file OSC-webgate.conf with a text editor.
 * For a first test I recommend changing the settings to:
 * <pre>
 * root = "./www"
 * port = 8080
 * </pre>
 *
 * @subsection test Test OSC-webgate
 * Run OSC-webgate:
 * <pre>
 * ./OSC-webgate
 * </pre>
 * Start a web browser on a machine in the same network and enter
 * <pre>
 * http://ip:8080/
 * </pre>
 * where ip is the IP address of the machine OSC-webgate is running on.
 * You should see a simple page with some controls.
 *
 * @subsection config2 Configure II
 * After a successful test you may edit the default web-pages located
 * in the www sub-directory. Also consider moving the www directory to
 * another location. /var/www/ is widely used as a web-server root. 
 * Also look at the file OSC-webgate.conf and read all configuration
 * possibilities there.
 *
 * @subsection service Running OSC-webgate as a Service
 * The best way to use OSC-webgate is to run it as a service (daemon).
 * Service concepts may differ depending on the Linux distribution.
 * The description here is for Arch Linux using systemd.
 *
 * A setup file setup_service.sh is added in the distribution. Running
 * this script will:
 * - copy OSC-webgate to /usr/local/bin
 * - create directory /etc/OSC-webgate
 * - copy OSC-webgate.conf to /etc/OSC-webgate
 * - copy OSC-webgate.service to /usr/lib/systemd/system/ 
 * - create directory /var/www
 * - copy content of www to /var/www
 *
 * Note that the path of the configuration file is now:
 * <pre>
 * /etc/OSC-webgate/OSC-webgate.conf
 * </pre>
 * and the path of the www root folder
 * <pre>
 * /var/www
 * </pre>
 *
 * At this point you may delete the OSC-webgate directory created
 * with the git clone command.
 *
 * <b>Start OSC-webgate service:</b>
 * <pre>
 * systemctl start OSC-webgate
 * </pre>
 *
 * <b>Stop OSC-webgate service:</b>
 * <pre>
 * systemctl stop OSC-webgate
 * </pre>
 *
 * <b>Add OSC-webgate service to the start-up procedure:</b>
 * <pre>
 * systemctl enable OSC-webgate
 * </pre>
 *
 * <b>Remove OSC-webgate service from start-up procedure:</b>
 * <pre>
 * systemctl disable OSC-webgate
 * </pre>
 *
 * @section communication Communication Protocol
 * The communication between OSC-webgate and the clients (web-browser, smartphone or even own 
 * programs) is based on standard HTTP GET and/or POST requests. A detailed description can
 * be found in the CGI module description.
 *
 * @section extensions Extensions
 * Say you added some controls (potentiometers) and also a display to your Raspberry Pi and
 * want to use them to control some parameters on the OSC host. There are mainly two 
 * possibilities to achieve this.
 *
 * @subsection extensions1 1. Web-Server Communication
 * Create a program that can send HTTP requests and use the CGI API described in the CGI
 * module documentation. Using this API you can read and write variable values.
 * An example can be found in the examples sub-directory (OSC-webgate-client).
 *
 * @subsection extensions2 2. Direct Integration
 * Extend the file datapooluser.c to your needs. The API there allows you to directly read and
 * write variable values.
 *
 * @section Credits Credits
 * - <b> Mongoose web-server:</b> http://cesanta.com/mongoose.shtml
 * - <b> OSC client by Matt Wright:</b> http://archive.cnmat.berkeley.edu/OpenSoundControl/src/
 * - <b> jQuery:</b> http://jquery.com/
 *
 * @section ReleaseNotes Release Notes
 * <b>[v1.1.0]</b>
 * - [fix] System (pre-defined) data-pool is now checked before user data-pool.
 *
 * <b>[v1.0.0]</b>
 * - [new] Initial release.
 *
 * @section Info Information
 * @author Frédéric Bourgeois <bourgeoislab@gmail.com>
 * @see Project homepage: http://bourgeoislab.wordpress.com/osc-webgate
 * @see Latest release: http://github.com/bourgeoislab/osc-webgate/releases
 * @see Latest sources: http://github.com/bourgeoislab/osc-webgate
 */
 
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#ifdef WIN32
  #include <winsock.h>
#endif
#include "release.h"
#include "utils.h"
#include "mongoose.h"
#include "datapool.h"
#include "cgi.h"

/****************************************************************************/

/** Main loop state */
static int running = 1;

/** Application configuration structure */
T_AppConfig app;

/** Web-server structure */
struct mg_server *webserver = NULL;

/****************************************************************************/

/**
 * @brief Web-server event handler (see mongoose documentation).
 * @param conn HTTP request information
 * @param ev Event number
 * @return Depends on event number
 */
static int event_handler(struct mg_connection *conn, enum mg_event ev)
{
    if (ev == MG_REQUEST)
    {
        if (strncmp(conn->uri, "/cgi-bin/", 9) == 0)
        {
            if (strcmp(conn->uri+9, "json.cgi") == 0)
            {
                CGI_processJSON(conn);
            }
            else if (strcmp(conn->uri + 9, "getValue.cgi") == 0)
            {
                CGI_processGetValue(conn);
            }
            else if (strcmp(conn->uri + 9, "setValue.cgi") == 0)
            {
                CGI_processSetValue(conn);
            }
            else
            {
                // send forbidden status code
                mg_send_status(conn, 403);
                mg_send_data(conn, "", 0);
            }
            return MG_TRUE;
        }
    }
    return MG_FALSE;
}

/**
 * @brief Close all stuff.
 */
static void closeAll(void)
{
    // close only once
    static int closed = 0;
    if (closed)
        return;
    closed = 1;

    // de-initialize web-server
    mg_destroy_server(&webserver);

    // de-initialize data-pool
    DP_deinit();

  #ifdef WIN32
    // de-initialize windows socket API
    WSACleanup();
  #endif
  
    printf("Bye.\n");
}

/**
 * @brief Function callback for signals.
 * @param code Signal code
 */
static void on_signal(int code)
{
    // go out of the main loop and call closeAll() in the main task
    running = 0;

    // the application should terminate during this time
    SYS_sleep(1000);

    // the application hang, so close all stuff
    closeAll();

    // exit application
    exit(0);
}

/****************************************************************************/

/**
 * @brief Set default settings.
 */
static void initConfig(void)
{
    memset(&app, 0, sizeof(T_AppConfig));
    strcpy(app.root, MONGOOSE_DEFAULT_ROOT);
    app.port = MONGOOSE_DEFAULT_PORT;
    strcpy(app.user_prefix, DPU_DEFAULT_PREFIX);
    strcpy(app.osc_host, OSC_DEFAULT_HOST);
    app.osc_port = OSC_DEFAULT_PORT;
    strcpy(app.osc_prefix, OSC_DEFAULT_PREFIX);
}

/** 
 * @brief Callback function when loading configuration file.
 * @param pParameter Parameter
 * @param pValue Value
 * @return 1 if the line is valid else 0
 */
static int callback_initFromFile(char *pParameter, char* pValue)
{
    if (strcmp("port", pParameter) == 0)
    {
        app.port = atoi(pValue);
    }
    else if (strcmp("root", pParameter) == 0)
    {
        strncpy(app.root, pValue, CONFIG_BUFFER_SIZE - 1);
    }
    else if (strcmp("on_the_fly_allocation", pParameter) == 0)
    {
        app.onTheFlyAllocation = atoi(pValue);
    }
    else if (strcmp("user_prefix", pParameter) == 0)
    {
        strncpy(app.user_prefix, pValue, CONFIG_BUFFER_SIZE - 1);
    }
    else if (strcmp("osc_host", pParameter) == 0)
    {
        strncpy(app.osc_host, pValue, CONFIG_BUFFER_SIZE - 1);
    }
    else if (strcmp("osc_port", pParameter) == 0)
    {
        app.osc_port = atoi(pValue);
    }
    else if (strcmp("osc_prefix", pParameter) == 0)
    {
        strncpy(app.osc_prefix, pValue, CONFIG_BUFFER_SIZE - 1);
    }
    else
    {
        return 0;
    }
    return 1;
}

/**
 * @brief Load configuration from a file.
 * @param pFileName Configuration file
 * @return -1 on error
 */
static int loadConfig(const char *pFileName)
{
    return getConfigFromFile(pFileName, "["CONFIG_SECTION_APP"]", callback_initFromFile);
}

/****************************************************************************/

/**
 * @brief Main entry point.
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int main(int argc, char* argv[])
{
    const char *error_msg;
    char buffer[32];
  #ifdef WIN32
    WSADATA wsaData;
  #endif
    
    // initialize configuration
    initConfig();

    // load configuration
    if (loadConfig(APP_CONFIG_FILE) < 0)
    {
        printf("Failed to load "APP_CONFIG_FILE".\n");
        return 1;
    }

    printf(APP_NAME" v"APP_VERSION" started on port %d\n", app.port);
    printf("Routing \"%s...\" to OSC host %s:%d\n", app.osc_prefix, app.osc_host, app.osc_port);

    // set signals
    signal(SIGINT, on_signal);      // if user presses CTRL+C
    signal(SIGTERM, on_signal);     // kill command
  #ifdef WIN32
    signal(SIGBREAK, on_signal);    // CTRL-break sequence
  #endif

  #ifdef WIN32
    // initialize windows socket API
    WSAStartup(0x0101, &wsaData);
  #endif

    // initialize data-pool
    DP_init(APP_CONFIG_FILE, app.onTheFlyAllocation);

    // initialize web-server
    webserver = mg_create_server(NULL, event_handler);
    error_msg = mg_set_option(webserver, "document_root", app.root);
    if (!error_msg)
    {
        sprintf(buffer, "%d", app.port);
        error_msg = mg_set_option(webserver, "listening_port", buffer);
        if (!error_msg)
        {
            // endless loop
            while (running)
            {
                DP_refresh();
                mg_poll_server(webserver, 100);
            }
        }
    }

    // check for an error
    if (error_msg)
        printf("Error: %s\n", error_msg);

    // close all stuff
    closeAll();

    return 0;
}
