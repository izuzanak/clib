
#ifndef __TD_CONF_H
#define __TD_CONF_H

@begin
include "cl_sys.h"
include "cl_json.h"
include "cl_validator.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libconf_tdl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libconf_tdl_EXPORTS
#define libconf_tdl_EXPORT __declspec(dllexport)
#else
#define libconf_tdl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_TRACED_CONF_OPEN_ERROR 1
#define ERROR_TRACED_CONF_READ_ERROR 2
#define ERROR_TRACED_CONF_PARSE_ERROR 3
#define ERROR_TRACED_CONF_INVALID_CONFIGURATION 4
#define ERROR_TRACED_CONF_NON_UNIQUE_RECORD_ID 5
#define ERROR_TRACED_CONF_NON_UNIQUE_TRACE_ID 6

// === constants and definitions ===============================================

#define TD_JSON_CONFIG_FILE "td_config.json"

// === definition of generated structures ======================================

// -- td_conf_record_s --
@begin
struct
<
string_s:record_id
string_s:type
ui:size
>
td_conf_record_s;
@end

static inline void td_conf_record_s_from_var(td_conf_record_s *this,var_s a_var);

// -- td_conf_record_tree_s --
@begin
rb_tree<td_conf_record_s> td_conf_record_tree_s;
@end

WUR libconf_tdl_EXPORT int td_conf_record_tree_s_from_var(td_conf_record_tree_s *this,var_s a_var);

// -- td_conf_trace_data_s --
@begin
struct
<
string_s:path
ulli:offset
ulli:size
>
td_conf_trace_data_s;
@end

static inline void td_conf_trace_data_s_from_var(td_conf_trace_data_s *this,var_s a_var);

// -- td_conf_trace_s --
@begin
struct
<
string_s:trace_id
string_s:record_id
td_conf_trace_data_s:header
td_conf_trace_data_s:trace
ui:timestamp_div
td_conf_trace_data_s:timestamp
>
td_conf_trace_s;
@end

static inline void td_conf_trace_s_from_var(td_conf_trace_s *this,var_s a_var);

// -- td_conf_trace_tree_s --
@begin
rb_tree<td_conf_trace_s> td_conf_trace_tree_s;
@end

WUR libconf_tdl_EXPORT int td_conf_trace_tree_s_from_var(td_conf_trace_tree_s *this,var_s a_var);

// -- td_config_s --
@begin
struct
<
td_conf_record_tree_s:records
td_conf_trace_tree_s:traces
>
td_config_s;
@end

WUR libconf_tdl_EXPORT int td_config_s_from_var(td_config_s *this,var_s a_var);
WUR libconf_tdl_EXPORT int td_config_s_read_file(td_config_s *this,const char *a_file_name);

// === definition of global functions ==========================================

void libconf_tdl_init();
void libconf_tdl_clear();

// === inline methods of generated structures ==================================

// -- td_conf_record_s --
@begin
inlines td_conf_record_s
@end

static inline void td_conf_record_s_from_var(td_conf_record_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->record_id,loc_s_string_value(loc_s_dict_str_get(a_var,"record_id")));
  string_s_copy(&this->type,loc_s_string_value(loc_s_dict_str_get(a_var,"type")));
  this->size = loc_s_int_value(loc_s_dict_str_get(a_var,"size"));
}/*}}}*/

// -- td_conf_record_tree_s --
@begin
inlines td_conf_record_tree_s
@end

static inline int td_conf_record_tree_s___compare_value(const td_conf_record_tree_s *this,const td_conf_record_s *a_first,const td_conf_record_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->record_id;
  const string_s *second = &a_second->record_id;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }

  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- td_conf_trace_data_s --
@begin
inlines td_conf_trace_data_s
@end

static inline void td_conf_trace_data_s_from_var(td_conf_trace_data_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->path,loc_s_string_value(loc_s_dict_str_get(a_var,"path")));
  this->offset = loc_s_int_value(loc_s_dict_str_get(a_var,"offset"));
  this->size = loc_s_int_value(loc_s_dict_str_get(a_var,"size"));
}/*}}}*/

// -- td_conf_trace_s --
@begin
inlines td_conf_trace_s
@end

static inline void td_conf_trace_s_from_var(td_conf_trace_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->trace_id,loc_s_string_value(loc_s_dict_str_get(a_var,"trace_id")));
  string_s_copy(&this->record_id,loc_s_string_value(loc_s_dict_str_get(a_var,"record_id")));
  td_conf_trace_data_s_from_var(&this->header,loc_s_dict_str_get(a_var,"header"));
  td_conf_trace_data_s_from_var(&this->trace,loc_s_dict_str_get(a_var,"trace"));
  this->timestamp_div = loc_s_int_value(loc_s_dict_str_get(a_var,"timestamp_div"));
  td_conf_trace_data_s_from_var(&this->timestamp,loc_s_dict_str_get(a_var,"timestamp"));
}/*}}}*/

// -- td_conf_trace_tree_s --
@begin
inlines td_conf_trace_tree_s
@end

static inline int td_conf_trace_tree_s___compare_value(const td_conf_trace_tree_s *this,const td_conf_trace_s *a_first,const td_conf_trace_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->trace_id;
  const string_s *second = &a_second->trace_id;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }

  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- td_config_s --
@begin
inlines td_config_s
@end

#endif

