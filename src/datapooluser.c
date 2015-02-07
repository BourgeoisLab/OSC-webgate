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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datapool.h"

// example variables
static int myIntVar = 0;
static char myStrVar[DP_VALUE_LENGTH_MAX];

/**
 */
void DPUSER_init(void)
{
    // add your initialization code here
    myIntVar = 0;
    strncpy(myStrVar, "String variable", DP_VALUE_LENGTH_MAX);
}

/**
 */
void DPUSER_deinit(void)
{
    // add your de-initialization code here
}

/**
 */
void DPUSER_refresh(void)
{
    // add your refresh code here
    static int counter = 0;
    if (++counter % 10 == 0)
        ++myIntVar;
}

/**
 */
const char* DPUSER_getValue(const char *pVariable)
{
    static char pValue[DP_VALUE_LENGTH_MAX];
	
    *pValue = '\0';
    
    // skip variable prefix (app.user_prefix)
    pVariable += strlen(app.user_prefix);
	   
    // add your code to get the value of a specific variable here
    if (strcmp("myIntVar", pVariable) == 0)
    {
        sprintf(pValue, "%d", myIntVar);
    }
    else if (strcmp("myStrVar", pVariable) == 0)
    {
        strncpy(pValue, myStrVar, DP_VALUE_LENGTH_MAX);
    }

    return pValue;
}

/**
 */
void DPUSER_setValue(const char *pVariable, const char *pValue)
{
    // skip variable prefix (app.user_prefix)
    pVariable += strlen(app.user_prefix);

    // add your code to read the value of a specific variable here
    if (strcmp("myIntVar", pVariable) == 0)
    {
        myIntVar = atoi(pValue);
    }
    else if (strcmp("myStrVar", pVariable) == 0)
    {
        strncpy(myStrVar, pValue, DP_VALUE_LENGTH_MAX);
    }
}
