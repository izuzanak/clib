
#ifndef __OD_CONF_H
#define __OD_CONF_H

@begin
include "cl_sys.h"
include "cl_json.h"
include "cl_validator.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libconf_odl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libconf_odl_EXPORTS
#define libconf_odl_EXPORT __declspec(dllexport)
#else
#define libconf_odl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_OBJECTDB_CONF_OPEN_ERROR 1
#define ERROR_OBJECTDB_CONF_READ_ERROR 2
#define ERROR_OBJECTDB_CONF_PARSE_ERROR 3
#define ERROR_OBJECTDB_CONF_INVALID_CONFIGURATION 4

// === constants and definitions ===============================================

extern validator_s g_config_validator;

// === definition of generated structures ======================================

// -- od_conf_ip_port_s --
@begin
struct
<
string_s:ip
usi:port
>
options ( from_var )
od_conf_ip_port_s;
@end

// -- od_conf_storage_s --
@begin
struct
<
string_s:path
lli:max_size
>
options ( from_var )
od_conf_storage_s;
@end

// -- od_conf_fuser_s --
@begin
struct
<
string_s:mountpoint
>
options ( from_var )
od_conf_fuser_s;
@end

// -- od_config_s --
@begin
struct
<
od_conf_ip_port_s:channel
od_conf_storage_s:storage
od_conf_fuser_s:fuser
>
options ( from_var )
od_config_s;
@end

WUR libconf_odl_EXPORT int od_config_s_read_file(od_config_s *this,const char *a_file_name);

// === definition of global functions ==========================================

void libconf_odl_init();
void libconf_odl_clear();

// === inline methods of generated structures ==================================

// -- od_conf_ip_port_s --
@begin
inlines od_conf_ip_port_s
@end

// -- od_conf_storage_s --
@begin
inlines od_conf_storage_s
@end

// -- od_conf_fuser_s --
@begin
inlines od_conf_fuser_s
@end

// -- od_config_s --
@begin
inlines od_config_s
@end

#endif

