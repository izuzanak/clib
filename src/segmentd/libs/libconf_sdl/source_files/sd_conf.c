
@begin
include "sd_conf.h"
@end

// === constants and definitions ===============================================

validator_s g_config_validator;
const char g_config_schema[] =
/*{{{*/
"{\"ip_address\":[\"type\",3],\"ip_port_pair\":[\"type\",5,\"items\",[\"ip\",\"ip_address\",\"port\",\"uint16\"]],\"path\":[\"regex\",\"^[a-zA-Z0-9/\\\\._-]\\\\+$\"],\"int\":[\"type\",1],\"uint\":[\"type\",1,\">=\",0],\"uint16\":[\"ref\",\"uint\",\"<=\",65535],\"uint32\":[\"ref\",\"uint\",\"<=\",4294967295],\"string_not_empty\":[\"type\",3,\"length >\",0],\"segment\":[\"type\",5,\"items\",[\"segment_id\",\"string_not_empty\",\"size\",\"uint\",\"type\",[\"regex\",\"^\\\\(file\\\\|files\\\\)$\"],\"path\",\"path\"]],\"mmap\":[\"type\",5,\"items\",[\"path\",\"path\",\"offset\",\"uint\",\"size\",\"uint\"]],\"record\":[\"type\",5,\"items\",[\"type\",[\"==\",\"raw\"],\"size\",\"uint\"]],\"trace_data\":[\"type\",5,\"items\",[\"type\",\"string_not_empty\"]],\"trace\":[\"type\",5,\"items\",[\"trace_id\",\"string_not_empty\",\"record\",\"record\",\"header\",\"trace_data\",\"trace\",\"mmap\",\"timestamp_div\",\"int\",\"timestamp\",\"trace_data\"]],\"config\":[\"type\",5,\"items\",[\"channel\",\"ip_port_pair\",\"segments\",[\"type\",4,\"all-items\",\"segment\"],\"traces\",[\"type\",4,\"all-items\",\"trace\"]]]}";
/*}}}*/

// === methods of generated structures =========================================

// -- sd_conf_ip_port_s --
@begin
methods sd_conf_ip_port_s
@end

// -- sd_conf_segment_s --
@begin
methods sd_conf_segment_s
@end

// -- sd_conf_segment_tree_s --
@begin
methods sd_conf_segment_tree_s
@end

int sd_conf_segment_tree_s_from_var(sd_conf_segment_tree_s *this,var_s a_var)
{/*{{{*/
  sd_conf_segment_tree_s_clear(this);

  var_array_s *conf_segments_arr = loc_s_array_value(a_var);

  if (conf_segments_arr->used != 0)
  {
    var_s *ct_ptr = conf_segments_arr->data;
    var_s *ct_ptr_end = ct_ptr + conf_segments_arr->used;
    do {
      CONT_INIT_CLEAR(sd_conf_segment_s,conf_segment);
      if (sd_conf_segment_s_from_var(&conf_segment,*ct_ptr))
      {
        throw_error(SEGMENTD_CONF_INVALID_CONFIGURATION);
      }

      // - check uniqueness of segment id -
      unsigned ct_index = sd_conf_segment_tree_s_get_idx(this,&conf_segment);
      if (ct_index != c_idx_not_exist)
      {
        throw_error(SEGMENTD_CONF_NON_UNIQUE_SEGMENT_ID);
      }

      sd_conf_segment_tree_s_swap_insert(this,&conf_segment);
    } while(++ct_ptr < ct_ptr_end);
  }

  return 0;
}/*}}}*/

// -- sd_conf_record_s --
@begin
methods sd_conf_record_s
@end

// -- sd_conf_mmap_s --
@begin
methods sd_conf_mmap_s
@end

// -- sd_conf_trace_data_s --
@begin
methods sd_conf_trace_data_s
@end

// -- sd_conf_trace_s --
@begin
methods sd_conf_trace_s
@end

// -- sd_conf_trace_tree_s --
@begin
methods sd_conf_trace_tree_s
@end

