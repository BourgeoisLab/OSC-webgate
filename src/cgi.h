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
 *  @file cgi.h
 *  @brief Functions to handle CGI requests.
 *  @author Frédéric Bourgeois
 *  @version 1.0
 *  @date 5 Aug 2014
 */

#ifndef _CGI_H_
#define _CGI_H_

#include "release.h"
#include "mongoose.h"

/**
 * @defgroup CGI CGI Request
 * @brief Handles the webservers CGI requests.
 * @{
 */

 /**
 * @brief CGI request to get one or more variable values.
 * <b>Examples</b>
 * <pre>
 * http://server_url/cgi-bin/getValue.cgi?variable
 * </pre>
 * @param conn HTTP request containing incoming data
 */
void CGI_processGetValue(struct mg_connection *conn);

/**
 * @brief CGI request to set one or more variable values.
 * <b>Examples</b>
 * <pre>
 * http://server_url/cgi-bin/setValue.cgi?variable=value
 * http://server_url/cgi-bin/setValue.cgi?variable1=value1&variable2=value2
 * </pre>
 * @param conn HTTP request containing incoming data
 */
void CGI_processSetValue(struct mg_connection *conn);

/**
 * @brief Parse incoming JSON data and compute a response JSON data.
 * JSON data can be used to write or read variables from the data-pool.
 *
 * Examples of JSON request and response:
 *
 * <b>Request reading variables:</b>
 *
 *  <PRE>
 *  {
 *   "version":"1",
 *   "read":[
 *           {"var":"/osc/sb_fuzz/switch"},
 *           {"var":"/osc/sb_fuzz/drive"},
 *           {"var":"/osc/sb_fuzz/clip"}
 *          ]
 *  }
 *  </PRE>
 *
 * <b>Response:</b>
 *
 *  <PRE>
 *  {
 *   "version":"1",
 *   "read":[
 *           {"var":"/osc/sb_fuzz/switch","val":"1"},
 *           {"var":"/osc/sb_fuzz/drive","val":"26"},
 *           {"var":"/osc/sb_fuzz/clip","val":"0.6"}
 *          ]
 *  }
 *  </PRE>
 *
 * <b>Request writing a variable:</b>
 *
 *  <PRE>
 *  {
 *   "version":"1",
 *   "write":[
 *           {"var":"/osc/sb_fuzz/drive","val":"30"}
 *          ]
 *  }
 *  </PRE>
 *
 * <b>Response:</b>
 *
 *  <PRE>
 *  {
 *   "version":"1",
 *   "write":[
 *           {"var":"/osc/sb_fuzz/drive","val":"30"}
 *          ]
 *  }
 *  </PRE>
 * @param conn HTTP request containing incoming data
 */
void CGI_processJSON(struct mg_connection *conn);

/** @} CGI */

#endif // _CGI_H_
