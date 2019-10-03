
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
#define ERROR_SEGMENTD_CONF_NON_UNIQUE_RECORD_ID 5
#define ERROR_SEGMENTD_CONF_NON_UNIQUE_SEGMENT_ID 6
#define ERROR_SEGMENTD_CONF_NON_UNIQUE_TRACE_ID 7

// === constants and definitions ===============================================

#define SD_JSON_CONFIG_FILE "sd_config.json"

extern validator_s g_config_validator;

// - segment type identifiers -
enum
{
  c_sd_segment_type_FILE = 0,
};

// - trace data type identifiers -
enum
{
  c_sd_trace_data_type_MMAP = 0,
  c_sd_trace_data_type_SEGMENT,
};

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

WUR static inline int sd_conf_ip_port_s_from_var(sd_conf_ip_port_s *this,var_s a_var);

// -- sd_conf_segment_s --
@begin
struct
<
string_s:segment_id
ui:size
ui:type
string_s:path
>
sd_conf_segment_s;
@end

WUR static inline int sd_conf_segment_s_from_var(sd_conf_segment_s *this,var_s a_var);

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

WUR static inline int sd_conf_record_s_from_var(sd_conf_record_s *this,var_s a_var);

// -- sd_conf_mmap_s --
@begin
struct
<
string_s:path
ulli:offset
ulli:size
>
sd_conf_mmap_s;
@end

WUR static inline int sd_conf_mmap_s_from_var(sd_conf_mmap_s *this,var_s a_var);

// -- sd_conf_trace_data_s --
@begin
struct
<
ui:type
sd_conf_mmap_s:mmap
sd_conf_segment_s:segment
>
sd_conf_trace_data_s;
@end

WUR static inline int sd_conf_trace_data_s_from_var(sd_conf_trace_data_s *this,var_s a_var);

// -- sd_conf_trace_s --
@begin
struct
<
string_s:trace_id
sd_conf_record_s:record
sd_conf_trace_data_s:header
sd_conf_mmap_s:trace
ui:timestamp_div
sd_conf_mmap_s:timestamp
>
sd_conf_trace_s;
@end

WUR static inline int sd_conf_trace_s_from_var(sd_conf_trace_s *this,var_s a_var);

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

static inline int sd_conf_ip_port_s_from_var(sd_conf_ip_port_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->ip,loc_s_string_value(loc_s_dict_str_get(a_var,"ip")));
  this->port = loc_s_int_value(loc_s_dict_str_get(a_var,"port"));

  return 0;
}/*}}}*/

// -- sd_conf_segment_s --
@begin
inlines sd_conf_segment_s
@end

static inline int sd_conf_segment_s_from_var(sd_conf_segment_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->segment_id,loc_s_string_value(loc_s_dict_str_get(a_var,"segment_id")));
  this->size = loc_s_int_value(loc_s_dict_str_get(a_var,"size"));

  const string_s *type = loc_s_string_value(loc_s_dict_str_get(a_var,"type"));

  if (strcmp(type->data,"file") == 0)
  {
    this->type = c_sd_segment_type_FILE;
  }
  else
  {
    debug_assert(0);
  }

  string_s_copy(&this->path,loc_s_string_value(loc_s_dict_str_get(a_var,"path")));

  return 0;
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

static inline int sd_conf_record_s_from_var(sd_conf_record_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->type,loc_s_string_value(loc_s_dict_str_get(a_var,"type")));
  this->size = loc_s_int_value(loc_s_dict_str_get(a_var,"size"));

  return 0;
}/*}}}*/

// -- sd_conf_mmap_s --
@begin
inlines sd_conf_mmap_s
@end

static inline int sd_conf_mmap_s_from_var(sd_conf_mmap_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->path,loc_s_string_value(loc_s_dict_str_get(a_var,"path")));
  this->offset = loc_s_int_value(loc_s_dict_str_get(a_var,"offset"));
  this->size = loc_s_int_value(loc_s_dict_str_get(a_var,"size"));

  return 0;
}/*}}}*/

// -- sd_conf_trace_data_s --
@begin
inlines sd_conf_trace_data_s
@end

static inline int sd_conf_trace_data_s_from_var(sd_conf_trace_data_s *this,var_s a_var)
{/*{{{*/
  var_s type_var = loc_s_dict_str_get(a_var,"type");
  var_s data_var = loc_s_dict_get(a_var,type_var);

  // - validate trace data configuration -
  if (validator_s_validate(&g_config_validator,type_var,data_var))
  {
    throw_error(SEGMENTD_CONF_INVALID_CONFIGURATION);
  }

  const string_s *type = loc_s_string_value(type_var);

  if (strcmp(type->data,"mmap") == 0)
  {
    this->type = c_sd_trace_data_type_MMAP;
    if (sd_conf_mmap_s_from_var(&this->mmap,data_var))
    {
      throw_error(SEGMENTD_CONF_INVALID_CONFIGURATION);
    }
  }
  else if (strcmp(type->data,"segment") == 0)
  {
    this->type = c_sd_trace_data_type_SEGMENT;
    if (sd_conf_segment_s_from_var(&this->segment,data_var))
    {
      throw_error(SEGMENTD_CONF_INVALID_CONFIGURATION);
    }
  }
  else
  {
    debug_assert(0);
  }

  return 0;
}/*}}}*/

// -- sd_conf_trace_s --
@begin
inlines sd_conf_trace_s
@end

static inline int sd_conf_trace_s_from_var(sd_conf_trace_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(&this->trace_id,loc_s_string_value(loc_s_dict_str_get(a_var,"trace_id")));
  this->timestamp_div = loc_s_int_value(loc_s_dict_str_get(a_var,"timestamp_div"));

  if (sd_conf_record_s_from_var(&this->record,loc_s_dict_str_get(a_var,"record")) ||
      sd_conf_trace_data_s_from_var(&this->header,loc_s_dict_str_get(a_var,"header")) ||
      sd_conf_mmap_s_from_var(&this->trace,loc_s_dict_str_get(a_var,"trace")) ||
      sd_conf_mmap_s_from_var(&this->timestamp,loc_s_dict_str_get(a_var,"timestamp")))
  {
    throw_error(SEGMENTD_CONF_INVALID_CONFIGURATION);
  }

  return 0;
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

