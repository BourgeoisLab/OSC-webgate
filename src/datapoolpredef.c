/****************************************************************************
 *   Copyright (c) 2014 Frédéric Bourgeois <bourgeoislab@gmail.com>         *
 *                                                                          *
 *   This file is part of OSC-webgate.                                      *
 *                                                                          *
 *   OSC-webgate is free software: you can redistribute it and/or           *
 *   modify it under the terms of the GNU General Public License as         *
 *   published bythe Free Software Foundation, either version 3 of the      *
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datapool.h"

#ifdef LINUX
  #include <unistd.h>
  #include <sys/ioctl.h>
  #include <net/if.h>
  #include <arpa/inet.h>
#else
  #include <WinSock2.h>
#endif

/****************************************************************************/

static const char* getServerIpAddress(void);
static const char* getServerPort(void);

/****************************************************************************/

typedef const char*  (*getValueFnc)(void);
typedef void (*setValueFnc)(const char*);

/** Structure for a predefined data */
typedef struct t_PredefData
{
    const char*    pVariable;           /**< Name of predefined data variable */
    const char*    pConstValue;         /**< Value if constant, else NULL */
    getValueFnc    getValue;            /**< Function pointer to get value */
    setValueFnc    setValue;            /**< Function pointer to set value */
}T_PredefData, *PT_PredefData;

static T_PredefData predefData[] =
{
    { "APP_NAME", APP_NAME, NULL, NULL },
    { "APP_VERSION", APP_VERSION, NULL, NULL },
    { "SERVER_IP", NULL, getServerIpAddress, NULL },
    { "SERVER_PORT", NULL, getServerPort, NULL },
    { NULL, NULL, NULL, NULL }
};

/****************************************************************************/

/**
 */
void DPPREDEF_init(void)
{
    // nothing to do
}

/**
 */
void DPPREDEF_deinit(void)
{
    // nothing to do
}

/**
 */
void DPPREDEF_refresh(void)
{
    // nothing to do
}

/**
 */
const char* DPPREDEF_getValue(const char *pVariable)
{
    PT_PredefData ptr = predefData;
    while (ptr->pVariable)
    {
        if (strcmp(ptr->pVariable, pVariable) == 0)
        {
            if (ptr->pConstValue)
                return ptr->pConstValue;
            else
                return ptr->getValue();
        }
        ++ptr;
    }
    return NULL;
}

/**
 */
int DPPREDEF_setValue(const char *pVariable, const char *pValue)
{
    PT_PredefData ptr = predefData;
    while (ptr->pVariable)
    {
        if (strcmp(ptr->pVariable, pVariable) == 0)
        {
            if (ptr->setValue)
                ptr->setValue(pValue);
            return 1;
        }
        ++ptr;
    }
    return 0;
}

/****************************************************************************/

/**
 */
static const char* getServerIpAddress(void)
{
  #ifdef LINUX
    const char *ret = "";
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd != -1)
    {
        struct ifreq ifr;
        ifr.ifr_addr.sa_family = AF_INET;
        strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);
        if (ioctl(fd, SIOCGIFADDR, &ifr) != -1)
            ret = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
        close(fd);
    }
    return ret;
  #else
    char name[256];
    PHOSTENT hostinfo;
    if (gethostname(name, sizeof(name)) == 0)
    {
        if ((hostinfo = gethostbyname(name)) != NULL)
        {
            return inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
        }
    }
  #endif
    return "";
}

/**
 */
static const char* getServerPort(void)
{
    static char port[8];
    sprintf(port, "%d", app.port);
    return port;
}
