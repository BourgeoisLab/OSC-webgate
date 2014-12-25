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

#include "osc.h"

#if OSC_EN

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "OSC-client.h"
#ifdef _WIN32
  #include <winsock.h>
  typedef SOCKET sock_t;
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netdb.h>
  #include <unistd.h>
  #define SOCKET                 int
  #define INVALID_SOCKET (-1)
  #define closesocket(x) close(x)
#endif

/****************************************************************************/

/** OSC buffer containing a whole message bundle */
#define OSC_BUFFER_SIZE             4096

/** output buffer */
static char osc_buffer[OSC_BUFFER_SIZE];

/** OSC buffer */
static OSCbuf osc;

static int isBundle = 0;

/****************************************************************************/

/**
 * @brief Send a UDP packet.
 * @param host Host where packet should be sent
 * @param port Port number of the host
 * @param pBuffer packet to send
 * @param len Length of the packet
 * @return 0 on success
 */
static int sendUDP(const char *host, int port, const char *pBuffer, int len)
{
    int ret = 0;
    SOCKET s;
    struct sockaddr_in servaddr;
    struct hostent *he;

    // open a UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == INVALID_SOCKET)
        return -1;

    // set family and port
    memset((void *)&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(port);
    he = gethostbyname(host);
    servaddr.sin_addr = * (struct in_addr *) he->h_addr_list[0];

    // send UDP packet
    if (sendto(s, pBuffer, len, 0, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) == -1)
        ret = -1;

    // close socket
    closesocket(s);

    return ret;
}

/**
 */
int OSC_initMessages(int bundle)
{
    isBundle = bundle;

    // initialize OSC buffer
    OSC_initBuffer(&osc, OSC_BUFFER_SIZE, osc_buffer);

    // open a bundle
    if (isBundle)
    {
	    if (OSC_openBundle(&osc, OSCTT_Immediately()))
	        return -1;
    }

    return 0;
}

/**
 */
int OSC_appendMessage(const char *address, int numArgs, PT_OSC_ArgType args)
{
    int i;

    // write the address of the message
    if (OSC_writeAddress(&osc, address))
        return -1;

    // write arguments
    for (i = 0; i < numArgs; i++)
    {
        switch (args[i].type)
        {
            case OSC_INT:
                if (OSC_writeIntArg(&osc, args[i].datum.i) != 0)
                    return -1;
                break;

            case OSC_FLOAT:
                if (OSC_writeFloatArg(&osc, args[i].datum.f) != 0)
                    return -1;
                break;

            case OSC_STRING:
                if (OSC_writeStringArg(&osc, args[i].datum.s) != 0)
                    return -1;
                break;
        }
    }

    return 0;
}

/**
 */
int OSC_sendMessages(const char *host, int port)
{
    if (isBundle)
    {
        isBundle = 0;

        // close the bundle
        if (OSC_closeBundle(&osc))
	        return -1;

        // check if a message was added
        if (OSC_packetSize(&osc) == 16) // 16 bytes for "#bundle" and time tag
            return 0;
    }

    // check if the buffer is empty
    if (OSC_isBufferEmpty(&osc))
        return -1;

    // finish buffer
    if (!OSC_isBufferDone(&osc))
        return -1;

    // send messages
    return sendUDP(host, port, OSC_getPacket(&osc), OSC_packetSize(&osc));
}

/**
 */
T_OSC_ArgType OSC_getArgType(const char *pStr)
{
    const char *p = pStr;
    T_OSC_ArgType arg;

    if (*p == '-')
        p++;

    if (isdigit(*p) || *p == '.')
    {
        while (isdigit(*p))
            p++;
        if (*p == '\0')
        {
            arg.type = OSC_INT;
            arg.datum.i = atoi(pStr);
            return arg;
        }
        if (*p == '.')
        {
            p++;
            while (isdigit(*p))
                p++;
            if (*p == '\0')
            {
                arg.type = OSC_FLOAT;
                arg.datum.f = (float)atof(pStr);
                return arg;
            }
        }
    }

    arg.type = OSC_STRING;
    arg.datum.s = pStr;
    return arg;
}

#endif // OSC_EN
