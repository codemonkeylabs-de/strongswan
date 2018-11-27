AM_CPPFLAGS = \
	-I$(top_srcdir)/src/libstrongswan \
	-I$(top_srcdir)/src/libcharon

AM_CFLAGS = \
	$(PLUGIN_CFLAGS)

if MONOLITHIC
noinst_LTLIBRARIES = libstrongswan-json.la
else
plugin_LTLIBRARIES = libstrongswan-json.la
endif

libstrongswan_json_la_SOURCES = \
	json_plugin.h json_plugin.c json_config.h json_config.c

libstrongswan_json_la_LDFLAGS = -module -avoid-version
