
@begin
include "hr_conf.h"
@end

// === constants and definitions ===============================================

validator_s g_config_validator;
const char g_config_schema[] =
/*{{{*/
"{\"uint\":[\"type\",1,\">=\",0],\"uint16\":[\"ref\",\"uint\",\"<=\",65535],\"ip_address\":[\"type\",3],\"path\":[\"regex\",\"^[a-zA-Z0-9/\\\\._-]*$\"],\"ip_port_pair\":[\"type\",5,\"items\",[\"ip\",\"ip_address\",\"port\",\"uint16\"]],\"config\":[\"type\",5,\"items\",[\"server\",\"ip_port_pair\",\"upload_dir\",\"path\"]]}";
/*}}}*/

// === methods of generated structures =========================================

// -- hr_conf_ip_port_s --
@begin
methods hr_conf_ip_port_s
@end

// -- hr_config_s --
@begin
methods hr_config_s
@end

int hr_config_s_read_file(hr_config_s *this,const char *a_file_name)
{/*{{{*/
  hr_config_s_clear(this);

  CONT_INIT_CLEAR(file_s,file);

  if (file_s_open(&file,a_file_name,"r"))
  {
    throw_error(OBJECTDB_CONF_OPEN_ERROR);
  }

  CONT_INIT_CLEAR(bc_array_s,conf_buffer);

  if (file_s_read_close(&file,&conf_buffer))
  {
    throw_error(OBJECTDB_CONF_READ_ERROR);
  }
  bc_array_s_push(&conf_buffer,'\0');

  CONT_INIT_CLEAR(json_parser_s,json_parser);
  CONT_INIT_CLEAR(var_s,config_var);

  if (json_parser_s_parse(&json_parser,&conf_buffer,&config_var) ||
      config_var->v_type != c_bi_type_dict)
  {
    throw_error(OBJECTDB_CONF_PARSE_ERROR);
  }

  // - validate configuration -
  VAR_CLEAR(str_config_var,loc_s_string_ptr("config"));
  if (validator_s_validate(&g_config_validator,str_config_var,config_var))
  {
    //DEBUG_PRINT(var_array_s,&g_config_validator.value_stack);
    //DEBUG_PRINT(var_array_s,&g_config_validator.props_stack);

    throw_error(OBJECTDB_CONF_INVALID_CONFIGURATION);
  }

  // - retrieve configuration -
  if (hr_config_s_from_var(this,config_var))
  {
    throw_error(OBJECTDB_CONF_INVALID_CONFIGURATION);
  }

  return 0;
}/*}}}*/

// === global functions ========================================================

void libconf_hrl_init()
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

void libconf_hrl_clear()
{/*{{{*/

  // - clear validator -
  validator_s_clear(&g_config_validator);
}/*}}}*/

