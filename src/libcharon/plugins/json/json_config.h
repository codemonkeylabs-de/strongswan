/*
 * Copyright (C) 2018 Erick Gonzalez
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
 * @defgroup json_config_i json_config
 * @{ @ingroup json
 */

#ifndef JSON_CONFIG_H_
#define JSON_CONFIG_H_

#include <config/backend.h>

typedef struct json_config_t json_config_t;

/**
 * JSON configuration backend.
 */
struct json_config_t {

        /**
         * Implements backend_t interface
         */
        backend_t backend;

        /**
         * Destry the backend.
         */
        void (*destroy)(json_config_t *this);
};

/**
 * Create a json_config backend instance.
 *
 * @return                      backend instance
 */
json_config_t *json_config_create();

#endif /** JSON_CONFIG_H_ @}*/

