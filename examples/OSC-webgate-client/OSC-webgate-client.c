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
 *  @file OSC-webgate-client.c
 *  @brief Example of a client for OSC-webgate.
 *
 *  This application uses HTTPClient by Eitan Michaelson but you can use
 *  any HTTP client you want.
 * 
 *  @author Frédéric Bourgeois
 *  @version 1.0
 *  @date 21 Dez 2014
 */

#include <stdio.h>
#include "OSC-webgate-client-api.h"

// IP and port of OSC-webgate
static const char *host = "localhost:80";

/**
 */
int main(int argc, char *argv[])
{
    char value[256] = "";

    // initializes the API
    OSCC_init();

    // Print some information about the server
    printf("APP_NAME: %s\n", OSCC_getValue(host, "APP_NAME", value, 256) ? value : "---");
    printf("APP_VERSION: %s\n", OSCC_getValue(host, "APP_VERSION", value, 256) ? value : "---");
    printf("SERVER_IP: %s\n", OSCC_getValue(host, "SERVER_IP", value, 256) ? value : "---");
    printf("SERVER_PORT: %s\n", OSCC_getValue(host, "SERVER_PORT", value, 256) ? value : "---");
    printf("\n");

    // Read and write DPU.myIntVar
    if (OSCC_getValue(host, "DPU.myIntVar", value, 256))
    {
        printf("Get myIntVar = %s\n", value);
    }
    if (OSCC_setValue(host, "DPU.myIntVar", "99"))
    {
        printf("Set myIntVar = 99\n");
    }
    if (OSCC_getValue(host, "DPU.myIntVar", value, 256))
    {
        printf("Get myIntVar = %s\n", value);
    }
    printf("\n");

    // Read and write DPU.myStrVar
    if (OSCC_getValue(host, "DPU.myStrVar", value, 256))
    {
        printf("Get myStrVar = %s\n", value);
    }

    if (OSCC_setValue(host, "DPU.myStrVar", "This is DPU.myStrVar..."))
    {
        printf("Set myStrVar = This is DPU.myStrVar...\n");
    }

    if (OSCC_getValue(host, "DPU.myStrVar", value, 256))
    {
        printf("Get myStrVar = %s\n", value);
    }
    printf("\n");

    // De-initialize the API
    OSCC_deinit();

    return 0;
}
