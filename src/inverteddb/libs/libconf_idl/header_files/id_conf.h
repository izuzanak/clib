
#ifndef ID_LIB_CONF_H
#define ID_LIB_CONF_H

@begin
include "cl_sys.h"
include "cl_json.h"
include "cl_validator.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libconf_idl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libconf_idl_EXPORTS
#define libconf_idl_EXPORT __declspec(dllexport)
#else
#define libconf_idl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_INVERTEDDB_CONF_OPEN_ERROR 1
#define ERROR_INVERTEDDB_CONF_READ_ERROR 2
#define ERROR_INVERTEDDB_CONF_PARSE_ERROR 3
#define ERROR_INVERTEDDB_CONF_INVALID_CONFIGURATION 4

// === constants and definitions ===============================================

extern validator_s g_config_validator;

// === definition of generated structures ======================================

// -- id_conf_ip_port_s --
@begin
struct
<
string_s:ip
usi:port
>
options ( from_var )
id_conf_ip_port_s;
@end

// -- id_conf_database_s --
@begin
struct
<
string_s:name
string_s:storage
ui:max_indexes
ui:merge_cnt
>
options ( from_var )
id_conf_database_s;
@end

// -- id_conf_database_tree_s --
@begin
safe_rb_tree<id_conf_database_s>
options ( from_var )
id_conf_database_tree_s;
@end

// -- id_config_s --
@begin
struct
<
id_conf_ip_port_s:channel
id_conf_ip_port_s:watchdog
id_conf_database_tree_s:databases
>
options ( from_var )
id_config_s;
@end

WUR libconf_idl_EXPORT int id_config_s_read_file(id_config_s *this,const char *a_file_name);

// === definition of global functions ==========================================

void libconf_idl_init();
void libconf_idl_clear();

// === inline methods of generated structures ==================================

// -- id_conf_ip_port_s --
@begin
inlines id_conf_ip_port_s
@end

// -- id_conf_database_s --
@begin
inlines id_conf_database_s
@end

// -- id_conf_database_tree_s --
@begin
inlines id_conf_database_tree_s
@end

static inline int id_conf_database_tree_s___compare_value(const id_conf_database_tree_s *this,const id_conf_database_s *a_first,const id_conf_database_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->name;
  const string_s *second = &a_second->name;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }
  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- id_config_s --
@begin
inlines id_config_s
@end

#endif

