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

#include "json_config.h"

#include <daemon.h>

typedef struct private_json_config_t private_json_config_t;

struct private_json_config_t {
        json_config_t public;

};

typedef struct {
        enumerator_t public;
        private_json_config_t *this;
} peer_enumerator_t;

typedef struct {
        enumerator_t public;
        private_json_config_t *this;
} ike_enumerator_t;

METHOD(enumerator_t,
       peer_enumerator_enumerate,
       bool,
       peer_enumerator_t *this,
       va_list args)
{
        return FALSE;
}

METHOD(enumerator_t,
       peer_enumerator_destroy,
       void,
       peer_enumerator_t *this)
{
        free(this);
}


METHOD(backend_t,
       create_peer_cfg_enumerator,
       enumerator_t*,
       private_json_config_t *this,
       identification_t *me,
       identification_t *other)
{
        peer_enumerator_t *e;

        INIT(e,
                .public = {
                        .enumerate = enumerator_enumerate_default,
                        .venumerate = _peer_enumerator_enumerate,
                        .destroy = _peer_enumerator_destroy,
                },
                .this = this
        );

        /* TODO actually make a request to the configuration server */

        DBG1(DBG_CFG, "JSON plugin: request peer configs matching %Y - %Y (not implemented)",
             me, other);

        free(e);
        return NULL;
        //        return &e->public;
}

METHOD(enumerator_t,
       ike_enumerator_enumerate,
       bool,
       ike_enumerator_t *this,
       va_list args)
{
        return FALSE;
}

METHOD(enumerator_t,
       ike_enumerator_destroy,
       void,
       ike_enumerator_t *this)
{
        free(this);
}

METHOD(backend_t,
       create_ike_cfg_enumerator,
       enumerator_t*,
       private_json_config_t *this,
       host_t *me,
       host_t *other)
{
        ike_enumerator_t *e;

        INIT(e,
                .public = {
                        .enumerate  = enumerator_enumerate_default,
                        .venumerate = _ike_enumerator_enumerate,
                        .destroy    = _ike_enumerator_destroy,
                },
                .this = this
             );

        DBG1(DBG_CFG, "JSON plugin request IKE config for %H..%H", me, other);

        free(e);
        return NULL;

        //        return &e->public;
}

METHOD(backend_t,
       get_peer_cfg_by_name,
       peer_cfg_t*,
       private_json_config_t *this,
       char *name)
{
        peer_cfg_t *peer_cfg = NULL;

        DBG1(DBG_CFG, "JSON plugin: get config for peer %s", name);

        return peer_cfg;
}

METHOD(json_config_t, destroy, void, private_json_config_t *this)
{
        free(this);
}

json_config_t *json_config_create() {
        private_json_config_t *this;


        INIT(this,
                .public = {
                        .backend = {
                                .create_peer_cfg_enumerator = _create_peer_cfg_enumerator,
                                .create_ike_cfg_enumerator  = _create_ike_cfg_enumerator,
                                .get_peer_cfg_by_name       = _get_peer_cfg_by_name
                        },
                        .destroy = _destroy
                }
        );

        return &this->public;
}
