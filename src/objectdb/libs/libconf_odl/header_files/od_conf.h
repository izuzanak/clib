
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
#define ERROR_OBJECTDB_CONF_NON_UNIQUE_RECORD_ID 5
#define ERROR_OBJECTDB_CONF_NON_UNIQUE_SEGMENT_ID 6
#define ERROR_OBJECTDB_CONF_NON_UNIQUE_TRACE_ID 7

// === constants and definitions ===============================================

#define OD_JSON_CONFIG_FILE "od_config.json"

extern validator_s g_config_validator;

// === definition of generated structures ======================================

// -- od_conf_ip_port_s --
@begin
struct
<
string_s:ip
usi:port
>
od_conf_ip_port_s;
@end

WUR static inline int od_conf_ip_port_s_from_var(od_conf_ip_port_s *this,var_s a_var);

// -- od_config_s --
@begin
struct
<
od_conf_ip_port_s:channel
>
od_config_s;
@end

WUR libconf_odl_EXPORT int od_config_s_from_var(od_config_s *this,var_s a_var);
WUR libconf_odl_EXPORT int od_config_s_read_file(od_config_s *this,const char *a_file_name);

// === definition of global functions ==========================================

void libconf_odl_init();
void libconf_odl_clear();

// === inline methods of generated structures ==================================

// -- od_conf_ip_port_s --
@begin
inlines od_conf_ip_port_s
@end

static inline int od_conf_ip_port_s_from_var(od_conf_ip_port_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->ip,loc_s_string_value(loc_s_dict_str_get(a_var,"ip")));
  this->port = loc_s_int_value(loc_s_dict_str_get(a_var,"port"));

  return 0;
}/*}}}*/

// -- od_config_s --
@begin
inlines od_config_s
@end

#endif

