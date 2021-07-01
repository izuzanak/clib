
#ifndef __WD_CONF_H
#define __WD_CONF_H

@begin
include "cl_sys.h"
include "cl_json.h"
include "cl_validator.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libconf_wdl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libconf_wdl_EXPORTS
#define libconf_wdl_EXPORT __declspec(dllexport)
#else
#define libconf_wdl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_WATCHDOG_CONF_OPEN_ERROR 1
#define ERROR_WATCHDOG_CONF_READ_ERROR 2
#define ERROR_WATCHDOG_CONF_PARSE_ERROR 3
#define ERROR_WATCHDOG_CONF_INVALID_CONFIGURATION 4

// === constants and definitions ===============================================

extern validator_s g_config_validator;

// === definition of generated structures ======================================

// -- wd_conf_ip_port_s --
@begin
struct
<
string_s:ip
usi:port
>
options ( from_var )
wd_conf_ip_port_s;
@end

// -- wd_conf_watchdog_s --
@begin
struct
<
string_s:path
ulli:period
>
options ( from_var )
wd_conf_watchdog_s;
@end

// -- wd_config_s --
@begin
struct
<
wd_conf_ip_port_s:channel
wd_conf_watchdog_s:watchdog
>
options ( from_var )
wd_config_s;
@end

WUR libconf_wdl_EXPORT int wd_config_s_read_file(wd_config_s *this,const char *a_file_name);

// === definition of global functions ==========================================

void libconf_wdl_init();
void libconf_wdl_clear();

// === inline methods of generated structures ==================================

// -- wd_conf_ip_port_s --
@begin
inlines wd_conf_ip_port_s
@end

// -- wd_conf_watchdog_s --
@begin
inlines wd_conf_watchdog_s
@end

// -- wd_config_s --
@begin
inlines wd_config_s
@end

#endif

