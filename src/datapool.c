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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datapool.h"
#include "utils.h"

#if OSC_EN
  #include "osc.h"
#endif

/****************************************************************************/

/** @brief Structure for a entry in the data-pool */
typedef struct t_DataPoolEntry
{
    char *pVariable;                /**< pointer to the variable */
    char *pValue;                   /**< pointer to the value */
    struct t_DataPoolEntry *pNext;  /**< pointer to the next list entry */
} T_DataPoolEntry, *PT_DataPoolEntry;

/****************************************************************************/

/** Initialization state of the data-pool */
static int initialized = 0;

/** Data-pool variable */
static PT_DataPoolEntry pDataPool = NULL;

/** Specify if new variables should be added on the fly if not found */
static int allocOnTheFly = 0;

/****************************************************************************/

/**
 * @brief Add a new entry in the data-pool.
 * @param pVariable Variable name of the new entry
 * @param pValue Value of the new entry
 * @return Pointer to the new entry or NULL if out of memory.
 */
static PT_DataPoolEntry addEntry(const char *pVariable, const char *pValue)
{
    PT_DataPoolEntry pParent = pDataPool;
    PT_DataPoolEntry pNext;

    // go up to last entry
    while (pParent)
    {
        if (pParent->pNext == NULL)
            break;
        pParent = pParent->pNext;
    }

    // allocate a new entry
    pNext = SYS_malloc(sizeof(T_DataPoolEntry));
    if (pNext)
    {
        pNext->pNext = NULL;
        pNext->pVariable = SYS_malloc(strlen(pVariable) + 1);
        if (pNext->pVariable)
        {
            pNext->pValue = SYS_malloc(DP_VALUE_LENGTH_MAX);
            if (pNext->pValue)
            {
                strcpy(pNext->pVariable, pVariable);
                strncpy(pNext->pValue, pValue, DP_VALUE_LENGTH_MAX - 1);
                if (pParent)
                    pParent->pNext = pNext;
                else
                    pDataPool = pNext;
                return pNext;
            }
        }
    }
    return NULL;
}

/** 
 * @brief Callback function when loading configuration file.
 * @param pParameter Parameter
 * @param pValue Value
 * @return 1 if the line is valid else 0
 */
static int callback_initFromFile(char *pParameter, char* pValue)
{
    addEntry(pParameter, pValue);
  #if OSC_EN
    if (strncmp(app.osc_prefix, pParameter, strlen(app.osc_prefix)) == 0)
    {
        T_OSC_ArgType arg = OSC_getArgType(pValue);
        OSC_appendMessage(pParameter, 1, &arg);
    }
  #endif
    return 1;
}

/****************************************************************************/

/**
 */
int DP_init(const char *pFileName, int onTheFlyAllocation)
{
    int ret = 0;

    // check if already initialized
    if (initialized)
        return 0;

    // initialize variables from a file
    if (pFileName)
    {

      #if OSC_EN
        OSC_initMessages(1);
      #endif

        ret = getConfigFromFile(pFileName, "["CONFIG_SECTION_DATAPOOL"]", callback_initFromFile);

      #if OSC_EN
        OSC_sendMessages(app.osc_host, app.osc_port);
      #endif
    }

    // initialize predefined data-pool
    DPPREDEF_init();
    
    // set on the fly allocation mode
    allocOnTheFly = onTheFlyAllocation;

    // set initialized flag
    initialized = 1;
    
    // initialize user data-pool
    DPUSER_init();
    
    return ret;
}

/**
 */
void DP_deinit(void)
{
    PT_DataPoolEntry pDel;
    PT_DataPoolEntry pData = pDataPool;
    
    // check if initialized
    if (!initialized)
        return;
        
    // de-initialize user data-pool
    DPUSER_deinit();

    // de-initialize predefined data-pool
    DPPREDEF_deinit();

    // clear initialized flag  
    initialized = 0;

    // free all allocated memory
    while (pData)
    {
        pDel = pData;
        pData = pData->pNext;
        SYS_free(pDel->pValue);
        SYS_free(pDel->pVariable);
        SYS_free(pDel);
    }
}

/**
 */
void DP_refresh(void)
{
    // refresh predefined data-pool
    DPUSER_refresh();

    // refresh user data-pool
    DPUSER_refresh();
}
 
/**
 */
const char* DP_getValue(const char *pVariable)
{
    const char *pValue = "";
    PT_DataPoolEntry pData = pDataPool;

    // check if initialized
    if (!initialized)
        return pValue;
    
    if (strncmp(app.user_prefix, pVariable, strlen(app.user_prefix)) == 0)
    {
        // look for entry in user data-pool
        pValue = DPUSER_getValue(pVariable);
    }
    else
    {
        // look for entry in predefined data-pool
        pValue = DPPREDEF_getValue(pVariable);

        if (!pValue)
        {
            // look for entry in data-pool
            while (pData)
            {
                if (strcmp(pData->pVariable, pVariable) == 0)
                {
                    pValue = pData->pValue;
                    break;
                }
                pData = pData->pNext;
            }

            // add new entry
            if (allocOnTheFly && pData == NULL)
            {
                pData = addEntry(pVariable, "");
                pValue = pData->pValue;
            }
        }
    }
    return pValue;
}

/**
 */
void DP_setValue(const char *pVariable, const char *pValue)
{
    PT_DataPoolEntry pData = pDataPool;

    // check if initialized
    if (!initialized)
        return;

    if (strncmp(app.user_prefix, pVariable, strlen(app.user_prefix)) == 0)
    {
        // look for entry in user data-pool
        DPUSER_setValue(pVariable, pValue);
    }
    else
    {
        // look for entry in predefined data-pool
        int isPreDef = DPPREDEF_setValue(pVariable, pValue);

        if (!isPreDef)
        {
            // look for entry in data-pool
            while (pData)
            {
                if (strcmp(pData->pVariable, pVariable) == 0)
                {
                    // update value
                    strncpy(pData->pValue, pValue, DP_VALUE_LENGTH_MAX - 1);
                    break;
                }
                pData = pData->pNext;
            }

            // add new entry
            if (allocOnTheFly && pData == NULL)
            {
                pData = addEntry(pVariable, pValue);
            }
        }
    }

  #if OSC_EN
    // route new value to OSC host
    if (strncmp(app.osc_prefix, pVariable, strlen(app.osc_prefix)) == 0)
    {
        T_OSC_ArgType arg = OSC_getArgType(pValue);
        OSC_initMessages(0);
        OSC_appendMessage(pVariable, 1, &arg);
        OSC_sendMessages(app.osc_host, app.osc_port);
    }
  #endif

    return;
}
