
@begin
include "cl_vdv301_gen.h"
@end

// === methods of generated structures =========================================

// VDV301 GENERATED METHODS

// -- ConnectionStateEnumeration_s --
@begin
methods ConnectionStateEnumeration_s
@end

string_tree_s g_ConnectionStateEnumeration_tree;

int ConnectionStateEnumeration_s_write(ConnectionStateEnumeration_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->value < 1 || this->value >= g_ConnectionStateEnumeration_tree.used)
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  string_s *value = string_tree_s_at(&g_ConnectionStateEnumeration_tree,this->value);
  xml_create_append_string(value->size - 1,value->data,a_trg);
  return 0;
}/*}}}*/

int ConnectionStateEnumeration_s_read(ConnectionStateEnumeration_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  if (value_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  this->value = string_tree_s_get_idx(&g_ConnectionStateEnumeration_tree,loc_s_string_value(value_var));
  if (this->value == c_idx_not_exist)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- ConnectionTypeEnumeration_s --
@begin
methods ConnectionTypeEnumeration_s
@end

string_tree_s g_ConnectionTypeEnumeration_tree;

int ConnectionTypeEnumeration_s_write(ConnectionTypeEnumeration_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->value < 1 || this->value >= g_ConnectionTypeEnumeration_tree.used)
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  string_s *value = string_tree_s_at(&g_ConnectionTypeEnumeration_tree,this->value);
  xml_create_append_string(value->size - 1,value->data,a_trg);
  return 0;
}/*}}}*/

int ConnectionTypeEnumeration_s_read(ConnectionTypeEnumeration_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  if (value_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  this->value = string_tree_s_get_idx(&g_ConnectionTypeEnumeration_tree,loc_s_string_value(value_var));
  if (this->value == c_idx_not_exist)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- DoorCountingObjectClassEnumeration_s --
@begin
methods DoorCountingObjectClassEnumeration_s
@end

string_tree_s g_DoorCountingObjectClassEnumeration_tree;

int DoorCountingObjectClassEnumeration_s_write(DoorCountingObjectClassEnumeration_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->value < 1 || this->value >= g_DoorCountingObjectClassEnumeration_tree.used)
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  string_s *value = string_tree_s_at(&g_DoorCountingObjectClassEnumeration_tree,this->value);
  xml_create_append_string(value->size - 1,value->data,a_trg);
  return 0;
}/*}}}*/

int DoorCountingObjectClassEnumeration_s_read(DoorCountingObjectClassEnumeration_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  if (value_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  this->value = string_tree_s_get_idx(&g_DoorCountingObjectClassEnumeration_tree,loc_s_string_value(value_var));
  if (this->value == c_idx_not_exist)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- DoorCountingQualityEnumeration_s --
@begin
methods DoorCountingQualityEnumeration_s
@end

string_tree_s g_DoorCountingQualityEnumeration_tree;

int DoorCountingQualityEnumeration_s_write(DoorCountingQualityEnumeration_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->value < 1 || this->value >= g_DoorCountingQualityEnumeration_tree.used)
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  string_s *value = string_tree_s_at(&g_DoorCountingQualityEnumeration_tree,this->value);
  xml_create_append_string(value->size - 1,value->data,a_trg);
  return 0;
}/*}}}*/

int DoorCountingQualityEnumeration_s_read(DoorCountingQualityEnumeration_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  if (value_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  this->value = string_tree_s_get_idx(&g_DoorCountingQualityEnumeration_tree,loc_s_string_value(value_var));
  if (this->value == c_idx_not_exist)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- DoorOpenStateEnumeration_s --
@begin
methods DoorOpenStateEnumeration_s
@end

string_tree_s g_DoorOpenStateEnumeration_tree;

int DoorOpenStateEnumeration_s_write(DoorOpenStateEnumeration_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->value < 1 || this->value >= g_DoorOpenStateEnumeration_tree.used)
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  string_s *value = string_tree_s_at(&g_DoorOpenStateEnumeration_tree,this->value);
  xml_create_append_string(value->size - 1,value->data,a_trg);
  return 0;
}/*}}}*/

int DoorOpenStateEnumeration_s_read(DoorOpenStateEnumeration_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  if (value_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  this->value = string_tree_s_get_idx(&g_DoorOpenStateEnumeration_tree,loc_s_string_value(value_var));
  if (this->value == c_idx_not_exist)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- DoorOperationStateEnumeration_s --
@begin
methods DoorOperationStateEnumeration_s
@end

string_tree_s g_DoorOperationStateEnumeration_tree;

int DoorOperationStateEnumeration_s_write(DoorOperationStateEnumeration_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->value < 1 || this->value >= g_DoorOperationStateEnumeration_tree.used)
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  string_s *value = string_tree_s_at(&g_DoorOperationStateEnumeration_tree,this->value);
  xml_create_append_string(value->size - 1,value->data,a_trg);
  return 0;
}/*}}}*/

int DoorOperationStateEnumeration_s_read(DoorOperationStateEnumeration_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  if (value_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  this->value = string_tree_s_get_idx(&g_DoorOperationStateEnumeration_tree,loc_s_string_value(value_var));
  if (this->value == c_idx_not_exist)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- ErrorCodeEnumeration_s --
@begin
methods ErrorCodeEnumeration_s
@end

string_tree_s g_ErrorCodeEnumeration_tree;

int ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->value < 1 || this->value >= g_ErrorCodeEnumeration_tree.used)
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  string_s *value = string_tree_s_at(&g_ErrorCodeEnumeration_tree,this->value);
  xml_create_append_string(value->size - 1,value->data,a_trg);
  return 0;
}/*}}}*/

int ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  if (value_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  this->value = string_tree_s_get_idx(&g_ErrorCodeEnumeration_tree,loc_s_string_value(value_var));
  if (this->value == c_idx_not_exist)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- TicketRazziaInformationEnumeration_s --
@begin
methods TicketRazziaInformationEnumeration_s
@end

string_tree_s g_TicketRazziaInformationEnumeration_tree;

int TicketRazziaInformationEnumeration_s_write(TicketRazziaInformationEnumeration_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->value < 1 || this->value >= g_TicketRazziaInformationEnumeration_tree.used)
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  string_s *value = string_tree_s_at(&g_TicketRazziaInformationEnumeration_tree,this->value);
  xml_create_append_string(value->size - 1,value->data,a_trg);
  return 0;
}/*}}}*/

int TicketRazziaInformationEnumeration_s_read(TicketRazziaInformationEnumeration_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  if (value_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  this->value = string_tree_s_get_idx(&g_TicketRazziaInformationEnumeration_tree,loc_s_string_value(value_var));
  if (this->value == c_idx_not_exist)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- TicketValidationEnumeration_s --
@begin
methods TicketValidationEnumeration_s
@end

string_tree_s g_TicketValidationEnumeration_tree;

int TicketValidationEnumeration_s_write(TicketValidationEnumeration_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->value < 1 || this->value >= g_TicketValidationEnumeration_tree.used)
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  string_s *value = string_tree_s_at(&g_TicketValidationEnumeration_tree,this->value);
  xml_create_append_string(value->size - 1,value->data,a_trg);
  return 0;
}/*}}}*/

int TicketValidationEnumeration_s_read(TicketValidationEnumeration_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  if (value_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  this->value = string_tree_s_get_idx(&g_TicketValidationEnumeration_tree,loc_s_string_value(value_var));
  if (this->value == c_idx_not_exist)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- CountingStateEnumeration_s --
@begin
methods CountingStateEnumeration_s
@end

string_tree_s g_CountingStateEnumeration_tree;

int CountingStateEnumeration_s_write(CountingStateEnumeration_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->value < 1 || this->value >= g_CountingStateEnumeration_tree.used)
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  string_s *value = string_tree_s_at(&g_CountingStateEnumeration_tree,this->value);
  xml_create_append_string(value->size - 1,value->data,a_trg);
  return 0;
}/*}}}*/

int CountingStateEnumeration_s_read(CountingStateEnumeration_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  if (value_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  this->value = string_tree_s_get_idx(&g_CountingStateEnumeration_tree,loc_s_string_value(value_var));
  if (this->value == c_idx_not_exist)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- ErrorCodeEnumeration_array_s --
@begin
methods ErrorCodeEnumeration_array_s
@end

// -- ConnectionStateEnumeration_array_s --
@begin
methods ConnectionStateEnumeration_array_s
@end

// -- DoorCountingQualityEnumeration_array_s --
@begin
methods DoorCountingQualityEnumeration_array_s
@end

// -- TicketingService_SetRazziaRequestStructure_s --
@begin
methods TicketingService_SetRazziaRequestStructure_s
@end

int TicketingService_SetRazziaRequestStructure_s_write(TicketingService_SetRazziaRequestStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<TicketInformationRazziaStatus>");
  if (TicketRazziaInformationEnumeration_s_write(&this->TicketInformationRazziaStatus,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</TicketInformationRazziaStatus>");
  return 0;
}/*}}}*/

int TicketingService_SetRazziaRequestStructure_s_read(TicketingService_SetRazziaRequestStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s TicketInformationRazziaStatus = loc_s_dict_str_get(nodes_var,"TicketInformationRazziaStatus");
  if (TicketInformationRazziaStatus == NULL ||
      TicketRazziaInformationEnumeration_s_read(&this->TicketInformationRazziaStatus,TicketInformationRazziaStatus))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_int_s --
@begin
methods IBIS_IP_int_s
@end

int IBIS_IP_int_s_write(IBIS_IP_int_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_int_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int IBIS_IP_int_s_read(IBIS_IP_int_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_int_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_byte_s --
@begin
methods IBIS_IP_byte_s
@end

int IBIS_IP_byte_s_write(IBIS_IP_byte_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_byte_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int IBIS_IP_byte_s_read(IBIS_IP_byte_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_byte_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_double_s --
@begin
methods IBIS_IP_double_s
@end

int IBIS_IP_double_s_write(IBIS_IP_double_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_double_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int IBIS_IP_double_s_read(IBIS_IP_double_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_double_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_string_s --
@begin
methods IBIS_IP_string_s
@end

int IBIS_IP_string_s_write(IBIS_IP_string_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_string_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int IBIS_IP_string_s_read(IBIS_IP_string_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_string_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_NMTOKEN_s --
@begin
methods IBIS_IP_NMTOKEN_s
@end

int IBIS_IP_NMTOKEN_s_write(IBIS_IP_NMTOKEN_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_NMTOKEN_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int IBIS_IP_NMTOKEN_s_read(IBIS_IP_NMTOKEN_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_NMTOKEN_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DoorIdType_s --
@begin
methods DoorIdType_s
@end

int DoorIdType_s_write(DoorIdType_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_NMTOKEN_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int DoorIdType_s_read(DoorIdType_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_NMTOKEN_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_boolean_s --
@begin
methods IBIS_IP_boolean_s
@end

int IBIS_IP_boolean_s_write(IBIS_IP_boolean_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_boolean_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int IBIS_IP_boolean_s_read(IBIS_IP_boolean_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_boolean_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_dateTime_s --
@begin
methods IBIS_IP_dateTime_s
@end

int IBIS_IP_dateTime_s_write(IBIS_IP_dateTime_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_dateTime_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int IBIS_IP_dateTime_s_read(IBIS_IP_dateTime_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_dateTime_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_duration_s --
@begin
methods IBIS_IP_duration_s
@end

int IBIS_IP_duration_s_write(IBIS_IP_duration_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_duration_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int IBIS_IP_duration_s_read(IBIS_IP_duration_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_duration_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_language_s --
@begin
methods IBIS_IP_language_s
@end

int IBIS_IP_language_s_write(IBIS_IP_language_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_language_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int IBIS_IP_language_s_read(IBIS_IP_language_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_language_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- GNSSCoordinateStructure_s --
@begin
methods GNSSCoordinateStructure_s
@end

int GNSSCoordinateStructure_s_write(GNSSCoordinateStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Degree>");
  if (IBIS_IP_double_s_write(&this->Degree,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Degree>");
  bc_array_s_append_ptr(a_trg,"<Direction>");
  if (IBIS_IP_string_s_write(&this->Direction,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Direction>");
  return 0;
}/*}}}*/

int GNSSCoordinateStructure_s_read(GNSSCoordinateStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Degree = loc_s_dict_str_get(nodes_var,"Degree");
  if (Degree == NULL ||
      IBIS_IP_double_s_read(&this->Degree,Degree))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Direction = loc_s_dict_str_get(nodes_var,"Direction");
  if (Direction == NULL ||
      IBIS_IP_string_s_read(&this->Direction,Direction))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_unsignedInt_s --
@begin
methods IBIS_IP_unsignedInt_s
@end

int IBIS_IP_unsignedInt_s_write(IBIS_IP_unsignedInt_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_unsignedInt_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int IBIS_IP_unsignedInt_s_read(IBIS_IP_unsignedInt_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_unsignedInt_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_unsignedLong_s --
@begin
methods IBIS_IP_unsignedLong_s
@end

int IBIS_IP_unsignedLong_s_write(IBIS_IP_unsignedLong_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_unsignedLong_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int IBIS_IP_unsignedLong_s_read(IBIS_IP_unsignedLong_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_unsignedLong_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- TicketingService_ValidationResultDataStructure_s --
@begin
methods TicketingService_ValidationResultDataStructure_s
@end

int TicketingService_ValidationResultDataStructure_s_write(TicketingService_ValidationResultDataStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<TimeStamp>");
  if (IBIS_IP_dateTime_s_write(&this->TimeStamp,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</TimeStamp>");
  bc_array_s_append_ptr(a_trg,"<ValidationResult>");
  if (TicketValidationEnumeration_s_write(&this->ValidationResult,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</ValidationResult>");
  return 0;
}/*}}}*/

int TicketingService_ValidationResultDataStructure_s_read(TicketingService_ValidationResultDataStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s TimeStamp = loc_s_dict_str_get(nodes_var,"TimeStamp");
  if (TimeStamp == NULL ||
      IBIS_IP_dateTime_s_read(&this->TimeStamp,TimeStamp))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ValidationResult = loc_s_dict_str_get(nodes_var,"ValidationResult");
  if (ValidationResult == NULL ||
      TicketValidationEnumeration_s_read(&this->ValidationResult,ValidationResult))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_nonNegativeInteger_s --
@begin
methods IBIS_IP_nonNegativeInteger_s
@end

int IBIS_IP_nonNegativeInteger_s_write(IBIS_IP_nonNegativeInteger_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_nonNegativeInteger_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int IBIS_IP_nonNegativeInteger_s_read(IBIS_IP_nonNegativeInteger_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_nonNegativeInteger_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DoorOpenStateStructure_s --
@begin
methods DoorOpenStateStructure_s
@end

int DoorOpenStateStructure_s_write(DoorOpenStateStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (DoorOpenStateEnumeration_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int DoorOpenStateStructure_s_read(DoorOpenStateStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      DoorOpenStateEnumeration_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DoorOperationStateStructure_s --
@begin
methods DoorOperationStateStructure_s
@end

int DoorOperationStateStructure_s_write(DoorOperationStateStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (DoorOperationStateEnumeration_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int DoorOperationStateStructure_s_read(DoorOperationStateStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      DoorOperationStateEnumeration_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- InternationalTextType_s --
@begin
methods InternationalTextType_s
@end

int InternationalTextType_s_write(InternationalTextType_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Value>");
  if (xs_string_s_write(&this->Value,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Value>");
  bc_array_s_append_ptr(a_trg,"<Language>");
  if (xs_language_s_write(&this->Language,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Language>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  return 0;
}/*}}}*/

int InternationalTextType_s_read(InternationalTextType_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Value = loc_s_dict_str_get(nodes_var,"Value");
  if (Value == NULL ||
      xs_string_s_read(&this->Value,Value))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Language = loc_s_dict_str_get(nodes_var,"Language");
  if (Language == NULL ||
      xs_language_s_read(&this->Language,Language))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DoorCountingStructure_s --
@begin
methods DoorCountingStructure_s
@end

int DoorCountingStructure_s_write(DoorCountingStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<ObjectClass>");
  if (DoorCountingObjectClassEnumeration_s_write(&this->ObjectClass,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</ObjectClass>");
  bc_array_s_append_ptr(a_trg,"<In>");
  if (IBIS_IP_int_s_write(&this->In,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</In>");
  bc_array_s_append_ptr(a_trg,"<Out>");
  if (IBIS_IP_int_s_write(&this->Out,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Out>");
  if (this->CountQuality.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<CountQuality>");
    if (DoorCountingQualityEnumeration_s_write(DoorCountingQualityEnumeration_array_s_last(&this->CountQuality),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</CountQuality>");
  }
  return 0;
}/*}}}*/

int DoorCountingStructure_s_read(DoorCountingStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ObjectClass = loc_s_dict_str_get(nodes_var,"ObjectClass");
  if (ObjectClass == NULL ||
      DoorCountingObjectClassEnumeration_s_read(&this->ObjectClass,ObjectClass))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s In = loc_s_dict_str_get(nodes_var,"In");
  if (In == NULL ||
      IBIS_IP_int_s_read(&this->In,In))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Out = loc_s_dict_str_get(nodes_var,"Out");
  if (Out == NULL ||
      IBIS_IP_int_s_read(&this->Out,Out))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CountQuality = loc_s_dict_str_get(nodes_var,"CountQuality");
  if (CountQuality != NULL)
  {
    if (CountQuality->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(CountQuality);
      if (array_ptr->used != 0)
      {
        DoorCountingQualityEnumeration_array_s_copy_resize(&this->CountQuality,1);
        DoorCountingQualityEnumeration_array_s_push_blank(&this->CountQuality);
        if (DoorCountingQualityEnumeration_s_read(DoorCountingQualityEnumeration_array_s_last(&this->CountQuality),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      DoorCountingQualityEnumeration_array_s_copy_resize(&this->CountQuality,1);
      DoorCountingQualityEnumeration_array_s_push_blank(&this->CountQuality);
      if (DoorCountingQualityEnumeration_s_read(DoorCountingQualityEnumeration_array_s_last(&this->CountQuality),CountQuality))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DoorIdType_array_s --
@begin
methods DoorIdType_array_s
@end

// -- IBIS_IP_int_array_s --
@begin
methods IBIS_IP_int_array_s
@end

// -- IBIS_IP_byte_array_s --
@begin
methods IBIS_IP_byte_array_s
@end

// -- IBIS_IP_double_array_s --
@begin
methods IBIS_IP_double_array_s
@end

// -- IBIS_IP_string_array_s --
@begin
methods IBIS_IP_string_array_s
@end

// -- IBIS_IP_NMTOKEN_array_s --
@begin
methods IBIS_IP_NMTOKEN_array_s
@end

// -- PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s --
@begin
methods PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s
@end

int PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s_write(PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<DoorID>");
  if (IBIS_IP_NMTOKEN_s_write(&this->DoorID,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</DoorID>");
  return 0;
}/*}}}*/

int PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s_read(PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DoorID = loc_s_dict_str_get(nodes_var,"DoorID");
  if (DoorID == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->DoorID,DoorID))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- TicketingService_RetrieveTariffInformationRequestStructure_s --
@begin
methods TicketingService_RetrieveTariffInformationRequestStructure_s
@end

int TicketingService_RetrieveTariffInformationRequestStructure_s_write(TicketingService_RetrieveTariffInformationRequestStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<TripRef>");
  if (IBIS_IP_NMTOKEN_s_write(&this->TripRef,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</TripRef>");
  bc_array_s_append_ptr(a_trg,"<LineRef>");
  if (IBIS_IP_NMTOKEN_s_write(&this->LineRef,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</LineRef>");
  bc_array_s_append_ptr(a_trg,"<StopRef>");
  if (IBIS_IP_NMTOKEN_s_write(&this->StopRef,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</StopRef>");
  return 0;
}/*}}}*/

int TicketingService_RetrieveTariffInformationRequestStructure_s_read(TicketingService_RetrieveTariffInformationRequestStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s TripRef = loc_s_dict_str_get(nodes_var,"TripRef");
  if (TripRef == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->TripRef,TripRef))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s LineRef = loc_s_dict_str_get(nodes_var,"LineRef");
  if (LineRef == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->LineRef,LineRef))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s StopRef = loc_s_dict_str_get(nodes_var,"StopRef");
  if (StopRef == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->StopRef,StopRef))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_boolean_array_s --
@begin
methods IBIS_IP_boolean_array_s
@end

// -- DoorIdListType_s --
@begin
methods DoorIdListType_s
@end

int DoorIdListType_s_write(DoorIdListType_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->DoorID.used != 0)
  {
    DoorIdType_s *ptr = this->DoorID.data;
    DoorIdType_s *ptr_end = ptr + this->DoorID.used;
    do {
      bc_array_s_append_ptr(a_trg,"<DoorID>");
      if (DoorIdType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</DoorID>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int DoorIdListType_s_read(DoorIdListType_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DoorID = loc_s_dict_str_get(nodes_var,"DoorID");
  if (DoorID != NULL)
  {
    if (DoorID->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(DoorID);
      DoorIdType_array_s_copy_resize(&this->DoorID,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          DoorIdType_array_s_push_blank(&this->DoorID);
          if (DoorIdType_s_read(DoorIdType_array_s_last(&this->DoorID),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      DoorIdType_array_s_copy_resize(&this->DoorID,1);
      DoorIdType_array_s_push_blank(&this->DoorID);
      if (DoorIdType_s_read(DoorIdType_array_s_last(&this->DoorID),DoorID))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- IBIS_IP_dateTime_array_s --
@begin
methods IBIS_IP_dateTime_array_s
@end

// -- IBIS_IP_duration_array_s --
@begin
methods IBIS_IP_duration_array_s
@end

// -- IBIS_IP_unsignedInt_array_s --
@begin
methods IBIS_IP_unsignedInt_array_s
@end

// -- BayAreaStructure_s --
@begin
methods BayAreaStructure_s
@end

int BayAreaStructure_s_write(BayAreaStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->BeforeBay.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<BeforeBay>");
    if (IBIS_IP_double_s_write(IBIS_IP_double_array_s_last(&this->BeforeBay),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</BeforeBay>");
  }
  if (this->BehindBay.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<BehindBay>");
    if (IBIS_IP_double_s_write(IBIS_IP_double_array_s_last(&this->BehindBay),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</BehindBay>");
  }
  return 0;
}/*}}}*/

int BayAreaStructure_s_read(BayAreaStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s BeforeBay = loc_s_dict_str_get(nodes_var,"BeforeBay");
  if (BeforeBay != NULL)
  {
    if (BeforeBay->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(BeforeBay);
      if (array_ptr->used != 0)
      {
        IBIS_IP_double_array_s_copy_resize(&this->BeforeBay,1);
        IBIS_IP_double_array_s_push_blank(&this->BeforeBay);
        if (IBIS_IP_double_s_read(IBIS_IP_double_array_s_last(&this->BeforeBay),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_double_array_s_copy_resize(&this->BeforeBay,1);
      IBIS_IP_double_array_s_push_blank(&this->BeforeBay);
      if (IBIS_IP_double_s_read(IBIS_IP_double_array_s_last(&this->BeforeBay),BeforeBay))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s BehindBay = loc_s_dict_str_get(nodes_var,"BehindBay");
  if (BehindBay != NULL)
  {
    if (BehindBay->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(BehindBay);
      if (array_ptr->used != 0)
      {
        IBIS_IP_double_array_s_copy_resize(&this->BehindBay,1);
        IBIS_IP_double_array_s_push_blank(&this->BehindBay);
        if (IBIS_IP_double_s_read(IBIS_IP_double_array_s_last(&this->BehindBay),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_double_array_s_copy_resize(&this->BehindBay,1);
      IBIS_IP_double_array_s_push_blank(&this->BehindBay);
      if (IBIS_IP_double_s_read(IBIS_IP_double_array_s_last(&this->BehindBay),BehindBay))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DoorCountingStructure_array_s --
@begin
methods DoorCountingStructure_array_s
@end

// -- InternationalTextType_array_s --
@begin
methods InternationalTextType_array_s
@end

// -- IBIS_IP_nonNegativeInteger_array_s --
@begin
methods IBIS_IP_nonNegativeInteger_array_s
@end

// -- DoorOperationStateStructure_array_s --
@begin
methods DoorOperationStateStructure_array_s
@end

// -- TicketingService_ValidationResultDataStructure_array_s --
@begin
methods TicketingService_ValidationResultDataStructure_array_s
@end

// -- CountingStateStructure_s --
@begin
methods CountingStateStructure_s
@end

int CountingStateStructure_s_write(CountingStateStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<DoorID>");
  if (DoorIdType_s_write(&this->DoorID,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</DoorID>");
  bc_array_s_append_ptr(a_trg,"<CountingState>");
  if (CountingStateEnumeration_s_write(&this->CountingState,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</CountingState>");
  return 0;
}/*}}}*/

int CountingStateStructure_s_read(CountingStateStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DoorID = loc_s_dict_str_get(nodes_var,"DoorID");
  if (DoorID == NULL ||
      DoorIdType_s_read(&this->DoorID,DoorID))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CountingState = loc_s_dict_str_get(nodes_var,"CountingState");
  if (CountingState == NULL ||
      CountingStateEnumeration_s_read(&this->CountingState,CountingState))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- DoorCountingListStructure_s --
@begin
methods DoorCountingListStructure_s
@end

int DoorCountingListStructure_s_write(DoorCountingListStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<DoorID>");
  if (IBIS_IP_NMTOKEN_s_write(&this->DoorID,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</DoorID>");
  if (this->CountSet.used != 0)
  {
    DoorCountingStructure_s *ptr = this->CountSet.data;
    DoorCountingStructure_s *ptr_end = ptr + this->CountSet.used;
    do {
      bc_array_s_append_ptr(a_trg,"<CountSet>");
      if (DoorCountingStructure_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</CountSet>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int DoorCountingListStructure_s_read(DoorCountingListStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DoorID = loc_s_dict_str_get(nodes_var,"DoorID");
  if (DoorID == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->DoorID,DoorID))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CountSet = loc_s_dict_str_get(nodes_var,"CountSet");
  if (CountSet != NULL)
  {
    if (CountSet->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(CountSet);
      DoorCountingStructure_array_s_copy_resize(&this->CountSet,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          DoorCountingStructure_array_s_push_blank(&this->CountSet);
          if (DoorCountingStructure_s_read(DoorCountingStructure_array_s_last(&this->CountSet),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      DoorCountingStructure_array_s_copy_resize(&this->CountSet,1);
      DoorCountingStructure_array_s_push_blank(&this->CountSet);
      if (DoorCountingStructure_s_read(DoorCountingStructure_array_s_last(&this->CountSet),CountSet))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- AnnouncementStructure_s --
@begin
methods AnnouncementStructure_s
@end

int AnnouncementStructure_s_write(AnnouncementStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<AnnouncementRef>");
  if (IBIS_IP_NMTOKEN_s_write(&this->AnnouncementRef,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</AnnouncementRef>");
  if (this->AnnouncementText.used != 0)
  {
    InternationalTextType_s *ptr = this->AnnouncementText.data;
    InternationalTextType_s *ptr_end = ptr + this->AnnouncementText.used;
    do {
      bc_array_s_append_ptr(a_trg,"<AnnouncementText>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</AnnouncementText>");
    } while(++ptr < ptr_end);
  }
  if (this->AnnouncementTTSText.used != 0)
  {
    InternationalTextType_s *ptr = this->AnnouncementTTSText.data;
    InternationalTextType_s *ptr_end = ptr + this->AnnouncementTTSText.used;
    do {
      bc_array_s_append_ptr(a_trg,"<AnnouncementTTSText>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</AnnouncementTTSText>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int AnnouncementStructure_s_read(AnnouncementStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s AnnouncementRef = loc_s_dict_str_get(nodes_var,"AnnouncementRef");
  if (AnnouncementRef == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->AnnouncementRef,AnnouncementRef))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s AnnouncementText = loc_s_dict_str_get(nodes_var,"AnnouncementText");
  if (AnnouncementText != NULL)
  {
    if (AnnouncementText->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(AnnouncementText);
      InternationalTextType_array_s_copy_resize(&this->AnnouncementText,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->AnnouncementText);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->AnnouncementText),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->AnnouncementText,1);
      InternationalTextType_array_s_push_blank(&this->AnnouncementText);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->AnnouncementText),AnnouncementText))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s AnnouncementTTSText = loc_s_dict_str_get(nodes_var,"AnnouncementTTSText");
  if (AnnouncementTTSText != NULL)
  {
    if (AnnouncementTTSText->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(AnnouncementTTSText);
      InternationalTextType_array_s_copy_resize(&this->AnnouncementTTSText,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->AnnouncementTTSText);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->AnnouncementTTSText),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->AnnouncementTTSText,1);
      InternationalTextType_array_s_push_blank(&this->AnnouncementTTSText);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->AnnouncementTTSText),AnnouncementTTSText))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- CardType_s --
@begin
methods CardType_s
@end

int CardType_s_write(CardType_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<CardSerialNumber>");
  if (IBIS_IP_NMTOKEN_s_write(&this->CardSerialNumber,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</CardSerialNumber>");
  bc_array_s_append_ptr(a_trg,"<CardTypeID>");
  if (IBIS_IP_NMTOKEN_s_write(&this->CardTypeID,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</CardTypeID>");
  if (this->CardTypeText.used != 0)
  {
    InternationalTextType_s *ptr = this->CardTypeText.data;
    InternationalTextType_s *ptr_end = ptr + this->CardTypeText.used;
    do {
      bc_array_s_append_ptr(a_trg,"<CardTypeText>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</CardTypeText>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int CardType_s_read(CardType_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardSerialNumber = loc_s_dict_str_get(nodes_var,"CardSerialNumber");
  if (CardSerialNumber == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->CardSerialNumber,CardSerialNumber))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardTypeID = loc_s_dict_str_get(nodes_var,"CardTypeID");
  if (CardTypeID == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->CardTypeID,CardTypeID))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardTypeText = loc_s_dict_str_get(nodes_var,"CardTypeText");
  if (CardTypeText != NULL)
  {
    if (CardTypeText->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(CardTypeText);
      InternationalTextType_array_s_copy_resize(&this->CardTypeText,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->CardTypeText);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->CardTypeText),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->CardTypeText,1);
      InternationalTextType_array_s_push_blank(&this->CardTypeText);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->CardTypeText),CardTypeText))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DestinationStructure_s --
@begin
methods DestinationStructure_s
@end

int DestinationStructure_s_write(DestinationStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<DestinationRef>");
  if (IBIS_IP_NMTOKEN_s_write(&this->DestinationRef,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</DestinationRef>");
  if (this->DestinationName.used != 0)
  {
    InternationalTextType_s *ptr = this->DestinationName.data;
    InternationalTextType_s *ptr_end = ptr + this->DestinationName.used;
    do {
      bc_array_s_append_ptr(a_trg,"<DestinationName>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</DestinationName>");
    } while(++ptr < ptr_end);
  }
  if (this->DestinationShortName.used != 0)
  {
    InternationalTextType_s *ptr = this->DestinationShortName.data;
    InternationalTextType_s *ptr_end = ptr + this->DestinationShortName.used;
    do {
      bc_array_s_append_ptr(a_trg,"<DestinationShortName>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</DestinationShortName>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int DestinationStructure_s_read(DestinationStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DestinationRef = loc_s_dict_str_get(nodes_var,"DestinationRef");
  if (DestinationRef == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->DestinationRef,DestinationRef))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DestinationName = loc_s_dict_str_get(nodes_var,"DestinationName");
  if (DestinationName != NULL)
  {
    if (DestinationName->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(DestinationName);
      InternationalTextType_array_s_copy_resize(&this->DestinationName,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->DestinationName);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->DestinationName),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->DestinationName,1);
      InternationalTextType_array_s_push_blank(&this->DestinationName);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->DestinationName),DestinationName))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s DestinationShortName = loc_s_dict_str_get(nodes_var,"DestinationShortName");
  if (DestinationShortName != NULL)
  {
    if (DestinationShortName->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(DestinationShortName);
      InternationalTextType_array_s_copy_resize(&this->DestinationShortName,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->DestinationShortName);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->DestinationShortName),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->DestinationShortName,1);
      InternationalTextType_array_s_push_blank(&this->DestinationShortName);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->DestinationShortName),DestinationShortName))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- VehicleStructure_s --
@begin
methods VehicleStructure_s
@end

int VehicleStructure_s_write(VehicleStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<VehicleTypeRef>");
  if (IBIS_IP_NMTOKEN_s_write(&this->VehicleTypeRef,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</VehicleTypeRef>");
  if (this->Name.used != 0)
  {
    InternationalTextType_s *ptr = this->Name.data;
    InternationalTextType_s *ptr_end = ptr + this->Name.used;
    do {
      bc_array_s_append_ptr(a_trg,"<Name>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</Name>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int VehicleStructure_s_read(VehicleStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s VehicleTypeRef = loc_s_dict_str_get(nodes_var,"VehicleTypeRef");
  if (VehicleTypeRef == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->VehicleTypeRef,VehicleTypeRef))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Name = loc_s_dict_str_get(nodes_var,"Name");
  if (Name != NULL)
  {
    if (Name->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(Name);
      InternationalTextType_array_s_copy_resize(&this->Name,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->Name);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->Name),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->Name,1);
      InternationalTextType_array_s_push_blank(&this->Name);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->Name),Name))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- ZoneType_s --
@begin
methods ZoneType_s
@end

int ZoneType_s_write(ZoneType_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<FarezoneTypeID>");
  if (IBIS_IP_NMTOKEN_s_write(&this->FarezoneTypeID,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</FarezoneTypeID>");
  if (this->FareZoneTypeName.used != 0)
  {
    InternationalTextType_s *ptr = this->FareZoneTypeName.data;
    InternationalTextType_s *ptr_end = ptr + this->FareZoneTypeName.used;
    do {
      bc_array_s_append_ptr(a_trg,"<FareZoneTypeName>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</FareZoneTypeName>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int ZoneType_s_read(ZoneType_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s FarezoneTypeID = loc_s_dict_str_get(nodes_var,"FarezoneTypeID");
  if (FarezoneTypeID == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->FarezoneTypeID,FarezoneTypeID))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s FareZoneTypeName = loc_s_dict_str_get(nodes_var,"FareZoneTypeName");
  if (FareZoneTypeName != NULL)
  {
    if (FareZoneTypeName->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(FareZoneTypeName);
      InternationalTextType_array_s_copy_resize(&this->FareZoneTypeName,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->FareZoneTypeName);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->FareZoneTypeName),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->FareZoneTypeName,1);
      InternationalTextType_array_s_push_blank(&this->FareZoneTypeName);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->FareZoneTypeName),FareZoneTypeName))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- CardApplInformations_s --
@begin
methods CardApplInformations_s
@end

int CardApplInformations_s_write(CardApplInformations_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<CardApplInformationLength>");
  if (IBIS_IP_unsignedInt_s_write(&this->CardApplInformationLength,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</CardApplInformationLength>");
  if (this->CardApplInformationData.used != 0)
  {
    IBIS_IP_byte_s *ptr = this->CardApplInformationData.data;
    IBIS_IP_byte_s *ptr_end = ptr + this->CardApplInformationData.used;
    do {
      bc_array_s_append_ptr(a_trg,"<CardApplInformationData>");
      if (IBIS_IP_byte_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</CardApplInformationData>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int CardApplInformations_s_read(CardApplInformations_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardApplInformationLength = loc_s_dict_str_get(nodes_var,"CardApplInformationLength");
  if (CardApplInformationLength == NULL ||
      IBIS_IP_unsignedInt_s_read(&this->CardApplInformationLength,CardApplInformationLength))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardApplInformationData = loc_s_dict_str_get(nodes_var,"CardApplInformationData");
  if (CardApplInformationData != NULL)
  {
    if (CardApplInformationData->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(CardApplInformationData);
      IBIS_IP_byte_array_s_copy_resize(&this->CardApplInformationData,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          IBIS_IP_byte_array_s_push_blank(&this->CardApplInformationData);
          if (IBIS_IP_byte_s_read(IBIS_IP_byte_array_s_last(&this->CardApplInformationData),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      IBIS_IP_byte_array_s_copy_resize(&this->CardApplInformationData,1);
      IBIS_IP_byte_array_s_push_blank(&this->CardApplInformationData);
      if (IBIS_IP_byte_s_read(IBIS_IP_byte_array_s_last(&this->CardApplInformationData),CardApplInformationData))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- GlobalCardStatus_s --
@begin
methods GlobalCardStatus_s
@end

int GlobalCardStatus_s_write(GlobalCardStatus_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<GlobalCardStausID>");
  if (IBIS_IP_unsignedInt_s_write(&this->GlobalCardStausID,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</GlobalCardStausID>");
  if (this->GlobalCardStatusText.used != 0)
  {
    IBIS_IP_string_s *ptr = this->GlobalCardStatusText.data;
    IBIS_IP_string_s *ptr_end = ptr + this->GlobalCardStatusText.used;
    do {
      bc_array_s_append_ptr(a_trg,"<GlobalCardStatusText>");
      if (IBIS_IP_string_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</GlobalCardStatusText>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int GlobalCardStatus_s_read(GlobalCardStatus_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s GlobalCardStausID = loc_s_dict_str_get(nodes_var,"GlobalCardStausID");
  if (GlobalCardStausID == NULL ||
      IBIS_IP_unsignedInt_s_read(&this->GlobalCardStausID,GlobalCardStausID))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s GlobalCardStatusText = loc_s_dict_str_get(nodes_var,"GlobalCardStatusText");
  if (GlobalCardStatusText != NULL)
  {
    if (GlobalCardStatusText->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(GlobalCardStatusText);
      IBIS_IP_string_array_s_copy_resize(&this->GlobalCardStatusText,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          IBIS_IP_string_array_s_push_blank(&this->GlobalCardStatusText);
          if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->GlobalCardStatusText),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->GlobalCardStatusText,1);
      IBIS_IP_string_array_s_push_blank(&this->GlobalCardStatusText);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->GlobalCardStatusText),GlobalCardStatusText))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- SubscribeResponseStructure_s --
@begin
methods SubscribeResponseStructure_s
@end

int SubscribeResponseStructure_s_write(SubscribeResponseStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->Active.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<Active>");
    if (IBIS_IP_boolean_s_write(IBIS_IP_boolean_array_s_last(&this->Active),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</Active>");
  }
  if (this->OperationErrorMessage.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<OperationErrorMessage>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</OperationErrorMessage>");
  }
  return 0;
}/*}}}*/

int SubscribeResponseStructure_s_read(SubscribeResponseStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Active = loc_s_dict_str_get(nodes_var,"Active");
  if (Active != NULL)
  {
    if (Active->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(Active);
      if (array_ptr->used != 0)
      {
        IBIS_IP_boolean_array_s_copy_resize(&this->Active,1);
        IBIS_IP_boolean_array_s_push_blank(&this->Active);
        if (IBIS_IP_boolean_s_read(IBIS_IP_boolean_array_s_last(&this->Active),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_boolean_array_s_copy_resize(&this->Active,1);
      IBIS_IP_boolean_array_s_push_blank(&this->Active);
      if (IBIS_IP_boolean_s_read(IBIS_IP_boolean_array_s_last(&this->Active),Active))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s OperationErrorMessage = loc_s_dict_str_get(nodes_var,"OperationErrorMessage");
  if (OperationErrorMessage != NULL)
  {
    if (OperationErrorMessage->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(OperationErrorMessage);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
        IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
      IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),OperationErrorMessage))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- UnsubscribeResponseStructure_s --
@begin
methods UnsubscribeResponseStructure_s
@end

int UnsubscribeResponseStructure_s_write(UnsubscribeResponseStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->Active.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<Active>");
    if (IBIS_IP_boolean_s_write(IBIS_IP_boolean_array_s_last(&this->Active),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</Active>");
  }
  if (this->OperationErrorMessage.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<OperationErrorMessage>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</OperationErrorMessage>");
  }
  return 0;
}/*}}}*/

int UnsubscribeResponseStructure_s_read(UnsubscribeResponseStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Active = loc_s_dict_str_get(nodes_var,"Active");
  if (Active != NULL)
  {
    if (Active->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(Active);
      if (array_ptr->used != 0)
      {
        IBIS_IP_boolean_array_s_copy_resize(&this->Active,1);
        IBIS_IP_boolean_array_s_push_blank(&this->Active);
        if (IBIS_IP_boolean_s_read(IBIS_IP_boolean_array_s_last(&this->Active),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_boolean_array_s_copy_resize(&this->Active,1);
      IBIS_IP_boolean_array_s_push_blank(&this->Active);
      if (IBIS_IP_boolean_s_read(IBIS_IP_boolean_array_s_last(&this->Active),Active))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s OperationErrorMessage = loc_s_dict_str_get(nodes_var,"OperationErrorMessage");
  if (OperationErrorMessage != NULL)
  {
    if (OperationErrorMessage->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(OperationErrorMessage);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
        IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
      IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),OperationErrorMessage))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- TicketingService_GetValidationResultResponseStructure_s --
@begin
methods TicketingService_GetValidationResultResponseStructure_s
@end

int TicketingService_GetValidationResultResponseStructure_s_write(TicketingService_GetValidationResultResponseStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->ValidationResultData.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ValidationResultData>");
    if (TicketingService_ValidationResultDataStructure_s_write(TicketingService_ValidationResultDataStructure_array_s_last(&this->ValidationResultData),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ValidationResultData>");
  }
  if (this->OperationErrorMessage.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<OperationErrorMessage>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</OperationErrorMessage>");
  }
  return 0;
}/*}}}*/

int TicketingService_GetValidationResultResponseStructure_s_read(TicketingService_GetValidationResultResponseStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ValidationResultData = loc_s_dict_str_get(nodes_var,"ValidationResultData");
  if (ValidationResultData != NULL)
  {
    if (ValidationResultData->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ValidationResultData);
      if (array_ptr->used != 0)
      {
        TicketingService_ValidationResultDataStructure_array_s_copy_resize(&this->ValidationResultData,1);
        TicketingService_ValidationResultDataStructure_array_s_push_blank(&this->ValidationResultData);
        if (TicketingService_ValidationResultDataStructure_s_read(TicketingService_ValidationResultDataStructure_array_s_last(&this->ValidationResultData),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      TicketingService_ValidationResultDataStructure_array_s_copy_resize(&this->ValidationResultData,1);
      TicketingService_ValidationResultDataStructure_array_s_push_blank(&this->ValidationResultData);
      if (TicketingService_ValidationResultDataStructure_s_read(TicketingService_ValidationResultDataStructure_array_s_last(&this->ValidationResultData),ValidationResultData))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s OperationErrorMessage = loc_s_dict_str_get(nodes_var,"OperationErrorMessage");
  if (OperationErrorMessage != NULL)
  {
    if (OperationErrorMessage->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(OperationErrorMessage);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
        IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
      IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),OperationErrorMessage))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DoorStateStructure_s --
@begin
methods DoorStateStructure_s
@end

int DoorStateStructure_s_write(DoorStateStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<OpenState>");
  if (DoorOpenStateStructure_s_write(&this->OpenState,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</OpenState>");
  if (this->OperationState.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<OperationState>");
    if (DoorOperationStateStructure_s_write(DoorOperationStateStructure_array_s_last(&this->OperationState),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</OperationState>");
  }
  return 0;
}/*}}}*/

int DoorStateStructure_s_read(DoorStateStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s OpenState = loc_s_dict_str_get(nodes_var,"OpenState");
  if (OpenState == NULL ||
      DoorOpenStateStructure_s_read(&this->OpenState,OpenState))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s OperationState = loc_s_dict_str_get(nodes_var,"OperationState");
  if (OperationState != NULL)
  {
    if (OperationState->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(OperationState);
      if (array_ptr->used != 0)
      {
        DoorOperationStateStructure_array_s_copy_resize(&this->OperationState,1);
        DoorOperationStateStructure_array_s_push_blank(&this->OperationState);
        if (DoorOperationStateStructure_s_read(DoorOperationStateStructure_array_s_last(&this->OperationState),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      DoorOperationStateStructure_array_s_copy_resize(&this->OperationState,1);
      DoorOperationStateStructure_array_s_push_blank(&this->OperationState);
      if (DoorOperationStateStructure_s_read(DoorOperationStateStructure_array_s_last(&this->OperationState),OperationState))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- SubscribeRequestStructure_s --
@begin
methods SubscribeRequestStructure_s
@end

int SubscribeRequestStructure_s_write(SubscribeRequestStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Client-IP-Address>");
  if (IBIS_IP_string_s_write(&this->Client_IP_Address,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Client-IP-Address>");
  if (this->ReplyPort.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ReplyPort>");
    if (IBIS_IP_int_s_write(IBIS_IP_int_array_s_last(&this->ReplyPort),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ReplyPort>");
  }
  if (this->ReplyPath.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ReplyPath>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->ReplyPath),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ReplyPath>");
  }
  return 0;
}/*}}}*/

int SubscribeRequestStructure_s_read(SubscribeRequestStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Client_IP_Address = loc_s_dict_str_get(nodes_var,"Client-IP-Address");
  if (Client_IP_Address == NULL ||
      IBIS_IP_string_s_read(&this->Client_IP_Address,Client_IP_Address))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ReplyPort = loc_s_dict_str_get(nodes_var,"ReplyPort");
  if (ReplyPort != NULL)
  {
    if (ReplyPort->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ReplyPort);
      if (array_ptr->used != 0)
      {
        IBIS_IP_int_array_s_copy_resize(&this->ReplyPort,1);
        IBIS_IP_int_array_s_push_blank(&this->ReplyPort);
        if (IBIS_IP_int_s_read(IBIS_IP_int_array_s_last(&this->ReplyPort),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_int_array_s_copy_resize(&this->ReplyPort,1);
      IBIS_IP_int_array_s_push_blank(&this->ReplyPort);
      if (IBIS_IP_int_s_read(IBIS_IP_int_array_s_last(&this->ReplyPort),ReplyPort))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s ReplyPath = loc_s_dict_str_get(nodes_var,"ReplyPath");
  if (ReplyPath != NULL)
  {
    if (ReplyPath->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ReplyPath);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->ReplyPath,1);
        IBIS_IP_string_array_s_push_blank(&this->ReplyPath);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->ReplyPath),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->ReplyPath,1);
      IBIS_IP_string_array_s_push_blank(&this->ReplyPath);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->ReplyPath),ReplyPath))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- UnsubscribeRequestStructure_s --
@begin
methods UnsubscribeRequestStructure_s
@end

int UnsubscribeRequestStructure_s_write(UnsubscribeRequestStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<Client-IP-Address>");
  if (IBIS_IP_string_s_write(&this->Client_IP_Address,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Client-IP-Address>");
  if (this->ReplyPort.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ReplyPort>");
    if (IBIS_IP_int_s_write(IBIS_IP_int_array_s_last(&this->ReplyPort),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ReplyPort>");
  }
  if (this->ReplyPath.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ReplyPath>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->ReplyPath),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ReplyPath>");
  }
  return 0;
}/*}}}*/

int UnsubscribeRequestStructure_s_read(UnsubscribeRequestStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Client_IP_Address = loc_s_dict_str_get(nodes_var,"Client-IP-Address");
  if (Client_IP_Address == NULL ||
      IBIS_IP_string_s_read(&this->Client_IP_Address,Client_IP_Address))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ReplyPort = loc_s_dict_str_get(nodes_var,"ReplyPort");
  if (ReplyPort != NULL)
  {
    if (ReplyPort->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ReplyPort);
      if (array_ptr->used != 0)
      {
        IBIS_IP_int_array_s_copy_resize(&this->ReplyPort,1);
        IBIS_IP_int_array_s_push_blank(&this->ReplyPort);
        if (IBIS_IP_int_s_read(IBIS_IP_int_array_s_last(&this->ReplyPort),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_int_array_s_copy_resize(&this->ReplyPort,1);
      IBIS_IP_int_array_s_push_blank(&this->ReplyPort);
      if (IBIS_IP_int_s_read(IBIS_IP_int_array_s_last(&this->ReplyPort),ReplyPort))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s ReplyPath = loc_s_dict_str_get(nodes_var,"ReplyPath");
  if (ReplyPath != NULL)
  {
    if (ReplyPath->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ReplyPath);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->ReplyPath,1);
        IBIS_IP_string_array_s_push_blank(&this->ReplyPath);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->ReplyPath),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->ReplyPath,1);
      IBIS_IP_string_array_s_push_blank(&this->ReplyPath);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->ReplyPath),ReplyPath))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- LineInformationStructure_s --
@begin
methods LineInformationStructure_s
@end

int LineInformationStructure_s_write(LineInformationStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<LineRef>");
  if (IBIS_IP_NMTOKEN_s_write(&this->LineRef,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</LineRef>");
  if (this->LineName.used != 0)
  {
    InternationalTextType_s *ptr = this->LineName.data;
    InternationalTextType_s *ptr_end = ptr + this->LineName.used;
    do {
      bc_array_s_append_ptr(a_trg,"<LineName>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</LineName>");
    } while(++ptr < ptr_end);
  }
  if (this->LineShortName.used != 0)
  {
    InternationalTextType_s *ptr = this->LineShortName.data;
    InternationalTextType_s *ptr_end = ptr + this->LineShortName.used;
    do {
      bc_array_s_append_ptr(a_trg,"<LineShortName>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</LineShortName>");
    } while(++ptr < ptr_end);
  }
  if (this->LineNumber.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<LineNumber>");
    if (IBIS_IP_int_s_write(IBIS_IP_int_array_s_last(&this->LineNumber),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</LineNumber>");
  }
  return 0;
}/*}}}*/

int LineInformationStructure_s_read(LineInformationStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s LineRef = loc_s_dict_str_get(nodes_var,"LineRef");
  if (LineRef == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->LineRef,LineRef))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s LineName = loc_s_dict_str_get(nodes_var,"LineName");
  if (LineName != NULL)
  {
    if (LineName->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(LineName);
      InternationalTextType_array_s_copy_resize(&this->LineName,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->LineName);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->LineName),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->LineName,1);
      InternationalTextType_array_s_push_blank(&this->LineName);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->LineName),LineName))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s LineShortName = loc_s_dict_str_get(nodes_var,"LineShortName");
  if (LineShortName != NULL)
  {
    if (LineShortName->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(LineShortName);
      InternationalTextType_array_s_copy_resize(&this->LineShortName,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->LineShortName);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->LineShortName),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->LineShortName,1);
      InternationalTextType_array_s_push_blank(&this->LineShortName);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->LineShortName),LineShortName))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s LineNumber = loc_s_dict_str_get(nodes_var,"LineNumber");
  if (LineNumber != NULL)
  {
    if (LineNumber->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(LineNumber);
      if (array_ptr->used != 0)
      {
        IBIS_IP_int_array_s_copy_resize(&this->LineNumber,1);
        IBIS_IP_int_array_s_push_blank(&this->LineNumber);
        if (IBIS_IP_int_s_read(IBIS_IP_int_array_s_last(&this->LineNumber),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_int_array_s_copy_resize(&this->LineNumber,1);
      IBIS_IP_int_array_s_push_blank(&this->LineNumber);
      if (IBIS_IP_int_s_read(IBIS_IP_int_array_s_last(&this->LineNumber),LineNumber))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- CardTicketData_s --
@begin
methods CardTicketData_s
@end

int CardTicketData_s_write(CardTicketData_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<CardTicketDataID>");
  if (IBIS_IP_unsignedLong_s_write(&this->CardTicketDataID,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</CardTicketDataID>");
  bc_array_s_append_ptr(a_trg,"<CardTicketDataLength>");
  if (IBIS_IP_unsignedInt_s_write(&this->CardTicketDataLength,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</CardTicketDataLength>");
  if (this->CardTicketData.used != 0)
  {
    IBIS_IP_byte_s *ptr = this->CardTicketData.data;
    IBIS_IP_byte_s *ptr_end = ptr + this->CardTicketData.used;
    do {
      bc_array_s_append_ptr(a_trg,"<CardTicketData>");
      if (IBIS_IP_byte_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</CardTicketData>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int CardTicketData_s_read(CardTicketData_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardTicketDataID = loc_s_dict_str_get(nodes_var,"CardTicketDataID");
  if (CardTicketDataID == NULL ||
      IBIS_IP_unsignedLong_s_read(&this->CardTicketDataID,CardTicketDataID))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardTicketDataLength = loc_s_dict_str_get(nodes_var,"CardTicketDataLength");
  if (CardTicketDataLength == NULL ||
      IBIS_IP_unsignedInt_s_read(&this->CardTicketDataLength,CardTicketDataLength))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardTicketData = loc_s_dict_str_get(nodes_var,"CardTicketData");
  if (CardTicketData != NULL)
  {
    if (CardTicketData->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(CardTicketData);
      IBIS_IP_byte_array_s_copy_resize(&this->CardTicketData,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          IBIS_IP_byte_array_s_push_blank(&this->CardTicketData);
          if (IBIS_IP_byte_s_read(IBIS_IP_byte_array_s_last(&this->CardTicketData),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      IBIS_IP_byte_array_s_copy_resize(&this->CardTicketData,1);
      IBIS_IP_byte_array_s_push_blank(&this->CardTicketData);
      if (IBIS_IP_byte_s_read(IBIS_IP_byte_array_s_last(&this->CardTicketData),CardTicketData))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- GNSSPointStructure_s --
@begin
methods GNSSPointStructure_s
@end

int GNSSPointStructure_s_write(GNSSPointStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->PointRef.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<PointRef>");
    if (IBIS_IP_NMTOKEN_s_write(IBIS_IP_NMTOKEN_array_s_last(&this->PointRef),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</PointRef>");
  }
  bc_array_s_append_ptr(a_trg,"<Longitude>");
  if (GNSSCoordinateStructure_s_write(&this->Longitude,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Longitude>");
  bc_array_s_append_ptr(a_trg,"<Latitude>");
  if (GNSSCoordinateStructure_s_write(&this->Latitude,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Latitude>");
  if (this->Altitude.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<Altitude>");
    if (IBIS_IP_double_s_write(IBIS_IP_double_array_s_last(&this->Altitude),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</Altitude>");
  }
  return 0;
}/*}}}*/

int GNSSPointStructure_s_read(GNSSPointStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s PointRef = loc_s_dict_str_get(nodes_var,"PointRef");
  if (PointRef != NULL)
  {
    if (PointRef->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(PointRef);
      if (array_ptr->used != 0)
      {
        IBIS_IP_NMTOKEN_array_s_copy_resize(&this->PointRef,1);
        IBIS_IP_NMTOKEN_array_s_push_blank(&this->PointRef);
        if (IBIS_IP_NMTOKEN_s_read(IBIS_IP_NMTOKEN_array_s_last(&this->PointRef),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_NMTOKEN_array_s_copy_resize(&this->PointRef,1);
      IBIS_IP_NMTOKEN_array_s_push_blank(&this->PointRef);
      if (IBIS_IP_NMTOKEN_s_read(IBIS_IP_NMTOKEN_array_s_last(&this->PointRef),PointRef))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s Longitude = loc_s_dict_str_get(nodes_var,"Longitude");
  if (Longitude == NULL ||
      GNSSCoordinateStructure_s_read(&this->Longitude,Longitude))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Latitude = loc_s_dict_str_get(nodes_var,"Latitude");
  if (Latitude == NULL ||
      GNSSCoordinateStructure_s_read(&this->Latitude,Latitude))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Altitude = loc_s_dict_str_get(nodes_var,"Altitude");
  if (Altitude != NULL)
  {
    if (Altitude->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(Altitude);
      if (array_ptr->used != 0)
      {
        IBIS_IP_double_array_s_copy_resize(&this->Altitude,1);
        IBIS_IP_double_array_s_push_blank(&this->Altitude);
        if (IBIS_IP_double_s_read(IBIS_IP_double_array_s_last(&this->Altitude),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_double_array_s_copy_resize(&this->Altitude,1);
      IBIS_IP_double_array_s_push_blank(&this->Altitude);
      if (IBIS_IP_double_s_read(IBIS_IP_double_array_s_last(&this->Altitude),Altitude))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- ViaPointStructure_s --
@begin
methods ViaPointStructure_s
@end

int ViaPointStructure_s_write(ViaPointStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<ViaPointRef>");
  if (IBIS_IP_NMTOKEN_s_write(&this->ViaPointRef,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</ViaPointRef>");
  if (this->PlaceRef.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<PlaceRef>");
    if (IBIS_IP_NMTOKEN_s_write(IBIS_IP_NMTOKEN_array_s_last(&this->PlaceRef),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</PlaceRef>");
  }
  if (this->PlaceName.used != 0)
  {
    InternationalTextType_s *ptr = this->PlaceName.data;
    InternationalTextType_s *ptr_end = ptr + this->PlaceName.used;
    do {
      bc_array_s_append_ptr(a_trg,"<PlaceName>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</PlaceName>");
    } while(++ptr < ptr_end);
  }
  if (this->PlaceShortName.used != 0)
  {
    InternationalTextType_s *ptr = this->PlaceShortName.data;
    InternationalTextType_s *ptr_end = ptr + this->PlaceShortName.used;
    do {
      bc_array_s_append_ptr(a_trg,"<PlaceShortName>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</PlaceShortName>");
    } while(++ptr < ptr_end);
  }
  if (this->ViaPointDisplayPriority.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ViaPointDisplayPriority>");
    if (IBIS_IP_int_s_write(IBIS_IP_int_array_s_last(&this->ViaPointDisplayPriority),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ViaPointDisplayPriority>");
  }
  return 0;
}/*}}}*/

int ViaPointStructure_s_read(ViaPointStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ViaPointRef = loc_s_dict_str_get(nodes_var,"ViaPointRef");
  if (ViaPointRef == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->ViaPointRef,ViaPointRef))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s PlaceRef = loc_s_dict_str_get(nodes_var,"PlaceRef");
  if (PlaceRef != NULL)
  {
    if (PlaceRef->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(PlaceRef);
      if (array_ptr->used != 0)
      {
        IBIS_IP_NMTOKEN_array_s_copy_resize(&this->PlaceRef,1);
        IBIS_IP_NMTOKEN_array_s_push_blank(&this->PlaceRef);
        if (IBIS_IP_NMTOKEN_s_read(IBIS_IP_NMTOKEN_array_s_last(&this->PlaceRef),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_NMTOKEN_array_s_copy_resize(&this->PlaceRef,1);
      IBIS_IP_NMTOKEN_array_s_push_blank(&this->PlaceRef);
      if (IBIS_IP_NMTOKEN_s_read(IBIS_IP_NMTOKEN_array_s_last(&this->PlaceRef),PlaceRef))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s PlaceName = loc_s_dict_str_get(nodes_var,"PlaceName");
  if (PlaceName != NULL)
  {
    if (PlaceName->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(PlaceName);
      InternationalTextType_array_s_copy_resize(&this->PlaceName,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->PlaceName);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->PlaceName),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->PlaceName,1);
      InternationalTextType_array_s_push_blank(&this->PlaceName);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->PlaceName),PlaceName))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s PlaceShortName = loc_s_dict_str_get(nodes_var,"PlaceShortName");
  if (PlaceShortName != NULL)
  {
    if (PlaceShortName->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(PlaceShortName);
      InternationalTextType_array_s_copy_resize(&this->PlaceShortName,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->PlaceShortName);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->PlaceShortName),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->PlaceShortName,1);
      InternationalTextType_array_s_push_blank(&this->PlaceShortName);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->PlaceShortName),PlaceShortName))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s ViaPointDisplayPriority = loc_s_dict_str_get(nodes_var,"ViaPointDisplayPriority");
  if (ViaPointDisplayPriority != NULL)
  {
    if (ViaPointDisplayPriority->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ViaPointDisplayPriority);
      if (array_ptr->used != 0)
      {
        IBIS_IP_int_array_s_copy_resize(&this->ViaPointDisplayPriority,1);
        IBIS_IP_int_array_s_push_blank(&this->ViaPointDisplayPriority);
        if (IBIS_IP_int_s_read(IBIS_IP_int_array_s_last(&this->ViaPointDisplayPriority),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_int_array_s_copy_resize(&this->ViaPointDisplayPriority,1);
      IBIS_IP_int_array_s_push_blank(&this->ViaPointDisplayPriority);
      if (IBIS_IP_int_s_read(IBIS_IP_int_array_s_last(&this->ViaPointDisplayPriority),ViaPointDisplayPriority))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DataAcceptedResponseDataStructure_s --
@begin
methods DataAcceptedResponseDataStructure_s
@end

int DataAcceptedResponseDataStructure_s_write(DataAcceptedResponseDataStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<TimeStamp>");
  if (IBIS_IP_dateTime_s_write(&this->TimeStamp,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</TimeStamp>");
  bc_array_s_append_ptr(a_trg,"<DataAccepted>");
  if (IBIS_IP_boolean_s_write(&this->DataAccepted,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</DataAccepted>");
  if (this->ErrorCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorCode>");
    if (ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorCode>");
  }
  if (this->ErrorInformation.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorInformation>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->ErrorInformation),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorInformation>");
  }
  return 0;
}/*}}}*/

int DataAcceptedResponseDataStructure_s_read(DataAcceptedResponseDataStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s TimeStamp = loc_s_dict_str_get(nodes_var,"TimeStamp");
  if (TimeStamp == NULL ||
      IBIS_IP_dateTime_s_read(&this->TimeStamp,TimeStamp))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DataAccepted = loc_s_dict_str_get(nodes_var,"DataAccepted");
  if (DataAccepted == NULL ||
      IBIS_IP_boolean_s_read(&this->DataAccepted,DataAccepted))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ErrorCode = loc_s_dict_str_get(nodes_var,"ErrorCode");
  if (ErrorCode != NULL)
  {
    if (ErrorCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorCode);
      if (array_ptr->used != 0)
      {
        ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
        ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
        if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ErrorCodeEnumeration_array_s_copy_resize(&this->ErrorCode,1);
      ErrorCodeEnumeration_array_s_push_blank(&this->ErrorCode);
      if (ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_array_s_last(&this->ErrorCode),ErrorCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s ErrorInformation = loc_s_dict_str_get(nodes_var,"ErrorInformation");
  if (ErrorInformation != NULL)
  {
    if (ErrorInformation->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorInformation);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->ErrorInformation,1);
        IBIS_IP_string_array_s_push_blank(&this->ErrorInformation);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->ErrorInformation),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->ErrorInformation,1);
      IBIS_IP_string_array_s_push_blank(&this->ErrorInformation);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->ErrorInformation),ErrorInformation))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- ZoneType_array_s --
@begin
methods ZoneType_array_s
@end

// -- PassengerCountingService_StartCountingRequestStructure_s --
@begin
methods PassengerCountingService_StartCountingRequestStructure_s
@end

int PassengerCountingService_StartCountingRequestStructure_s_write(PassengerCountingService_StartCountingRequestStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<DoorIdList>");
  if (DoorIdListType_s_write(&this->DoorIdList,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</DoorIdList>");
  return 0;
}/*}}}*/

int PassengerCountingService_StartCountingRequestStructure_s_read(PassengerCountingService_StartCountingRequestStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DoorIdList = loc_s_dict_str_get(nodes_var,"DoorIdList");
  if (DoorIdList == NULL ||
      DoorIdListType_s_read(&this->DoorIdList,DoorIdList))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- PassengerCountingService_StopCountingRequestStructure_s --
@begin
methods PassengerCountingService_StopCountingRequestStructure_s
@end

int PassengerCountingService_StopCountingRequestStructure_s_write(PassengerCountingService_StopCountingRequestStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<DoorIdList>");
  if (DoorIdListType_s_write(&this->DoorIdList,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</DoorIdList>");
  return 0;
}/*}}}*/

int PassengerCountingService_StopCountingRequestStructure_s_read(PassengerCountingService_StopCountingRequestStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DoorIdList = loc_s_dict_str_get(nodes_var,"DoorIdList");
  if (DoorIdList == NULL ||
      DoorIdListType_s_read(&this->DoorIdList,DoorIdList))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- BayAreaStructure_array_s --
@begin
methods BayAreaStructure_array_s
@end

// -- VehicleStructure_array_s --
@begin
methods VehicleStructure_array_s
@end

// -- ViaPointStructure_array_s --
@begin
methods ViaPointStructure_array_s
@end

// -- DoorStateStructure_array_s --
@begin
methods DoorStateStructure_array_s
@end

// -- GNSSPointStructure_array_s --
@begin
methods GNSSPointStructure_array_s
@end

// -- CardApplInformations_array_s --
@begin
methods CardApplInformations_array_s
@end

// -- AnnouncementStructure_array_s --
@begin
methods AnnouncementStructure_array_s
@end

// -- CountingStateStructure_array_s --
@begin
methods CountingStateStructure_array_s
@end

// -- DoorCountingListStructure_array_s --
@begin
methods DoorCountingListStructure_array_s
@end

// -- PassengerCountingService_SetCounterDataRequestStructure_s --
@begin
methods PassengerCountingService_SetCounterDataRequestStructure_s
@end

int PassengerCountingService_SetCounterDataRequestStructure_s_write(PassengerCountingService_SetCounterDataRequestStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->DoorSetList.used != 0)
  {
    DoorCountingListStructure_s *ptr = this->DoorSetList.data;
    DoorCountingListStructure_s *ptr_end = ptr + this->DoorSetList.used;
    do {
      bc_array_s_append_ptr(a_trg,"<DoorSetList>");
      if (DoorCountingListStructure_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</DoorSetList>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int PassengerCountingService_SetCounterDataRequestStructure_s_read(PassengerCountingService_SetCounterDataRequestStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DoorSetList = loc_s_dict_str_get(nodes_var,"DoorSetList");
  if (DoorSetList != NULL)
  {
    if (DoorSetList->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(DoorSetList);
      DoorCountingListStructure_array_s_copy_resize(&this->DoorSetList,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          DoorCountingListStructure_array_s_push_blank(&this->DoorSetList);
          if (DoorCountingListStructure_s_read(DoorCountingListStructure_array_s_last(&this->DoorSetList),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      DoorCountingListStructure_array_s_copy_resize(&this->DoorSetList,1);
      DoorCountingListStructure_array_s_push_blank(&this->DoorSetList);
      if (DoorCountingListStructure_s_read(DoorCountingListStructure_array_s_last(&this->DoorSetList),DoorSetList))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DataAcceptedResponseDataStructure_array_s --
@begin
methods DataAcceptedResponseDataStructure_array_s
@end

// -- PassengerCountingService_CountingStatesStructure_s --
@begin
methods PassengerCountingService_CountingStatesStructure_s
@end

int PassengerCountingService_CountingStatesStructure_s_write(PassengerCountingService_CountingStatesStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<TimeStamp>");
  if (IBIS_IP_dateTime_s_write(&this->TimeStamp,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</TimeStamp>");
  if (this->CountingStates.used != 0)
  {
    CountingStateStructure_s *ptr = this->CountingStates.data;
    CountingStateStructure_s *ptr_end = ptr + this->CountingStates.used;
    do {
      bc_array_s_append_ptr(a_trg,"<CountingStates>");
      if (CountingStateStructure_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</CountingStates>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int PassengerCountingService_CountingStatesStructure_s_read(PassengerCountingService_CountingStatesStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s TimeStamp = loc_s_dict_str_get(nodes_var,"TimeStamp");
  if (TimeStamp == NULL ||
      IBIS_IP_dateTime_s_read(&this->TimeStamp,TimeStamp))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CountingStates = loc_s_dict_str_get(nodes_var,"CountingStates");
  if (CountingStates != NULL)
  {
    if (CountingStates->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(CountingStates);
      CountingStateStructure_array_s_copy_resize(&this->CountingStates,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          CountingStateStructure_array_s_push_blank(&this->CountingStates);
          if (CountingStateStructure_s_read(CountingStateStructure_array_s_last(&this->CountingStates),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      CountingStateStructure_array_s_copy_resize(&this->CountingStates,1);
      CountingStateStructure_array_s_push_blank(&this->CountingStates);
      if (CountingStateStructure_s_read(CountingStateStructure_array_s_last(&this->CountingStates),CountingStates))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DataAcceptedResponseStructure_s --
@begin
methods DataAcceptedResponseStructure_s
@end

int DataAcceptedResponseStructure_s_write(DataAcceptedResponseStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->DataAcceptedResponseData.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<DataAcceptedResponseData>");
    if (DataAcceptedResponseDataStructure_s_write(DataAcceptedResponseDataStructure_array_s_last(&this->DataAcceptedResponseData),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</DataAcceptedResponseData>");
  }
  if (this->OperationErrorMessage.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<OperationErrorMessage>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</OperationErrorMessage>");
  }
  return 0;
}/*}}}*/

int DataAcceptedResponseStructure_s_read(DataAcceptedResponseStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DataAcceptedResponseData = loc_s_dict_str_get(nodes_var,"DataAcceptedResponseData");
  if (DataAcceptedResponseData != NULL)
  {
    if (DataAcceptedResponseData->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(DataAcceptedResponseData);
      if (array_ptr->used != 0)
      {
        DataAcceptedResponseDataStructure_array_s_copy_resize(&this->DataAcceptedResponseData,1);
        DataAcceptedResponseDataStructure_array_s_push_blank(&this->DataAcceptedResponseData);
        if (DataAcceptedResponseDataStructure_s_read(DataAcceptedResponseDataStructure_array_s_last(&this->DataAcceptedResponseData),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      DataAcceptedResponseDataStructure_array_s_copy_resize(&this->DataAcceptedResponseData,1);
      DataAcceptedResponseDataStructure_array_s_push_blank(&this->DataAcceptedResponseData);
      if (DataAcceptedResponseDataStructure_s_read(DataAcceptedResponseDataStructure_array_s_last(&this->DataAcceptedResponseData),DataAcceptedResponseData))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s OperationErrorMessage = loc_s_dict_str_get(nodes_var,"OperationErrorMessage");
  if (OperationErrorMessage != NULL)
  {
    if (OperationErrorMessage->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(OperationErrorMessage);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
        IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
      IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),OperationErrorMessage))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- FareZoneInformationStructure_s --
@begin
methods FareZoneInformationStructure_s
@end

int FareZoneInformationStructure_s_write(FareZoneInformationStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<FareZoneID>");
  if (IBIS_IP_NMTOKEN_s_write(&this->FareZoneID,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</FareZoneID>");
  if (this->FareZoneType.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<FareZoneType>");
    if (ZoneType_s_write(ZoneType_array_s_last(&this->FareZoneType),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</FareZoneType>");
  }
  if (this->FareZoneLongName.used != 0)
  {
    InternationalTextType_s *ptr = this->FareZoneLongName.data;
    InternationalTextType_s *ptr_end = ptr + this->FareZoneLongName.used;
    do {
      bc_array_s_append_ptr(a_trg,"<FareZoneLongName>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</FareZoneLongName>");
    } while(++ptr < ptr_end);
  }
  if (this->FareZoneShortName.used != 0)
  {
    InternationalTextType_s *ptr = this->FareZoneShortName.data;
    InternationalTextType_s *ptr_end = ptr + this->FareZoneShortName.used;
    do {
      bc_array_s_append_ptr(a_trg,"<FareZoneShortName>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</FareZoneShortName>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int FareZoneInformationStructure_s_read(FareZoneInformationStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s FareZoneID = loc_s_dict_str_get(nodes_var,"FareZoneID");
  if (FareZoneID == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->FareZoneID,FareZoneID))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s FareZoneType = loc_s_dict_str_get(nodes_var,"FareZoneType");
  if (FareZoneType != NULL)
  {
    if (FareZoneType->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(FareZoneType);
      if (array_ptr->used != 0)
      {
        ZoneType_array_s_copy_resize(&this->FareZoneType,1);
        ZoneType_array_s_push_blank(&this->FareZoneType);
        if (ZoneType_s_read(ZoneType_array_s_last(&this->FareZoneType),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ZoneType_array_s_copy_resize(&this->FareZoneType,1);
      ZoneType_array_s_push_blank(&this->FareZoneType);
      if (ZoneType_s_read(ZoneType_array_s_last(&this->FareZoneType),FareZoneType))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s FareZoneLongName = loc_s_dict_str_get(nodes_var,"FareZoneLongName");
  if (FareZoneLongName != NULL)
  {
    if (FareZoneLongName->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(FareZoneLongName);
      InternationalTextType_array_s_copy_resize(&this->FareZoneLongName,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->FareZoneLongName);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->FareZoneLongName),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->FareZoneLongName,1);
      InternationalTextType_array_s_push_blank(&this->FareZoneLongName);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->FareZoneLongName),FareZoneLongName))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s FareZoneShortName = loc_s_dict_str_get(nodes_var,"FareZoneShortName");
  if (FareZoneShortName != NULL)
  {
    if (FareZoneShortName->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(FareZoneShortName);
      InternationalTextType_array_s_copy_resize(&this->FareZoneShortName,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->FareZoneShortName);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->FareZoneShortName),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->FareZoneShortName,1);
      InternationalTextType_array_s_push_blank(&this->FareZoneShortName);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->FareZoneShortName),FareZoneShortName))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DoorInformationStructure_s --
@begin
methods DoorInformationStructure_s
@end

int DoorInformationStructure_s_write(DoorInformationStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<DoorID>");
  if (IBIS_IP_NMTOKEN_s_write(&this->DoorID,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</DoorID>");
  if (this->Count.used != 0)
  {
    DoorCountingStructure_s *ptr = this->Count.data;
    DoorCountingStructure_s *ptr_end = ptr + this->Count.used;
    do {
      bc_array_s_append_ptr(a_trg,"<Count>");
      if (DoorCountingStructure_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</Count>");
    } while(++ptr < ptr_end);
  }
  if (this->State.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<State>");
    if (DoorStateStructure_s_write(DoorStateStructure_array_s_last(&this->State),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</State>");
  }
  return 0;
}/*}}}*/

int DoorInformationStructure_s_read(DoorInformationStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DoorID = loc_s_dict_str_get(nodes_var,"DoorID");
  if (DoorID == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->DoorID,DoorID))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Count = loc_s_dict_str_get(nodes_var,"Count");
  if (Count != NULL)
  {
    if (Count->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(Count);
      DoorCountingStructure_array_s_copy_resize(&this->Count,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          DoorCountingStructure_array_s_push_blank(&this->Count);
          if (DoorCountingStructure_s_read(DoorCountingStructure_array_s_last(&this->Count),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      DoorCountingStructure_array_s_copy_resize(&this->Count,1);
      DoorCountingStructure_array_s_push_blank(&this->Count);
      if (DoorCountingStructure_s_read(DoorCountingStructure_array_s_last(&this->Count),Count))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s State = loc_s_dict_str_get(nodes_var,"State");
  if (State != NULL)
  {
    if (State->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(State);
      if (array_ptr->used != 0)
      {
        DoorStateStructure_array_s_copy_resize(&this->State,1);
        DoorStateStructure_array_s_push_blank(&this->State);
        if (DoorStateStructure_s_read(DoorStateStructure_array_s_last(&this->State),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      DoorStateStructure_array_s_copy_resize(&this->State,1);
      DoorStateStructure_array_s_push_blank(&this->State);
      if (DoorStateStructure_s_read(DoorStateStructure_array_s_last(&this->State),State))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- TicketingService_ValidateTicketRequestStructure_s --
@begin
methods TicketingService_ValidateTicketRequestStructure_s
@end

int TicketingService_ValidateTicketRequestStructure_s_write(TicketingService_ValidateTicketRequestStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<CardType>");
  if (CardType_s_write(&this->CardType,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</CardType>");
  if (this->CardApplInformation.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<CardApplInformation>");
    if (CardApplInformations_s_write(CardApplInformations_array_s_last(&this->CardApplInformation),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</CardApplInformation>");
  }
  bc_array_s_append_ptr(a_trg,"<NumberOfCardTicketDataBlocks>");
  if (IBIS_IP_unsignedInt_s_write(&this->NumberOfCardTicketDataBlocks,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</NumberOfCardTicketDataBlocks>");
  bc_array_s_append_ptr(a_trg,"<CardTicketDataBlock>");
  if (CardTicketData_s_write(&this->CardTicketDataBlock,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</CardTicketDataBlock>");
  return 0;
}/*}}}*/

int TicketingService_ValidateTicketRequestStructure_s_read(TicketingService_ValidateTicketRequestStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardType = loc_s_dict_str_get(nodes_var,"CardType");
  if (CardType == NULL ||
      CardType_s_read(&this->CardType,CardType))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardApplInformation = loc_s_dict_str_get(nodes_var,"CardApplInformation");
  if (CardApplInformation != NULL)
  {
    if (CardApplInformation->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(CardApplInformation);
      if (array_ptr->used != 0)
      {
        CardApplInformations_array_s_copy_resize(&this->CardApplInformation,1);
        CardApplInformations_array_s_push_blank(&this->CardApplInformation);
        if (CardApplInformations_s_read(CardApplInformations_array_s_last(&this->CardApplInformation),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      CardApplInformations_array_s_copy_resize(&this->CardApplInformation,1);
      CardApplInformations_array_s_push_blank(&this->CardApplInformation);
      if (CardApplInformations_s_read(CardApplInformations_array_s_last(&this->CardApplInformation),CardApplInformation))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s NumberOfCardTicketDataBlocks = loc_s_dict_str_get(nodes_var,"NumberOfCardTicketDataBlocks");
  if (NumberOfCardTicketDataBlocks == NULL ||
      IBIS_IP_unsignedInt_s_read(&this->NumberOfCardTicketDataBlocks,NumberOfCardTicketDataBlocks))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardTicketDataBlock = loc_s_dict_str_get(nodes_var,"CardTicketDataBlock");
  if (CardTicketDataBlock == NULL ||
      CardTicketData_s_read(&this->CardTicketDataBlock,CardTicketDataBlock))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- TicketingService_ValidateTicketResponseDataStructure_s --
@begin
methods TicketingService_ValidateTicketResponseDataStructure_s
@end

int TicketingService_ValidateTicketResponseDataStructure_s_write(TicketingService_ValidateTicketResponseDataStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<TimeStamp>");
  if (IBIS_IP_dateTime_s_write(&this->TimeStamp,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</TimeStamp>");
  bc_array_s_append_ptr(a_trg,"<GlobalCardStatus>");
  if (GlobalCardStatus_s_write(&this->GlobalCardStatus,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</GlobalCardStatus>");
  bc_array_s_append_ptr(a_trg,"<CardType>");
  if (CardType_s_write(&this->CardType,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</CardType>");
  if (this->CardApplStatusCode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<CardApplStatusCode>");
    if (IBIS_IP_unsignedInt_s_write(IBIS_IP_unsignedInt_array_s_last(&this->CardApplStatusCode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</CardApplStatusCode>");
  }
  if (this->CardApplikationInformation.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<CardApplikationInformation>");
    if (CardApplInformations_s_write(CardApplInformations_array_s_last(&this->CardApplikationInformation),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</CardApplikationInformation>");
  }
  bc_array_s_append_ptr(a_trg,"<CardValidationCode>");
  if (IBIS_IP_unsignedInt_s_write(&this->CardValidationCode,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</CardValidationCode>");
  bc_array_s_append_ptr(a_trg,"<CardTicketData>");
  if (CardTicketData_s_write(&this->CardTicketData,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</CardTicketData>");
  return 0;
}/*}}}*/

int TicketingService_ValidateTicketResponseDataStructure_s_read(TicketingService_ValidateTicketResponseDataStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s TimeStamp = loc_s_dict_str_get(nodes_var,"TimeStamp");
  if (TimeStamp == NULL ||
      IBIS_IP_dateTime_s_read(&this->TimeStamp,TimeStamp))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s GlobalCardStatus = loc_s_dict_str_get(nodes_var,"GlobalCardStatus");
  if (GlobalCardStatus == NULL ||
      GlobalCardStatus_s_read(&this->GlobalCardStatus,GlobalCardStatus))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardType = loc_s_dict_str_get(nodes_var,"CardType");
  if (CardType == NULL ||
      CardType_s_read(&this->CardType,CardType))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardApplStatusCode = loc_s_dict_str_get(nodes_var,"CardApplStatusCode");
  if (CardApplStatusCode != NULL)
  {
    if (CardApplStatusCode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(CardApplStatusCode);
      if (array_ptr->used != 0)
      {
        IBIS_IP_unsignedInt_array_s_copy_resize(&this->CardApplStatusCode,1);
        IBIS_IP_unsignedInt_array_s_push_blank(&this->CardApplStatusCode);
        if (IBIS_IP_unsignedInt_s_read(IBIS_IP_unsignedInt_array_s_last(&this->CardApplStatusCode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_unsignedInt_array_s_copy_resize(&this->CardApplStatusCode,1);
      IBIS_IP_unsignedInt_array_s_push_blank(&this->CardApplStatusCode);
      if (IBIS_IP_unsignedInt_s_read(IBIS_IP_unsignedInt_array_s_last(&this->CardApplStatusCode),CardApplStatusCode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s CardApplikationInformation = loc_s_dict_str_get(nodes_var,"CardApplikationInformation");
  if (CardApplikationInformation != NULL)
  {
    if (CardApplikationInformation->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(CardApplikationInformation);
      if (array_ptr->used != 0)
      {
        CardApplInformations_array_s_copy_resize(&this->CardApplikationInformation,1);
        CardApplInformations_array_s_push_blank(&this->CardApplikationInformation);
        if (CardApplInformations_s_read(CardApplInformations_array_s_last(&this->CardApplikationInformation),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      CardApplInformations_array_s_copy_resize(&this->CardApplikationInformation,1);
      CardApplInformations_array_s_push_blank(&this->CardApplikationInformation);
      if (CardApplInformations_s_read(CardApplInformations_array_s_last(&this->CardApplikationInformation),CardApplikationInformation))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s CardValidationCode = loc_s_dict_str_get(nodes_var,"CardValidationCode");
  if (CardValidationCode == NULL ||
      IBIS_IP_unsignedInt_s_read(&this->CardValidationCode,CardValidationCode))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CardTicketData = loc_s_dict_str_get(nodes_var,"CardTicketData");
  if (CardTicketData == NULL ||
      CardTicketData_s_read(&this->CardTicketData,CardTicketData))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- DisplayContentStructure_s --
@begin
methods DisplayContentStructure_s
@end

int DisplayContentStructure_s_write(DisplayContentStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->DisplayContentRef.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<DisplayContentRef>");
    if (IBIS_IP_NMTOKEN_s_write(IBIS_IP_NMTOKEN_array_s_last(&this->DisplayContentRef),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</DisplayContentRef>");
  }
  bc_array_s_append_ptr(a_trg,"<LineInformation>");
  if (LineInformationStructure_s_write(&this->LineInformation,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</LineInformation>");
  bc_array_s_append_ptr(a_trg,"<Destination>");
  if (DestinationStructure_s_write(&this->Destination,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Destination>");
  if (this->ViaPoint.used != 0)
  {
    ViaPointStructure_s *ptr = this->ViaPoint.data;
    ViaPointStructure_s *ptr_end = ptr + this->ViaPoint.used;
    do {
      bc_array_s_append_ptr(a_trg,"<ViaPoint>");
      if (ViaPointStructure_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</ViaPoint>");
    } while(++ptr < ptr_end);
  }
  if (this->AdditionalInformation.used != 0)
  {
    InternationalTextType_s *ptr = this->AdditionalInformation.data;
    InternationalTextType_s *ptr_end = ptr + this->AdditionalInformation.used;
    do {
      bc_array_s_append_ptr(a_trg,"<AdditionalInformation>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</AdditionalInformation>");
    } while(++ptr < ptr_end);
  }
  if (this->Priority.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<Priority>");
    if (IBIS_IP_nonNegativeInteger_s_write(IBIS_IP_nonNegativeInteger_array_s_last(&this->Priority),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</Priority>");
  }
  if (this->PeriodDuration.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<PeriodDuration>");
    if (IBIS_IP_duration_s_write(IBIS_IP_duration_array_s_last(&this->PeriodDuration),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</PeriodDuration>");
  }
  if (this->Duration.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<Duration>");
    if (IBIS_IP_duration_s_write(IBIS_IP_duration_array_s_last(&this->Duration),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</Duration>");
  }
  return 0;
}/*}}}*/

int DisplayContentStructure_s_read(DisplayContentStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DisplayContentRef = loc_s_dict_str_get(nodes_var,"DisplayContentRef");
  if (DisplayContentRef != NULL)
  {
    if (DisplayContentRef->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(DisplayContentRef);
      if (array_ptr->used != 0)
      {
        IBIS_IP_NMTOKEN_array_s_copy_resize(&this->DisplayContentRef,1);
        IBIS_IP_NMTOKEN_array_s_push_blank(&this->DisplayContentRef);
        if (IBIS_IP_NMTOKEN_s_read(IBIS_IP_NMTOKEN_array_s_last(&this->DisplayContentRef),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_NMTOKEN_array_s_copy_resize(&this->DisplayContentRef,1);
      IBIS_IP_NMTOKEN_array_s_push_blank(&this->DisplayContentRef);
      if (IBIS_IP_NMTOKEN_s_read(IBIS_IP_NMTOKEN_array_s_last(&this->DisplayContentRef),DisplayContentRef))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s LineInformation = loc_s_dict_str_get(nodes_var,"LineInformation");
  if (LineInformation == NULL ||
      LineInformationStructure_s_read(&this->LineInformation,LineInformation))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Destination = loc_s_dict_str_get(nodes_var,"Destination");
  if (Destination == NULL ||
      DestinationStructure_s_read(&this->Destination,Destination))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ViaPoint = loc_s_dict_str_get(nodes_var,"ViaPoint");
  if (ViaPoint != NULL)
  {
    if (ViaPoint->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ViaPoint);
      ViaPointStructure_array_s_copy_resize(&this->ViaPoint,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          ViaPointStructure_array_s_push_blank(&this->ViaPoint);
          if (ViaPointStructure_s_read(ViaPointStructure_array_s_last(&this->ViaPoint),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      ViaPointStructure_array_s_copy_resize(&this->ViaPoint,1);
      ViaPointStructure_array_s_push_blank(&this->ViaPoint);
      if (ViaPointStructure_s_read(ViaPointStructure_array_s_last(&this->ViaPoint),ViaPoint))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s AdditionalInformation = loc_s_dict_str_get(nodes_var,"AdditionalInformation");
  if (AdditionalInformation != NULL)
  {
    if (AdditionalInformation->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(AdditionalInformation);
      InternationalTextType_array_s_copy_resize(&this->AdditionalInformation,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->AdditionalInformation);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->AdditionalInformation),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->AdditionalInformation,1);
      InternationalTextType_array_s_push_blank(&this->AdditionalInformation);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->AdditionalInformation),AdditionalInformation))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s Priority = loc_s_dict_str_get(nodes_var,"Priority");
  if (Priority != NULL)
  {
    if (Priority->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(Priority);
      if (array_ptr->used != 0)
      {
        IBIS_IP_nonNegativeInteger_array_s_copy_resize(&this->Priority,1);
        IBIS_IP_nonNegativeInteger_array_s_push_blank(&this->Priority);
        if (IBIS_IP_nonNegativeInteger_s_read(IBIS_IP_nonNegativeInteger_array_s_last(&this->Priority),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_nonNegativeInteger_array_s_copy_resize(&this->Priority,1);
      IBIS_IP_nonNegativeInteger_array_s_push_blank(&this->Priority);
      if (IBIS_IP_nonNegativeInteger_s_read(IBIS_IP_nonNegativeInteger_array_s_last(&this->Priority),Priority))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s PeriodDuration = loc_s_dict_str_get(nodes_var,"PeriodDuration");
  if (PeriodDuration != NULL)
  {
    if (PeriodDuration->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(PeriodDuration);
      if (array_ptr->used != 0)
      {
        IBIS_IP_duration_array_s_copy_resize(&this->PeriodDuration,1);
        IBIS_IP_duration_array_s_push_blank(&this->PeriodDuration);
        if (IBIS_IP_duration_s_read(IBIS_IP_duration_array_s_last(&this->PeriodDuration),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_duration_array_s_copy_resize(&this->PeriodDuration,1);
      IBIS_IP_duration_array_s_push_blank(&this->PeriodDuration);
      if (IBIS_IP_duration_s_read(IBIS_IP_duration_array_s_last(&this->PeriodDuration),PeriodDuration))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s Duration = loc_s_dict_str_get(nodes_var,"Duration");
  if (Duration != NULL)
  {
    if (Duration->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(Duration);
      if (array_ptr->used != 0)
      {
        IBIS_IP_duration_array_s_copy_resize(&this->Duration,1);
        IBIS_IP_duration_array_s_push_blank(&this->Duration);
        if (IBIS_IP_duration_s_read(IBIS_IP_duration_array_s_last(&this->Duration),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_duration_array_s_copy_resize(&this->Duration,1);
      IBIS_IP_duration_array_s_push_blank(&this->Duration);
      if (IBIS_IP_duration_s_read(IBIS_IP_duration_array_s_last(&this->Duration),Duration))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- DisplayContentStructure_array_s --
@begin
methods DisplayContentStructure_array_s
@end

// -- DoorInformationStructure_array_s --
@begin
methods DoorInformationStructure_array_s
@end

// -- PassengerCountingService_CountingStatesStructure_array_s --
@begin
methods PassengerCountingService_CountingStatesStructure_array_s
@end

// -- TicketingService_ValidateTicketResponseDataStructure_array_s --
@begin
methods TicketingService_ValidateTicketResponseDataStructure_array_s
@end

// -- PassengerCountingService_SpecificDoorDataStructure_s --
@begin
methods PassengerCountingService_SpecificDoorDataStructure_s
@end

int PassengerCountingService_SpecificDoorDataStructure_s_write(PassengerCountingService_SpecificDoorDataStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<TimeStamp>");
  if (IBIS_IP_dateTime_s_write(&this->TimeStamp,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</TimeStamp>");
  bc_array_s_append_ptr(a_trg,"<CountingData>");
  if (DoorInformationStructure_s_write(&this->CountingData,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</CountingData>");
  return 0;
}/*}}}*/

int PassengerCountingService_SpecificDoorDataStructure_s_read(PassengerCountingService_SpecificDoorDataStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s TimeStamp = loc_s_dict_str_get(nodes_var,"TimeStamp");
  if (TimeStamp == NULL ||
      IBIS_IP_dateTime_s_read(&this->TimeStamp,TimeStamp))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CountingData = loc_s_dict_str_get(nodes_var,"CountingData");
  if (CountingData == NULL ||
      DoorInformationStructure_s_read(&this->CountingData,CountingData))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- PassengerCountingService_AllDataStructure_s --
@begin
methods PassengerCountingService_AllDataStructure_s
@end

int PassengerCountingService_AllDataStructure_s_write(PassengerCountingService_AllDataStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<TimeStamp>");
  if (IBIS_IP_dateTime_s_write(&this->TimeStamp,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</TimeStamp>");
  if (this->CountingData.used != 0)
  {
    DoorInformationStructure_s *ptr = this->CountingData.data;
    DoorInformationStructure_s *ptr_end = ptr + this->CountingData.used;
    do {
      bc_array_s_append_ptr(a_trg,"<CountingData>");
      if (DoorInformationStructure_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</CountingData>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int PassengerCountingService_AllDataStructure_s_read(PassengerCountingService_AllDataStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s TimeStamp = loc_s_dict_str_get(nodes_var,"TimeStamp");
  if (TimeStamp == NULL ||
      IBIS_IP_dateTime_s_read(&this->TimeStamp,TimeStamp))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s CountingData = loc_s_dict_str_get(nodes_var,"CountingData");
  if (CountingData != NULL)
  {
    if (CountingData->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(CountingData);
      DoorInformationStructure_array_s_copy_resize(&this->CountingData,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          DoorInformationStructure_array_s_push_blank(&this->CountingData);
          if (DoorInformationStructure_s_read(DoorInformationStructure_array_s_last(&this->CountingData),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      DoorInformationStructure_array_s_copy_resize(&this->CountingData,1);
      DoorInformationStructure_array_s_push_blank(&this->CountingData);
      if (DoorInformationStructure_s_read(DoorInformationStructure_array_s_last(&this->CountingData),CountingData))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- PassengerCountingService_GetCountingStateResponseStructure_s --
@begin
methods PassengerCountingService_GetCountingStateResponseStructure_s
@end

int PassengerCountingService_GetCountingStateResponseStructure_s_write(PassengerCountingService_GetCountingStateResponseStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->Data.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<Data>");
    if (PassengerCountingService_CountingStatesStructure_s_write(PassengerCountingService_CountingStatesStructure_array_s_last(&this->Data),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</Data>");
  }
  if (this->ErrorMessage.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ErrorMessage>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->ErrorMessage),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ErrorMessage>");
  }
  return 0;
}/*}}}*/

int PassengerCountingService_GetCountingStateResponseStructure_s_read(PassengerCountingService_GetCountingStateResponseStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Data = loc_s_dict_str_get(nodes_var,"Data");
  if (Data != NULL)
  {
    if (Data->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(Data);
      if (array_ptr->used != 0)
      {
        PassengerCountingService_CountingStatesStructure_array_s_copy_resize(&this->Data,1);
        PassengerCountingService_CountingStatesStructure_array_s_push_blank(&this->Data);
        if (PassengerCountingService_CountingStatesStructure_s_read(PassengerCountingService_CountingStatesStructure_array_s_last(&this->Data),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      PassengerCountingService_CountingStatesStructure_array_s_copy_resize(&this->Data,1);
      PassengerCountingService_CountingStatesStructure_array_s_push_blank(&this->Data);
      if (PassengerCountingService_CountingStatesStructure_s_read(PassengerCountingService_CountingStatesStructure_array_s_last(&this->Data),Data))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s ErrorMessage = loc_s_dict_str_get(nodes_var,"ErrorMessage");
  if (ErrorMessage != NULL)
  {
    if (ErrorMessage->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ErrorMessage);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->ErrorMessage,1);
        IBIS_IP_string_array_s_push_blank(&this->ErrorMessage);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->ErrorMessage),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->ErrorMessage,1);
      IBIS_IP_string_array_s_push_blank(&this->ErrorMessage);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->ErrorMessage),ErrorMessage))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- TicketingService_ValidateTicketResponseStructure_s --
@begin
methods TicketingService_ValidateTicketResponseStructure_s
@end

int TicketingService_ValidateTicketResponseStructure_s_write(TicketingService_ValidateTicketResponseStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->TicketingService_ValidationResponseData.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<TicketingService.ValidationResponseData>");
    if (TicketingService_ValidateTicketResponseDataStructure_s_write(TicketingService_ValidateTicketResponseDataStructure_array_s_last(&this->TicketingService_ValidationResponseData),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</TicketingService.ValidationResponseData>");
  }
  if (this->OperationErrorMessage.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<OperationErrorMessage>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</OperationErrorMessage>");
  }
  return 0;
}/*}}}*/

int TicketingService_ValidateTicketResponseStructure_s_read(TicketingService_ValidateTicketResponseStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s TicketingService_ValidationResponseData = loc_s_dict_str_get(nodes_var,"TicketingService.ValidationResponseData");
  if (TicketingService_ValidationResponseData != NULL)
  {
    if (TicketingService_ValidationResponseData->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(TicketingService_ValidationResponseData);
      if (array_ptr->used != 0)
      {
        TicketingService_ValidateTicketResponseDataStructure_array_s_copy_resize(&this->TicketingService_ValidationResponseData,1);
        TicketingService_ValidateTicketResponseDataStructure_array_s_push_blank(&this->TicketingService_ValidationResponseData);
        if (TicketingService_ValidateTicketResponseDataStructure_s_read(TicketingService_ValidateTicketResponseDataStructure_array_s_last(&this->TicketingService_ValidationResponseData),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      TicketingService_ValidateTicketResponseDataStructure_array_s_copy_resize(&this->TicketingService_ValidationResponseData,1);
      TicketingService_ValidateTicketResponseDataStructure_array_s_push_blank(&this->TicketingService_ValidationResponseData);
      if (TicketingService_ValidateTicketResponseDataStructure_s_read(TicketingService_ValidateTicketResponseDataStructure_array_s_last(&this->TicketingService_ValidationResponseData),TicketingService_ValidationResponseData))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s OperationErrorMessage = loc_s_dict_str_get(nodes_var,"OperationErrorMessage");
  if (OperationErrorMessage != NULL)
  {
    if (OperationErrorMessage->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(OperationErrorMessage);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
        IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
      IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),OperationErrorMessage))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- ConnectionStructure_s --
@begin
methods ConnectionStructure_s
@end

int ConnectionStructure_s_write(ConnectionStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<StopRef>");
  if (IBIS_IP_NMTOKEN_s_write(&this->StopRef,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</StopRef>");
  bc_array_s_append_ptr(a_trg,"<ConnectionRef>");
  if (IBIS_IP_NMTOKEN_s_write(&this->ConnectionRef,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</ConnectionRef>");
  bc_array_s_append_ptr(a_trg,"<ConnectionType>");
  if (ConnectionTypeEnumeration_s_write(&this->ConnectionType,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</ConnectionType>");
  bc_array_s_append_ptr(a_trg,"<DisplayContent>");
  if (DisplayContentStructure_s_write(&this->DisplayContent,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</DisplayContent>");
  if (this->Platform.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<Platform>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->Platform),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</Platform>");
  }
  if (this->ConnectionState.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ConnectionState>");
    if (ConnectionStateEnumeration_s_write(ConnectionStateEnumeration_array_s_last(&this->ConnectionState),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ConnectionState>");
  }
  if (this->TransportMode.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<TransportMode>");
    if (VehicleStructure_s_write(VehicleStructure_array_s_last(&this->TransportMode),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</TransportMode>");
  }
  if (this->ExpectedDepatureTime.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ExpectedDepatureTime>");
    if (IBIS_IP_dateTime_s_write(IBIS_IP_dateTime_array_s_last(&this->ExpectedDepatureTime),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ExpectedDepatureTime>");
  }
  return 0;
}/*}}}*/

int ConnectionStructure_s_read(ConnectionStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s StopRef = loc_s_dict_str_get(nodes_var,"StopRef");
  if (StopRef == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->StopRef,StopRef))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ConnectionRef = loc_s_dict_str_get(nodes_var,"ConnectionRef");
  if (ConnectionRef == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->ConnectionRef,ConnectionRef))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ConnectionType = loc_s_dict_str_get(nodes_var,"ConnectionType");
  if (ConnectionType == NULL ||
      ConnectionTypeEnumeration_s_read(&this->ConnectionType,ConnectionType))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DisplayContent = loc_s_dict_str_get(nodes_var,"DisplayContent");
  if (DisplayContent == NULL ||
      DisplayContentStructure_s_read(&this->DisplayContent,DisplayContent))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Platform = loc_s_dict_str_get(nodes_var,"Platform");
  if (Platform != NULL)
  {
    if (Platform->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(Platform);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->Platform,1);
        IBIS_IP_string_array_s_push_blank(&this->Platform);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->Platform),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->Platform,1);
      IBIS_IP_string_array_s_push_blank(&this->Platform);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->Platform),Platform))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s ConnectionState = loc_s_dict_str_get(nodes_var,"ConnectionState");
  if (ConnectionState != NULL)
  {
    if (ConnectionState->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ConnectionState);
      if (array_ptr->used != 0)
      {
        ConnectionStateEnumeration_array_s_copy_resize(&this->ConnectionState,1);
        ConnectionStateEnumeration_array_s_push_blank(&this->ConnectionState);
        if (ConnectionStateEnumeration_s_read(ConnectionStateEnumeration_array_s_last(&this->ConnectionState),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      ConnectionStateEnumeration_array_s_copy_resize(&this->ConnectionState,1);
      ConnectionStateEnumeration_array_s_push_blank(&this->ConnectionState);
      if (ConnectionStateEnumeration_s_read(ConnectionStateEnumeration_array_s_last(&this->ConnectionState),ConnectionState))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s TransportMode = loc_s_dict_str_get(nodes_var,"TransportMode");
  if (TransportMode != NULL)
  {
    if (TransportMode->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(TransportMode);
      if (array_ptr->used != 0)
      {
        VehicleStructure_array_s_copy_resize(&this->TransportMode,1);
        VehicleStructure_array_s_push_blank(&this->TransportMode);
        if (VehicleStructure_s_read(VehicleStructure_array_s_last(&this->TransportMode),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      VehicleStructure_array_s_copy_resize(&this->TransportMode,1);
      VehicleStructure_array_s_push_blank(&this->TransportMode);
      if (VehicleStructure_s_read(VehicleStructure_array_s_last(&this->TransportMode),TransportMode))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s ExpectedDepatureTime = loc_s_dict_str_get(nodes_var,"ExpectedDepatureTime");
  if (ExpectedDepatureTime != NULL)
  {
    if (ExpectedDepatureTime->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ExpectedDepatureTime);
      if (array_ptr->used != 0)
      {
        IBIS_IP_dateTime_array_s_copy_resize(&this->ExpectedDepatureTime,1);
        IBIS_IP_dateTime_array_s_push_blank(&this->ExpectedDepatureTime);
        if (IBIS_IP_dateTime_s_read(IBIS_IP_dateTime_array_s_last(&this->ExpectedDepatureTime),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_dateTime_array_s_copy_resize(&this->ExpectedDepatureTime,1);
      IBIS_IP_dateTime_array_s_push_blank(&this->ExpectedDepatureTime);
      if (IBIS_IP_dateTime_s_read(IBIS_IP_dateTime_array_s_last(&this->ExpectedDepatureTime),ExpectedDepatureTime))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- ConnectionStructure_array_s --
@begin
methods ConnectionStructure_array_s
@end

// -- PassengerCountingService_AllDataStructure_array_s --
@begin
methods PassengerCountingService_AllDataStructure_array_s
@end

// -- PassengerCountingService_SpecificDoorDataStructure_array_s --
@begin
methods PassengerCountingService_SpecificDoorDataStructure_array_s
@end

// -- PassengerCountingService_GetAllDataResponseStructure_s --
@begin
methods PassengerCountingService_GetAllDataResponseStructure_s
@end

int PassengerCountingService_GetAllDataResponseStructure_s_write(PassengerCountingService_GetAllDataResponseStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->AllData.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<AllData>");
    if (PassengerCountingService_AllDataStructure_s_write(PassengerCountingService_AllDataStructure_array_s_last(&this->AllData),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</AllData>");
  }
  if (this->OperationErrorMessage.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<OperationErrorMessage>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</OperationErrorMessage>");
  }
  return 0;
}/*}}}*/

int PassengerCountingService_GetAllDataResponseStructure_s_read(PassengerCountingService_GetAllDataResponseStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s AllData = loc_s_dict_str_get(nodes_var,"AllData");
  if (AllData != NULL)
  {
    if (AllData->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(AllData);
      if (array_ptr->used != 0)
      {
        PassengerCountingService_AllDataStructure_array_s_copy_resize(&this->AllData,1);
        PassengerCountingService_AllDataStructure_array_s_push_blank(&this->AllData);
        if (PassengerCountingService_AllDataStructure_s_read(PassengerCountingService_AllDataStructure_array_s_last(&this->AllData),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      PassengerCountingService_AllDataStructure_array_s_copy_resize(&this->AllData,1);
      PassengerCountingService_AllDataStructure_array_s_push_blank(&this->AllData);
      if (PassengerCountingService_AllDataStructure_s_read(PassengerCountingService_AllDataStructure_array_s_last(&this->AllData),AllData))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s OperationErrorMessage = loc_s_dict_str_get(nodes_var,"OperationErrorMessage");
  if (OperationErrorMessage != NULL)
  {
    if (OperationErrorMessage->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(OperationErrorMessage);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
        IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
      IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),OperationErrorMessage))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s --
@begin
methods PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s
@end

int PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s_write(PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->SpecificDoorData.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<SpecificDoorData>");
    if (PassengerCountingService_SpecificDoorDataStructure_s_write(PassengerCountingService_SpecificDoorDataStructure_array_s_last(&this->SpecificDoorData),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</SpecificDoorData>");
  }
  if (this->OperationErrorMessage.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<OperationErrorMessage>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</OperationErrorMessage>");
  }
  return 0;
}/*}}}*/

int PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s_read(PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s SpecificDoorData = loc_s_dict_str_get(nodes_var,"SpecificDoorData");
  if (SpecificDoorData != NULL)
  {
    if (SpecificDoorData->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(SpecificDoorData);
      if (array_ptr->used != 0)
      {
        PassengerCountingService_SpecificDoorDataStructure_array_s_copy_resize(&this->SpecificDoorData,1);
        PassengerCountingService_SpecificDoorDataStructure_array_s_push_blank(&this->SpecificDoorData);
        if (PassengerCountingService_SpecificDoorDataStructure_s_read(PassengerCountingService_SpecificDoorDataStructure_array_s_last(&this->SpecificDoorData),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      PassengerCountingService_SpecificDoorDataStructure_array_s_copy_resize(&this->SpecificDoorData,1);
      PassengerCountingService_SpecificDoorDataStructure_array_s_push_blank(&this->SpecificDoorData);
      if (PassengerCountingService_SpecificDoorDataStructure_s_read(PassengerCountingService_SpecificDoorDataStructure_array_s_last(&this->SpecificDoorData),SpecificDoorData))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s OperationErrorMessage = loc_s_dict_str_get(nodes_var,"OperationErrorMessage");
  if (OperationErrorMessage != NULL)
  {
    if (OperationErrorMessage->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(OperationErrorMessage);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
        IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
      IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),OperationErrorMessage))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- JourneyStopInformationStructure_s --
@begin
methods JourneyStopInformationStructure_s
@end

int JourneyStopInformationStructure_s_write(JourneyStopInformationStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<StopRef>");
  if (IBIS_IP_NMTOKEN_s_write(&this->StopRef,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</StopRef>");
  if (this->StopName.used != 0)
  {
    InternationalTextType_s *ptr = this->StopName.data;
    InternationalTextType_s *ptr_end = ptr + this->StopName.used;
    do {
      bc_array_s_append_ptr(a_trg,"<StopName>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</StopName>");
    } while(++ptr < ptr_end);
  }
  if (this->StopAlternativeName.used != 0)
  {
    InternationalTextType_s *ptr = this->StopAlternativeName.data;
    InternationalTextType_s *ptr_end = ptr + this->StopAlternativeName.used;
    do {
      bc_array_s_append_ptr(a_trg,"<StopAlternativeName>");
      if (InternationalTextType_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</StopAlternativeName>");
    } while(++ptr < ptr_end);
  }
  if (this->Platform.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<Platform>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->Platform),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</Platform>");
  }
  if (this->DisplayContent.used != 0)
  {
    DisplayContentStructure_s *ptr = this->DisplayContent.data;
    DisplayContentStructure_s *ptr_end = ptr + this->DisplayContent.used;
    do {
      bc_array_s_append_ptr(a_trg,"<DisplayContent>");
      if (DisplayContentStructure_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</DisplayContent>");
    } while(++ptr < ptr_end);
  }
  if (this->Announcement.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<Announcement>");
    if (AnnouncementStructure_s_write(AnnouncementStructure_array_s_last(&this->Announcement),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</Announcement>");
  }
  if (this->ArrivalScheduled.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<ArrivalScheduled>");
    if (IBIS_IP_dateTime_s_write(IBIS_IP_dateTime_array_s_last(&this->ArrivalScheduled),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</ArrivalScheduled>");
  }
  if (this->DepartureScheduled.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<DepartureScheduled>");
    if (IBIS_IP_dateTime_s_write(IBIS_IP_dateTime_array_s_last(&this->DepartureScheduled),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</DepartureScheduled>");
  }
  if (this->Connection.used != 0)
  {
    ConnectionStructure_s *ptr = this->Connection.data;
    ConnectionStructure_s *ptr_end = ptr + this->Connection.used;
    do {
      bc_array_s_append_ptr(a_trg,"<Connection>");
      if (ConnectionStructure_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</Connection>");
    } while(++ptr < ptr_end);
  }
  if (this->BayArea.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<BayArea>");
    if (BayAreaStructure_s_write(BayAreaStructure_array_s_last(&this->BayArea),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</BayArea>");
  }
  if (this->GNSSPoint.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<GNSSPoint>");
    if (GNSSPointStructure_s_write(GNSSPointStructure_array_s_last(&this->GNSSPoint),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</GNSSPoint>");
  }
  if (this->FareZone.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<FareZone>");
    if (IBIS_IP_NMTOKEN_s_write(IBIS_IP_NMTOKEN_array_s_last(&this->FareZone),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</FareZone>");
  }
  return 0;
}/*}}}*/

int JourneyStopInformationStructure_s_read(JourneyStopInformationStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s StopRef = loc_s_dict_str_get(nodes_var,"StopRef");
  if (StopRef == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->StopRef,StopRef))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s StopName = loc_s_dict_str_get(nodes_var,"StopName");
  if (StopName != NULL)
  {
    if (StopName->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(StopName);
      InternationalTextType_array_s_copy_resize(&this->StopName,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->StopName);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->StopName),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->StopName,1);
      InternationalTextType_array_s_push_blank(&this->StopName);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->StopName),StopName))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s StopAlternativeName = loc_s_dict_str_get(nodes_var,"StopAlternativeName");
  if (StopAlternativeName != NULL)
  {
    if (StopAlternativeName->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(StopAlternativeName);
      InternationalTextType_array_s_copy_resize(&this->StopAlternativeName,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          InternationalTextType_array_s_push_blank(&this->StopAlternativeName);
          if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->StopAlternativeName),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      InternationalTextType_array_s_copy_resize(&this->StopAlternativeName,1);
      InternationalTextType_array_s_push_blank(&this->StopAlternativeName);
      if (InternationalTextType_s_read(InternationalTextType_array_s_last(&this->StopAlternativeName),StopAlternativeName))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s Platform = loc_s_dict_str_get(nodes_var,"Platform");
  if (Platform != NULL)
  {
    if (Platform->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(Platform);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->Platform,1);
        IBIS_IP_string_array_s_push_blank(&this->Platform);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->Platform),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->Platform,1);
      IBIS_IP_string_array_s_push_blank(&this->Platform);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->Platform),Platform))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s DisplayContent = loc_s_dict_str_get(nodes_var,"DisplayContent");
  if (DisplayContent != NULL)
  {
    if (DisplayContent->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(DisplayContent);
      DisplayContentStructure_array_s_copy_resize(&this->DisplayContent,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          DisplayContentStructure_array_s_push_blank(&this->DisplayContent);
          if (DisplayContentStructure_s_read(DisplayContentStructure_array_s_last(&this->DisplayContent),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      DisplayContentStructure_array_s_copy_resize(&this->DisplayContent,1);
      DisplayContentStructure_array_s_push_blank(&this->DisplayContent);
      if (DisplayContentStructure_s_read(DisplayContentStructure_array_s_last(&this->DisplayContent),DisplayContent))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s Announcement = loc_s_dict_str_get(nodes_var,"Announcement");
  if (Announcement != NULL)
  {
    if (Announcement->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(Announcement);
      if (array_ptr->used != 0)
      {
        AnnouncementStructure_array_s_copy_resize(&this->Announcement,1);
        AnnouncementStructure_array_s_push_blank(&this->Announcement);
        if (AnnouncementStructure_s_read(AnnouncementStructure_array_s_last(&this->Announcement),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      AnnouncementStructure_array_s_copy_resize(&this->Announcement,1);
      AnnouncementStructure_array_s_push_blank(&this->Announcement);
      if (AnnouncementStructure_s_read(AnnouncementStructure_array_s_last(&this->Announcement),Announcement))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s ArrivalScheduled = loc_s_dict_str_get(nodes_var,"ArrivalScheduled");
  if (ArrivalScheduled != NULL)
  {
    if (ArrivalScheduled->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ArrivalScheduled);
      if (array_ptr->used != 0)
      {
        IBIS_IP_dateTime_array_s_copy_resize(&this->ArrivalScheduled,1);
        IBIS_IP_dateTime_array_s_push_blank(&this->ArrivalScheduled);
        if (IBIS_IP_dateTime_s_read(IBIS_IP_dateTime_array_s_last(&this->ArrivalScheduled),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_dateTime_array_s_copy_resize(&this->ArrivalScheduled,1);
      IBIS_IP_dateTime_array_s_push_blank(&this->ArrivalScheduled);
      if (IBIS_IP_dateTime_s_read(IBIS_IP_dateTime_array_s_last(&this->ArrivalScheduled),ArrivalScheduled))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s DepartureScheduled = loc_s_dict_str_get(nodes_var,"DepartureScheduled");
  if (DepartureScheduled != NULL)
  {
    if (DepartureScheduled->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(DepartureScheduled);
      if (array_ptr->used != 0)
      {
        IBIS_IP_dateTime_array_s_copy_resize(&this->DepartureScheduled,1);
        IBIS_IP_dateTime_array_s_push_blank(&this->DepartureScheduled);
        if (IBIS_IP_dateTime_s_read(IBIS_IP_dateTime_array_s_last(&this->DepartureScheduled),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_dateTime_array_s_copy_resize(&this->DepartureScheduled,1);
      IBIS_IP_dateTime_array_s_push_blank(&this->DepartureScheduled);
      if (IBIS_IP_dateTime_s_read(IBIS_IP_dateTime_array_s_last(&this->DepartureScheduled),DepartureScheduled))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s Connection = loc_s_dict_str_get(nodes_var,"Connection");
  if (Connection != NULL)
  {
    if (Connection->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(Connection);
      ConnectionStructure_array_s_copy_resize(&this->Connection,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          ConnectionStructure_array_s_push_blank(&this->Connection);
          if (ConnectionStructure_s_read(ConnectionStructure_array_s_last(&this->Connection),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      ConnectionStructure_array_s_copy_resize(&this->Connection,1);
      ConnectionStructure_array_s_push_blank(&this->Connection);
      if (ConnectionStructure_s_read(ConnectionStructure_array_s_last(&this->Connection),Connection))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s BayArea = loc_s_dict_str_get(nodes_var,"BayArea");
  if (BayArea != NULL)
  {
    if (BayArea->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(BayArea);
      if (array_ptr->used != 0)
      {
        BayAreaStructure_array_s_copy_resize(&this->BayArea,1);
        BayAreaStructure_array_s_push_blank(&this->BayArea);
        if (BayAreaStructure_s_read(BayAreaStructure_array_s_last(&this->BayArea),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      BayAreaStructure_array_s_copy_resize(&this->BayArea,1);
      BayAreaStructure_array_s_push_blank(&this->BayArea);
      if (BayAreaStructure_s_read(BayAreaStructure_array_s_last(&this->BayArea),BayArea))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s GNSSPoint = loc_s_dict_str_get(nodes_var,"GNSSPoint");
  if (GNSSPoint != NULL)
  {
    if (GNSSPoint->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(GNSSPoint);
      if (array_ptr->used != 0)
      {
        GNSSPointStructure_array_s_copy_resize(&this->GNSSPoint,1);
        GNSSPointStructure_array_s_push_blank(&this->GNSSPoint);
        if (GNSSPointStructure_s_read(GNSSPointStructure_array_s_last(&this->GNSSPoint),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      GNSSPointStructure_array_s_copy_resize(&this->GNSSPoint,1);
      GNSSPointStructure_array_s_push_blank(&this->GNSSPoint);
      if (GNSSPointStructure_s_read(GNSSPointStructure_array_s_last(&this->GNSSPoint),GNSSPoint))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s FareZone = loc_s_dict_str_get(nodes_var,"FareZone");
  if (FareZone != NULL)
  {
    if (FareZone->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(FareZone);
      if (array_ptr->used != 0)
      {
        IBIS_IP_NMTOKEN_array_s_copy_resize(&this->FareZone,1);
        IBIS_IP_NMTOKEN_array_s_push_blank(&this->FareZone);
        if (IBIS_IP_NMTOKEN_s_read(IBIS_IP_NMTOKEN_array_s_last(&this->FareZone),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_NMTOKEN_array_s_copy_resize(&this->FareZone,1);
      IBIS_IP_NMTOKEN_array_s_push_blank(&this->FareZone);
      if (IBIS_IP_NMTOKEN_s_read(IBIS_IP_NMTOKEN_array_s_last(&this->FareZone),FareZone))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- ShortTripStopStructure_s --
@begin
methods ShortTripStopStructure_s
@end

int ShortTripStopStructure_s_write(ShortTripStopStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<JourneyStopInformation>");
  if (JourneyStopInformationStructure_s_write(&this->JourneyStopInformation,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</JourneyStopInformation>");
  bc_array_s_append_ptr(a_trg,"<FareZoneInformation>");
  if (FareZoneInformationStructure_s_write(&this->FareZoneInformation,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</FareZoneInformation>");
  return 0;
}/*}}}*/

int ShortTripStopStructure_s_read(ShortTripStopStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s JourneyStopInformation = loc_s_dict_str_get(nodes_var,"JourneyStopInformation");
  if (JourneyStopInformation == NULL ||
      JourneyStopInformationStructure_s_read(&this->JourneyStopInformation,JourneyStopInformation))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s FareZoneInformation = loc_s_dict_str_get(nodes_var,"FareZoneInformation");
  if (FareZoneInformation == NULL ||
      FareZoneInformationStructure_s_read(&this->FareZoneInformation,FareZoneInformation))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- StopPointTariffInformationStructure_s --
@begin
methods StopPointTariffInformationStructure_s
@end

int StopPointTariffInformationStructure_s_write(StopPointTariffInformationStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<JourneyStopInformation>");
  if (JourneyStopInformationStructure_s_write(&this->JourneyStopInformation,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</JourneyStopInformation>");
  bc_array_s_append_ptr(a_trg,"<FareZoneInformation>");
  if (FareZoneInformationStructure_s_write(&this->FareZoneInformation,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</FareZoneInformation>");
  return 0;
}/*}}}*/

int StopPointTariffInformationStructure_s_read(StopPointTariffInformationStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s JourneyStopInformation = loc_s_dict_str_get(nodes_var,"JourneyStopInformation");
  if (JourneyStopInformation == NULL ||
      JourneyStopInformationStructure_s_read(&this->JourneyStopInformation,JourneyStopInformation))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s FareZoneInformation = loc_s_dict_str_get(nodes_var,"FareZoneInformation");
  if (FareZoneInformation == NULL ||
      FareZoneInformationStructure_s_read(&this->FareZoneInformation,FareZoneInformation))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- ShortTripStopStructure_array_s --
@begin
methods ShortTripStopStructure_array_s
@end

// -- ShortTripStopListStructure_s --
@begin
methods ShortTripStopListStructure_s
@end

int ShortTripStopListStructure_s_write(ShortTripStopListStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->ShortTripStop.used != 0)
  {
    ShortTripStopStructure_s *ptr = this->ShortTripStop.data;
    ShortTripStopStructure_s *ptr_end = ptr + this->ShortTripStop.used;
    do {
      bc_array_s_append_ptr(a_trg,"<ShortTripStop>");
      if (ShortTripStopStructure_s_write(ptr,a_trg))
      {
        throw_error(VDV301_WRITE_DATA_ERROR);
      }
      bc_array_s_append_ptr(a_trg,"</ShortTripStop>");
    } while(++ptr < ptr_end);
  }
  return 0;
}/*}}}*/

int ShortTripStopListStructure_s_read(ShortTripStopListStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ShortTripStop = loc_s_dict_str_get(nodes_var,"ShortTripStop");
  if (ShortTripStop != NULL)
  {
    if (ShortTripStop->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(ShortTripStop);
      ShortTripStopStructure_array_s_copy_resize(&this->ShortTripStop,array_ptr->used);
      if (array_ptr->used != 0)
      {
        var_s *ptr = array_ptr->data;
        var_s *ptr_end = ptr + array_ptr->used;
        do {
          ShortTripStopStructure_array_s_push_blank(&this->ShortTripStop);
          if (ShortTripStopStructure_s_read(ShortTripStopStructure_array_s_last(&this->ShortTripStop),*ptr))
          {
            throw_error(VDV301_READ_DATA_ERROR);
          }
        } while(++ptr < ptr_end);
      }
    }
    else
    {
      ShortTripStopStructure_array_s_copy_resize(&this->ShortTripStop,1);
      ShortTripStopStructure_array_s_push_blank(&this->ShortTripStop);
      if (ShortTripStopStructure_s_read(ShortTripStopStructure_array_s_last(&this->ShortTripStop),ShortTripStop))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

// -- TicketingService_GetTariffInformationResponseDataStructure_s --
@begin
methods TicketingService_GetTariffInformationResponseDataStructure_s
@end

int TicketingService_GetTariffInformationResponseDataStructure_s_write(TicketingService_GetTariffInformationResponseDataStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"<DefaultLanguage>");
  if (IBIS_IP_language_s_write(&this->DefaultLanguage,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</DefaultLanguage>");
  bc_array_s_append_ptr(a_trg,"<TimeStamp>");
  if (IBIS_IP_dateTime_s_write(&this->TimeStamp,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</TimeStamp>");
  bc_array_s_append_ptr(a_trg,"<TripRef>");
  if (IBIS_IP_NMTOKEN_s_write(&this->TripRef,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</TripRef>");
  bc_array_s_append_ptr(a_trg,"<Line>");
  if (LineInformationStructure_s_write(&this->Line,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</Line>");
  bc_array_s_append_ptr(a_trg,"<StopPointTariffInformation>");
  if (StopPointTariffInformationStructure_s_write(&this->StopPointTariffInformation,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</StopPointTariffInformation>");
  bc_array_s_append_ptr(a_trg,"<ShortTripStopList>");
  if (ShortTripStopListStructure_s_write(&this->ShortTripStopList,a_trg))
  {
    throw_error(VDV301_WRITE_DATA_ERROR);
  }
  bc_array_s_append_ptr(a_trg,"</ShortTripStopList>");
  return 0;
}/*}}}*/

int TicketingService_GetTariffInformationResponseDataStructure_s_read(TicketingService_GetTariffInformationResponseDataStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s DefaultLanguage = loc_s_dict_str_get(nodes_var,"DefaultLanguage");
  if (DefaultLanguage == NULL ||
      IBIS_IP_language_s_read(&this->DefaultLanguage,DefaultLanguage))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s TimeStamp = loc_s_dict_str_get(nodes_var,"TimeStamp");
  if (TimeStamp == NULL ||
      IBIS_IP_dateTime_s_read(&this->TimeStamp,TimeStamp))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s TripRef = loc_s_dict_str_get(nodes_var,"TripRef");
  if (TripRef == NULL ||
      IBIS_IP_NMTOKEN_s_read(&this->TripRef,TripRef))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s Line = loc_s_dict_str_get(nodes_var,"Line");
  if (Line == NULL ||
      LineInformationStructure_s_read(&this->Line,Line))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s StopPointTariffInformation = loc_s_dict_str_get(nodes_var,"StopPointTariffInformation");
  if (StopPointTariffInformation == NULL ||
      StopPointTariffInformationStructure_s_read(&this->StopPointTariffInformation,StopPointTariffInformation))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s ShortTripStopList = loc_s_dict_str_get(nodes_var,"ShortTripStopList");
  if (ShortTripStopList == NULL ||
      ShortTripStopListStructure_s_read(&this->ShortTripStopList,ShortTripStopList))
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  return 0;
}/*}}}*/

// -- TicketingService_GetTariffInformationResponseDataStructure_array_s --
@begin
methods TicketingService_GetTariffInformationResponseDataStructure_array_s
@end

// -- TicketingService_GetTariffInformationResponseStructure_s --
@begin
methods TicketingService_GetTariffInformationResponseStructure_s
@end

int TicketingService_GetTariffInformationResponseStructure_s_write(TicketingService_GetTariffInformationResponseStructure_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->TicketingService_GetTariffInformationResponseData.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<TicketingService.GetTariffInformationResponseData>");
    if (TicketingService_GetTariffInformationResponseDataStructure_s_write(TicketingService_GetTariffInformationResponseDataStructure_array_s_last(&this->TicketingService_GetTariffInformationResponseData),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</TicketingService.GetTariffInformationResponseData>");
  }
  if (this->OperationErrorMessage.used != 0)
  {
    bc_array_s_append_ptr(a_trg,"<OperationErrorMessage>");
    if (IBIS_IP_string_s_write(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),a_trg))
    {
      throw_error(VDV301_WRITE_DATA_ERROR);
    }
    bc_array_s_append_ptr(a_trg,"</OperationErrorMessage>");
  }
  return 0;
}/*}}}*/

int TicketingService_GetTariffInformationResponseStructure_s_read(TicketingService_GetTariffInformationResponseStructure_s *this,var_s a_var)
{/*{{{*/
  var_s nodes_var = loc_s_dict_get(a_var,g_str_nodes_var);
  if (nodes_var == NULL)
  {
    throw_error(VDV301_READ_DATA_ERROR);
  }
  var_s TicketingService_GetTariffInformationResponseData = loc_s_dict_str_get(nodes_var,"TicketingService.GetTariffInformationResponseData");
  if (TicketingService_GetTariffInformationResponseData != NULL)
  {
    if (TicketingService_GetTariffInformationResponseData->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(TicketingService_GetTariffInformationResponseData);
      if (array_ptr->used != 0)
      {
        TicketingService_GetTariffInformationResponseDataStructure_array_s_copy_resize(&this->TicketingService_GetTariffInformationResponseData,1);
        TicketingService_GetTariffInformationResponseDataStructure_array_s_push_blank(&this->TicketingService_GetTariffInformationResponseData);
        if (TicketingService_GetTariffInformationResponseDataStructure_s_read(TicketingService_GetTariffInformationResponseDataStructure_array_s_last(&this->TicketingService_GetTariffInformationResponseData),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      TicketingService_GetTariffInformationResponseDataStructure_array_s_copy_resize(&this->TicketingService_GetTariffInformationResponseData,1);
      TicketingService_GetTariffInformationResponseDataStructure_array_s_push_blank(&this->TicketingService_GetTariffInformationResponseData);
      if (TicketingService_GetTariffInformationResponseDataStructure_s_read(TicketingService_GetTariffInformationResponseDataStructure_array_s_last(&this->TicketingService_GetTariffInformationResponseData),TicketingService_GetTariffInformationResponseData))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  var_s OperationErrorMessage = loc_s_dict_str_get(nodes_var,"OperationErrorMessage");
  if (OperationErrorMessage != NULL)
  {
    if (OperationErrorMessage->v_type == c_bi_type_array)
    {
      var_array_s *array_ptr = loc_s_array_value(OperationErrorMessage);
      if (array_ptr->used != 0)
      {
        IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
        IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
        if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),*array_ptr->data))
        {
          throw_error(VDV301_READ_DATA_ERROR);
        }
      }
    }
    else
    {
      IBIS_IP_string_array_s_copy_resize(&this->OperationErrorMessage,1);
      IBIS_IP_string_array_s_push_blank(&this->OperationErrorMessage);
      if (IBIS_IP_string_s_read(IBIS_IP_string_array_s_last(&this->OperationErrorMessage),OperationErrorMessage))
      {
        throw_error(VDV301_READ_DATA_ERROR);
      }
    }
  }
  return 0;
}/*}}}*/

void libvdv301_cll_init()
{/*{{{*/
  vdv301_operation_descr_tree_s_init(&g_vdv301_operation_descr_tree);
  {
    CONT_INIT_CLEAR(string_s,value);
    string_tree_s_init(&g_ConnectionStateEnumeration_tree);
    string_s_set_ptr(&value,"ConnectionBroken");
    string_tree_s_swap_insert(&g_ConnectionStateEnumeration_tree,&value);
    string_s_set_ptr(&value,"ConnectionOK");
    string_tree_s_swap_insert(&g_ConnectionStateEnumeration_tree,&value);
    string_s_set_ptr(&value,"NoInformationAvailable");
    string_tree_s_swap_insert(&g_ConnectionStateEnumeration_tree,&value);
  }
  {
    CONT_INIT_CLEAR(string_s,value);
    string_tree_s_init(&g_ConnectionTypeEnumeration_tree);
    string_s_set_ptr(&value,"Interchange");
    string_tree_s_swap_insert(&g_ConnectionTypeEnumeration_tree,&value);
    string_s_set_ptr(&value,"ProtectedConnection");
    string_tree_s_swap_insert(&g_ConnectionTypeEnumeration_tree,&value);
  }
  {
    CONT_INIT_CLEAR(string_s,value);
    string_tree_s_init(&g_DoorCountingObjectClassEnumeration_tree);
    string_s_set_ptr(&value,"Adult");
    string_tree_s_swap_insert(&g_DoorCountingObjectClassEnumeration_tree,&value);
    string_s_set_ptr(&value,"Child");
    string_tree_s_swap_insert(&g_DoorCountingObjectClassEnumeration_tree,&value);
    string_s_set_ptr(&value,"Bike");
    string_tree_s_swap_insert(&g_DoorCountingObjectClassEnumeration_tree,&value);
    string_s_set_ptr(&value,"WheelChair");
    string_tree_s_swap_insert(&g_DoorCountingObjectClassEnumeration_tree,&value);
    string_s_set_ptr(&value,"Pram");
    string_tree_s_swap_insert(&g_DoorCountingObjectClassEnumeration_tree,&value);
    string_s_set_ptr(&value,"Unidentified");
    string_tree_s_swap_insert(&g_DoorCountingObjectClassEnumeration_tree,&value);
    string_s_set_ptr(&value,"Other");
    string_tree_s_swap_insert(&g_DoorCountingObjectClassEnumeration_tree,&value);
  }
  {
    CONT_INIT_CLEAR(string_s,value);
    string_tree_s_init(&g_DoorCountingQualityEnumeration_tree);
    string_s_set_ptr(&value,"Defect");
    string_tree_s_swap_insert(&g_DoorCountingQualityEnumeration_tree,&value);
    string_s_set_ptr(&value,"Regular");
    string_tree_s_swap_insert(&g_DoorCountingQualityEnumeration_tree,&value);
    string_s_set_ptr(&value,"Sabotage");
    string_tree_s_swap_insert(&g_DoorCountingQualityEnumeration_tree,&value);
    string_s_set_ptr(&value,"Other");
    string_tree_s_swap_insert(&g_DoorCountingQualityEnumeration_tree,&value);
  }
  {
    CONT_INIT_CLEAR(string_s,value);
    string_tree_s_init(&g_DoorOpenStateEnumeration_tree);
    string_s_set_ptr(&value,"DoorsOpen");
    string_tree_s_swap_insert(&g_DoorOpenStateEnumeration_tree,&value);
    string_s_set_ptr(&value,"AllDoorsClosed");
    string_tree_s_swap_insert(&g_DoorOpenStateEnumeration_tree,&value);
    string_s_set_ptr(&value,"SingleDoorOpen");
    string_tree_s_swap_insert(&g_DoorOpenStateEnumeration_tree,&value);
    string_s_set_ptr(&value,"SingleDoorClosed");
    string_tree_s_swap_insert(&g_DoorOpenStateEnumeration_tree,&value);
  }
  {
    CONT_INIT_CLEAR(string_s,value);
    string_tree_s_init(&g_DoorOperationStateEnumeration_tree);
    string_s_set_ptr(&value,"Locked");
    string_tree_s_swap_insert(&g_DoorOperationStateEnumeration_tree,&value);
    string_s_set_ptr(&value,"Normal");
    string_tree_s_swap_insert(&g_DoorOperationStateEnumeration_tree,&value);
    string_s_set_ptr(&value,"EmergencyRelease");
    string_tree_s_swap_insert(&g_DoorOperationStateEnumeration_tree,&value);
  }
  {
    CONT_INIT_CLEAR(string_s,value);
    string_tree_s_init(&g_ErrorCodeEnumeration_tree);
    string_s_set_ptr(&value,"DataEstimated");
    string_tree_s_swap_insert(&g_ErrorCodeEnumeration_tree,&value);
    string_s_set_ptr(&value,"FaultData");
    string_tree_s_swap_insert(&g_ErrorCodeEnumeration_tree,&value);
    string_s_set_ptr(&value,"NoScheduleDataAvailable");
    string_tree_s_swap_insert(&g_ErrorCodeEnumeration_tree,&value);
    string_s_set_ptr(&value,"DeviceMissing");
    string_tree_s_swap_insert(&g_ErrorCodeEnumeration_tree,&value);
    string_s_set_ptr(&value,"NoServiceResponse");
    string_tree_s_swap_insert(&g_ErrorCodeEnumeration_tree,&value);
    string_s_set_ptr(&value,"ImportantDataNotAvailable");
    string_tree_s_swap_insert(&g_ErrorCodeEnumeration_tree,&value);
    string_s_set_ptr(&value,"DataNotValid");
    string_tree_s_swap_insert(&g_ErrorCodeEnumeration_tree,&value);
  }
  {
    CONT_INIT_CLEAR(string_s,value);
    string_tree_s_init(&g_TicketRazziaInformationEnumeration_tree);
    string_s_set_ptr(&value,"razzia");
    string_tree_s_swap_insert(&g_TicketRazziaInformationEnumeration_tree,&value);
    string_s_set_ptr(&value,"norazzia");
    string_tree_s_swap_insert(&g_TicketRazziaInformationEnumeration_tree,&value);
  }
  {
    CONT_INIT_CLEAR(string_s,value);
    string_tree_s_init(&g_TicketValidationEnumeration_tree);
    string_s_set_ptr(&value,"valid");
    string_tree_s_swap_insert(&g_TicketValidationEnumeration_tree,&value);
    string_s_set_ptr(&value,"notvalid");
    string_tree_s_swap_insert(&g_TicketValidationEnumeration_tree,&value);
    string_s_set_ptr(&value,"NoCard");
    string_tree_s_swap_insert(&g_TicketValidationEnumeration_tree,&value);
  }
  {
    CONT_INIT_CLEAR(string_s,value);
    string_tree_s_init(&g_CountingStateEnumeration_tree);
    string_s_set_ptr(&value,"StartRequested");
    string_tree_s_swap_insert(&g_CountingStateEnumeration_tree,&value);
    string_s_set_ptr(&value,"Started");
    string_tree_s_swap_insert(&g_CountingStateEnumeration_tree,&value);
    string_s_set_ptr(&value,"StopRequested");
    string_tree_s_swap_insert(&g_CountingStateEnumeration_tree,&value);
    string_s_set_ptr(&value,"Stopped");
    string_tree_s_swap_insert(&g_CountingStateEnumeration_tree,&value);
  }
}/*}}}*/

void libvdv301_cll_clear()
{/*{{{*/
  string_tree_s_clear(&g_ConnectionStateEnumeration_tree);
  string_tree_s_clear(&g_ConnectionTypeEnumeration_tree);
  string_tree_s_clear(&g_DoorCountingObjectClassEnumeration_tree);
  string_tree_s_clear(&g_DoorCountingQualityEnumeration_tree);
  string_tree_s_clear(&g_DoorOpenStateEnumeration_tree);
  string_tree_s_clear(&g_DoorOperationStateEnumeration_tree);
  string_tree_s_clear(&g_ErrorCodeEnumeration_tree);
  string_tree_s_clear(&g_TicketRazziaInformationEnumeration_tree);
  string_tree_s_clear(&g_TicketValidationEnumeration_tree);
  string_tree_s_clear(&g_CountingStateEnumeration_tree);
  vdv301_operation_descr_tree_s_clear(&g_vdv301_operation_descr_tree);
}/*}}}*/

// VDV301 GENERATED METHODS

