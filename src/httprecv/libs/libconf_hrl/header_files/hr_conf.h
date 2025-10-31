
#ifndef HR_LIB_CONF_H
#define HR_LIB_CONF_H

@begin
include "cl_sys.h"
include "cl_json.h"
include "cl_validator.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libconf_hrl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libconf_hrl_EXPORTS
#define libconf_hrl_EXPORT __declspec(dllexport)
#else
#define libconf_hrl_EXPORT __declspec(dllimport)
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

// -- hr_conf_ip_port_s --
@begin
struct
<
string_s:ip
usi:port
>
options ( from_var )
hr_conf_ip_port_s;
@end

// -- hr_config_s --
@begin
struct
<
hr_conf_ip_port_s:server
string_s:upload_dir
>
options ( from_var )
hr_config_s;
@end

WUR libconf_hrl_EXPORT int hr_config_s_read_file(hr_config_s *this,const char *a_file_name);

// === definition of global functions ==========================================

void libconf_hrl_init();
void libconf_hrl_clear();

// === inline methods of generated structures ==================================

// -- hr_conf_ip_port_s --
@begin
inlines hr_conf_ip_port_s
@end

// -- hr_config_s --
@begin
inlines hr_config_s
@end

#endif

