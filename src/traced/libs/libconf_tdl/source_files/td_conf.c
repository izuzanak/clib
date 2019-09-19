
@begin
include "td_conf.h"
@end

// === constants and definitions ===============================================

validator_s g_config_validator;
const char g_config_schema[] =
/*{{{*/
"{\"ip_address\":[\"type\",3],\"path\":[\"regex\",\"^[a-zA-Z0-9/\\\\._-]\\\\+$\"],\"int\":[\"type\",1],\"uint\":[\"type\",1,\">=\",0],\"uint16\":[\"ref\",\"uint\",\"<=\",65535],\"uint32\":[\"ref\",\"uint\",\"<=\",4294967295],\"string_not_empty\":[\"type\",3,\"length >\",0],\"data\":[\"type\",5,\"items\",[\"path\",\"path\",\"offset\",\"uint\",\"size\",\"uint\"]],\"record\":[\"type\",5,\"items\",[\"record_id\",\"string_not_empty\",\"type\",[\"==\",\"raw\"],\"size\",\"uint\"]],\"trace\":[\"type\",5,\"items\",[\"trace_id\",\"string_not_empty\",\"record_id\",\"string_not_empty\",\"header\",\"data\",\"trace\",\"data\",\"timestamp_div\",\"int\",\"timestamp\",\"data\"]],\"config\":[\"type\",5,\"items\",[\"records\",[\"type\",4,\"all-items\",\"record\"],\"traces\",[\"type\",4,\"all-items\",\"trace\"]]]}";
/*}}}*/

// === methods of generated structures =========================================

// -- td_conf_record_s --
@begin
methods td_conf_record_s
@end

// -- td_conf_record_tree_s --
@begin
methods td_conf_record_tree_s
@end

int td_conf_record_tree_s_from_var(td_conf_record_tree_s *this,var_s a_var)
{/*{{{*/
  td_conf_record_tree_s_clear(this);

  var_array_s *conf_records_arr = loc_s_array_value(a_var);

  if (conf_records_arr->used != 0)
  {
    var_s *cr_ptr = conf_records_arr->data;
    var_s *cr_ptr_end = cr_ptr + conf_records_arr->used;
    do {
      CONT_INIT_CLEAR(td_conf_record_s,conf_record);
      td_conf_record_s_from_var(&conf_record,*cr_ptr);

      // - check uniqueness of record id -
      unsigned cr_index = td_conf_record_tree_s_get_idx(this,&conf_record);
      if (cr_index != c_idx_not_exist)
      {
        throw_error(TRACED_CONF_NON_UNIQUE_RECORD_ID);
      }

      td_conf_record_tree_s_swap_insert(this,&conf_record);
    } while(++cr_ptr < cr_ptr_end);
  }

  return 0;
}/*}}}*/

// -- td_conf_trace_data_s --
@begin
methods td_conf_trace_data_s
@end

// -- td_conf_trace_s --
@begin
methods td_conf_trace_s
@end

// -- td_conf_trace_tree_s --
@begin
methods td_conf_trace_tree_s
@end

int td_conf_trace_tree_s_from_var(td_conf_trace_tree_s *this,var_s a_var)
{/*{{{*/
  td_conf_trace_tree_s_clear(this);

  var_array_s *conf_traces_arr = loc_s_array_value(a_var);

  if (conf_traces_arr->used != 0)
  {
    var_s *ct_ptr = conf_traces_arr->data;
    var_s *ct_ptr_end = ct_ptr + conf_traces_arr->used;
    do {
      CONT_INIT_CLEAR(td_conf_trace_s,conf_trace);
      td_conf_trace_s_from_var(&conf_trace,*ct_ptr);

      // - check uniqueness of trace id -
      unsigned ct_index = td_conf_trace_tree_s_get_idx(this,&conf_trace);
      if (ct_index != c_idx_not_exist)
      {
        throw_error(TRACED_CONF_NON_UNIQUE_TRACE_ID);
      }

      td_conf_trace_tree_s_swap_insert(this,&conf_trace);
    } while(++ct_ptr < ct_ptr_end);
  }

  return 0;
}/*}}}*/

// -- td_config_s --
@begin
methods td_config_s
@end

int td_config_s_from_var(td_config_s *this,var_s a_var)
{/*{{{*/
  if (td_conf_record_tree_s_from_var(&this->records,loc_s_dict_str_get(a_var,"records")))
  {
    throw_error(TRACED_CONF_INVALID_CONFIGURATION);
  }

  if (td_conf_trace_tree_s_from_var(&this->traces,loc_s_dict_str_get(a_var,"traces")))
  {
    throw_error(TRACED_CONF_INVALID_CONFIGURATION);
  }

  return 0;
}/*}}}*/

int td_config_s_read_file(td_config_s *this,const char *a_file_name)
{/*{{{*/
  td_config_s_clear(this);

  CONT_INIT_CLEAR(file_s,file);

  if (file_s_open(&file,a_file_name,"r"))
  {
    throw_error(TRACED_CONF_OPEN_ERROR);
  }

  CONT_INIT_CLEAR(bc_array_s,conf_buffer);

  if (file_s_read_close(&file,&conf_buffer))
  {
    throw_error(TRACED_CONF_READ_ERROR);
  }
  bc_array_s_push(&conf_buffer,'\0');

  CONT_INIT_CLEAR(json_parser_s,json_parser);
  CONT_INIT_CLEAR(var_s,config_var);

  if (json_parser_s_parse(&json_parser,&conf_buffer,&config_var) ||
      config_var->v_type != c_bi_type_dict)
  {
    throw_error(TRACED_CONF_PARSE_ERROR);
  }

  // - validate configuration -
  VAR_CLEAR(str_config_var,loc_s_string_ptr("config"));
  if (validator_s_validate(&g_config_validator,str_config_var,config_var))
  {
    DEBUG_PRINT(var_array_s,&g_config_validator.value_stack);
    DEBUG_PRINT(var_array_s,&g_config_validator.props_stack);

    throw_error(TRACED_CONF_INVALID_CONFIGURATION);
  }

  // - retrieve configuration -
  if (td_config_s_from_var(this,config_var))
  {
    throw_error(TRACED_CONF_INVALID_CONFIGURATION);
  }

  return 0;
}/*}}}*/

// === global functions ========================================================

void libconf_tdl_init()
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

void libconf_tdl_clear()
{/*{{{*/

  // - clear validator -
  validator_s_clear(&g_config_validator);
}/*}}}*/

