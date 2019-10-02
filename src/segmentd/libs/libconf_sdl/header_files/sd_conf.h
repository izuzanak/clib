
#ifndef __SD_CONF_H
#define __SD_CONF_H

@begin
include "cl_sys.h"
include "cl_json.h"
include "cl_validator.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libconf_sdl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libconf_sdl_EXPORTS
#define libconf_sdl_EXPORT __declspec(dllexport)
#else
#define libconf_sdl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_SEGMENTD_CONF_OPEN_ERROR 1
#define ERROR_SEGMENTD_CONF_READ_ERROR 2
#define ERROR_SEGMENTD_CONF_PARSE_ERROR 3
#define ERROR_SEGMENTD_CONF_INVALID_CONFIGURATION 4
#define ERROR_SEGMENTD_CONF_INVALID_SEGMENTS_CONFIGURATION 5
#define ERROR_SEGMENTD_CONF_INVALID_TRACES_CONFIGURATION 6
#define ERROR_SEGMENTD_CONF_NON_UNIQUE_RECORD_ID 7
#define ERROR_SEGMENTD_CONF_NON_UNIQUE_SEGMENT_ID 8
#define ERROR_SEGMENTD_CONF_NON_UNIQUE_TRACE_ID 9

// === constants and definitions ===============================================

#define SD_JSON_CONFIG_FILE "sd_config.json"

// === definition of generated structures ======================================

// -- sd_conf_ip_port_s --
@begin
struct
<
string_s:ip
usi:port
>
sd_conf_ip_port_s;
@end

static inline void sd_conf_ip_port_s_from_var(sd_conf_ip_port_s *this,var_s a_var);

// -- sd_conf_segment_s --
@begin
struct
<
string_s:segment_id
ui:size
string_s:type
string_s:path
>
sd_conf_segment_s;
@end

static inline void sd_conf_segment_s_from_var(sd_conf_segment_s *this,var_s a_var);

// -- sd_conf_segment_tree_s --
@begin
safe_rb_tree<sd_conf_segment_s> sd_conf_segment_tree_s;
@end

WUR libconf_sdl_EXPORT int sd_conf_segment_tree_s_from_var(sd_conf_segment_tree_s *this,var_s a_var);

// -- sd_conf_record_s --
@begin
struct
<
string_s:type
ui:size
>
sd_conf_record_s;
@end

static inline void sd_conf_record_s_from_var(sd_conf_record_s *this,var_s a_var);

// -- sd_conf_trace_data_s --
@begin
struct
<
string_s:path
ulli:offset
ulli:size
>
sd_conf_trace_data_s;
@end

static inline void sd_conf_trace_data_s_from_var(sd_conf_trace_data_s *this,var_s a_var);

// -- sd_conf_trace_s --
@begin
struct
<
string_s:trace_id
sd_conf_record_s:record
sd_conf_trace_data_s:header
sd_conf_trace_data_s:trace
ui:timestamp_div
sd_conf_trace_data_s:timestamp
>
sd_conf_trace_s;
@end

static inline void sd_conf_trace_s_from_var(sd_conf_trace_s *this,var_s a_var);

// -- sd_conf_trace_tree_s --
@begin
safe_rb_tree<sd_conf_trace_s> sd_conf_trace_tree_s;
@end

WUR libconf_sdl_EXPORT int sd_conf_trace_tree_s_from_var(sd_conf_trace_tree_s *this,var_s a_var);

// -- sd_config_s --
@begin
struct
<
sd_conf_ip_port_s:channel
sd_conf_segment_tree_s:segments
sd_conf_trace_tree_s:traces
>
sd_config_s;
@end

WUR libconf_sdl_EXPORT int sd_config_s_from_var(sd_config_s *this,var_s a_var);
WUR libconf_sdl_EXPORT int sd_config_s_read_file(sd_config_s *this,const char *a_file_name);

// === definition of global functions ==========================================

void libconf_sdl_init();
void libconf_sdl_clear();

// === inline methods of generated structures ==================================

// -- sd_conf_ip_port_s --
@begin
inlines sd_conf_ip_port_s
@end

static inline void sd_conf_ip_port_s_from_var(sd_conf_ip_port_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->ip,loc_s_string_value(loc_s_dict_str_get(a_var,"ip")));
  this->port = loc_s_int_value(loc_s_dict_str_get(a_var,"port"));
}/*}}}*/

// -- sd_conf_segment_s --
@begin
inlines sd_conf_segment_s
@end

static inline void sd_conf_segment_s_from_var(sd_conf_segment_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->segment_id,loc_s_string_value(loc_s_dict_str_get(a_var,"segment_id")));
  this->size = loc_s_int_value(loc_s_dict_str_get(a_var,"size"));
  string_s_copy(&this->type,loc_s_string_value(loc_s_dict_str_get(a_var,"type")));
  string_s_copy(&this->path,loc_s_string_value(loc_s_dict_str_get(a_var,"path")));
}/*}}}*/

// -- sd_conf_segment_tree_s --
@begin
inlines sd_conf_segment_tree_s
@end

static inline int sd_conf_segment_tree_s___compare_value(const sd_conf_segment_tree_s *this,const sd_conf_segment_s *a_first,const sd_conf_segment_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->segment_id;
  const string_s *second = &a_second->segment_id;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }

  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- sd_conf_record_s --
@begin
inlines sd_conf_record_s
@end

static inline void sd_conf_record_s_from_var(sd_conf_record_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->type,loc_s_string_value(loc_s_dict_str_get(a_var,"type")));
  this->size = loc_s_int_value(loc_s_dict_str_get(a_var,"size"));
}/*}}}*/

// -- sd_conf_trace_data_s --
@begin
inlines sd_conf_trace_data_s
@end

static inline void sd_conf_trace_data_s_from_var(sd_conf_trace_data_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->path,loc_s_string_value(loc_s_dict_str_get(a_var,"path")));
  this->offset = loc_s_int_value(loc_s_dict_str_get(a_var,"offset"));
  this->size = loc_s_int_value(loc_s_dict_str_get(a_var,"size"));
}/*}}}*/

// -- sd_conf_trace_s --
@begin
inlines sd_conf_trace_s
@end

static inline void sd_conf_trace_s_from_var(sd_conf_trace_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->trace_id,loc_s_string_value(loc_s_dict_str_get(a_var,"trace_id")));
  sd_conf_record_s_from_var(&this->record,loc_s_dict_str_get(a_var,"record"));
  sd_conf_trace_data_s_from_var(&this->header,loc_s_dict_str_get(a_var,"header"));
  sd_conf_trace_data_s_from_var(&this->trace,loc_s_dict_str_get(a_var,"trace"));
  this->timestamp_div = loc_s_int_value(loc_s_dict_str_get(a_var,"timestamp_div"));
  sd_conf_trace_data_s_from_var(&this->timestamp,loc_s_dict_str_get(a_var,"timestamp"));
}/*}}}*/

// -- sd_conf_trace_tree_s --
@begin
inlines sd_conf_trace_tree_s
@end

static inline int sd_conf_trace_tree_s___compare_value(const sd_conf_trace_tree_s *this,const sd_conf_trace_s *a_first,const sd_conf_trace_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->trace_id;
  const string_s *second = &a_second->trace_id;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }

  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- sd_config_s --
@begin
inlines sd_config_s
@end

#endif

