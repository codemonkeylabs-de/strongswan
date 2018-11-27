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

#include "json_plugin.h"

#include <daemon.h>
#include <plugins/plugin_feature.h>

#include "json_config.h"

typedef struct private_json_plugin_ private_json_plugin_t;

struct private_json_plugin_ {
        json_plugin_t  public;
        json_config_t* config;
        const char*    uri;
};

static bool setup_plugin(private_json_plugin_t * this,
                         plugin_feature_t* feature,
                         bool reg,
                         void* cb_data) {
    if (reg) {
      const char* uri = lib->settings->get_str(lib->settings,
                                               "%s.plugins.json.config-server",
                                               NULL,
                                               lib->ns);
      if (!uri) {
        DBG0(DBG_CFG, "JSON plugin: configuration server has not been set");
        return FALSE;
      }

      DBG2(DBG_CFG, "JSON plugin: configuration server URI is %s", uri);
      this->uri = uri;

      this->config = json_config_create();

      charon->backends->add_backend(charon->backends, &this->config->backend);
    }
}

METHOD(plugin_t, get_name, char*, private_json_plugin_t *this)
{
        return "json";
}

METHOD(plugin_t, get_features, int, private_json_plugin_t *this, plugin_feature_t *features[])
{
        static plugin_feature_t f[] = {
          PLUGIN_CALLBACK((plugin_feature_callback_t)setup_plugin, NULL),
                  PLUGIN_PROVIDE(CUSTOM, "json")
        };
        *features = f;
        return countof(f);
}

METHOD(plugin_t, destroy, void, private_json_plugin_t *this)
{
        free(this);
}

plugin_t *json_plugin_create()
{
        private_json_plugin_t *this;

        INIT(this,
                .public = {
                        .plugin = {
                                .get_name = _get_name,
                                .get_features = _get_features,
                                .destroy = _destroy,
                        },
                },
        );

        return &this->public.plugin;
}
