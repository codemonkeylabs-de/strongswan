/*
 * Copyright (c) 2018 Erick Gonzalez
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

/**
 * @defgroup json json
 * @ingroup cplugins
 *
 * @defgroup json_plugin json_plugin
 * @{ @ingroup json
 */

#ifndef JSON_PLUGIN_H_
#define JSON_PLUGIN_H_

#include <plugins/plugin.h>

typedef struct json_plugin_t json_plugin_t;

/**
 * JSON configuration plugin
 */
struct json_plugin_t {

        /**
         * implements plugin interface
         */
        plugin_t plugin;
};

#endif /** JSON_PLUGIN_H_ @}*/