int sd_conf_trace_tree_s_from_var(sd_conf_trace_tree_s *this,var_s a_var)
{/*{{{*/
  sd_conf_trace_tree_s_clear(this);

  var_array_s *conf_traces_arr = loc_s_array_value(a_var);

  if (conf_traces_arr->used != 0)
  {
    var_s *ct_ptr = conf_traces_arr->data;
    var_s *ct_ptr_end = ct_ptr + conf_traces_arr->used;
    do {
      CONT_INIT_CLEAR(sd_conf_trace_s,conf_trace);
      if (sd_conf_trace_s_from_var(&conf_trace,*ct_ptr))
      {
        throw_error(SEGMENTD_CONF_INVALID_CONFIGURATION);
      }

      // - check uniqueness of trace id -
      unsigned ct_index = sd_conf_trace_tree_s_get_idx(this,&conf_trace);
      if (ct_index != c_idx_not_exist)
      {
        throw_error(SEGMENTD_CONF_NON_UNIQUE_TRACE_ID);
      }

      sd_conf_trace_tree_s_swap_insert(this,&conf_trace);
    } while(++ct_ptr < ct_ptr_end);
  }

  return 0;
}/*}}}*/

// -- sd_config_s --
@begin
methods sd_config_s
@end

int sd_config_s_from_var(sd_config_s *this,var_s a_var)
{/*{{{*/
  if (sd_conf_ip_port_s_from_var(&this->channel,loc_s_dict_str_get(a_var,"channel")) ||
      sd_conf_segment_tree_s_from_var(&this->segments,loc_s_dict_str_get(a_var,"segments")) ||
      sd_conf_trace_tree_s_from_var(&this->traces,loc_s_dict_str_get(a_var,"traces")))
  {
    throw_error(SEGMENTD_CONF_INVALID_CONFIGURATION);
  }

  return 0;
}/*}}}*/

int sd_config_s_read_file(sd_config_s *this,const char *a_file_name)
{/*{{{*/
  sd_config_s_clear(this);

  CONT_INIT_CLEAR(file_s,file);

  if (file_s_open(&file,a_file_name,"r"))
  {
    throw_error(SEGMENTD_CONF_OPEN_ERROR);
  }

  CONT_INIT_CLEAR(bc_array_s,conf_buffer);

  if (file_s_read_close(&file,&conf_buffer))
  {
    throw_error(SEGMENTD_CONF_READ_ERROR);
  }
  bc_array_s_push(&conf_buffer,'\0');

  CONT_INIT_CLEAR(json_parser_s,json_parser);
  CONT_INIT_CLEAR(var_s,config_var);

  if (json_parser_s_parse(&json_parser,&conf_buffer,&config_var) ||
      config_var->v_type != c_bi_type_dict)
  {
    throw_error(SEGMENTD_CONF_PARSE_ERROR);
  }

  // - validate configuration -
  VAR_CLEAR(str_config_var,loc_s_string_ptr("config"));
  if (validator_s_validate(&g_config_validator,str_config_var,config_var))
  {
    //DEBUG_PRINT(var_array_s,&g_config_validator.value_stack);
    //DEBUG_PRINT(var_array_s,&g_config_validator.props_stack);

    throw_error(SEGMENTD_CONF_INVALID_CONFIGURATION);
  }

  // - retrieve configuration -
  if (sd_config_s_from_var(this,config_var))
  {
    throw_error(SEGMENTD_CONF_INVALID_CONFIGURATION);
  }

  return 0;
}/*}}}*/

// === global functions ========================================================

void libconf_sdl_init()
{/*{{{*/

  // - init validator -
  CONT_INIT_CLEAR(json_parser_s,schema_parser);
  CONT_INIT_CLEAR(var_s,schema_var);

  unsigned cs_length = strlen(g_config_schema);
  bc_array_s schema_buffer = {cs_length,cs_length,(char *)g_config_schema};
  cassert(json_parser_s_parse(&schema_parser,&schema_buffer,&schema_var) == 0);

  validator_s_init(&g_config_validator);
  cassert(validator_s_create(&g_config_validator,schema_var) == 0);
}/*}}}*/

void libconf_sdl_clear()
{/*{{{*/

  // - clear validator -
  validator_s_clear(&g_config_validator);
}/*}}}*/

