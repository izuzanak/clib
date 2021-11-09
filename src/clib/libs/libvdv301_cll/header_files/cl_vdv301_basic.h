
#ifndef __CL_VDV301_BASIC_H
#define __CL_VDV301_BASIC_H

@begin
include "cl_time.h"
include "cl_xml.h"
include "cl_regex.h"
@end

// - error codes -
#define ERROR_VDV301_INVALID_DATA_ERROR 1
#define ERROR_VDV301_WRITE_DATA_ERROR 2
#define ERROR_VDV301_READ_DATA_ERROR 3
#define ERROR_VDV301_DATE_TIME_PARSE_ERROR 4
#define ERROR_VDV301_DURATION_PARSE_ERROR 5

// === constants and definitions ===============================================

extern const char XML_HEADER[];

typedef void (*vdv301_init_t)(void *);
typedef void (*vdv301_clear_t)(void *);

// === definition of structure vdv301_operation_s ==============================

enum
{
  vdv301_op_state_RECV = 0,
  vdv301_op_state_SUSPEND,
  vdv301_op_state_RESPONSE,
  vdv301_op_state_FAULT,
};

typedef struct vdv301_operation_s
{
  unsigned state;
  unsigned index;
  void *input;
  void *output;
  unsigned fault_index;
  void *fault;
}
vdv301_operation_s;

@begin
define vdv301_operation_s dynamic
@end

static inline void vdv301_operation_s_init(vdv301_operation_s *this);
static inline void vdv301_operation_s_clear(vdv301_operation_s *this);
static inline void vdv301_operation_s_flush_all(vdv301_operation_s *this);
static inline void vdv301_operation_s_swap(vdv301_operation_s *this,vdv301_operation_s *a_second);
static inline void vdv301_operation_s_copy(const vdv301_operation_s *this,const vdv301_operation_s *a_src);
static inline int vdv301_operation_s_compare(const vdv301_operation_s *this,const vdv301_operation_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void vdv301_operation_s_to_string(const vdv301_operation_s *this,bc_array_s *a_trg);
#endif

// === definition of generated structures ======================================

// -- vdv301_operation_descr_s --
@begin
struct
<
string_s:name
ui:input_size
pointer:input_init
pointer:input_clear
pointer:input_read
ui:output_size
pointer:output_init
pointer:output_clear
pointer:output_write
ui_array_s:faults
>
vdv301_operation_descr_s;
@end

// -- vdv301_operation_descr_tree_s --
@begin
safe_rb_tree<vdv301_operation_descr_s> vdv301_operation_descr_tree_s;
@end

// -- lli_array_s --
@begin
array<lli> lli_array_s;
@end

// -- bd_array_s --
@begin
array<bd> bd_array_s;
@end

// -- xs_boolean_s --
@begin
abbreviation bi xs_boolean_s
@end

WUR static inline int xs_boolean_s_write(const xs_boolean_s *this,bc_array_s *a_trg);
WUR static inline int xs_boolean_s_read(xs_boolean_s *this,var_s a_var);

// -- xs_byte_s --
@begin
abbreviation bi xs_byte_s
@end

WUR static inline int xs_byte_s_write(const xs_byte_s *this,bc_array_s *a_trg);
WUR static inline int xs_byte_s_read(xs_byte_s *this,var_s a_var);

// -- xs_int_s --
@begin
abbreviation lli xs_int_s
@end

WUR static inline int xs_int_s_write(const xs_int_s *this,bc_array_s *a_trg);
WUR static inline int xs_int_s_read(xs_int_s *this,var_s a_var);

// -- xs_unsignedInt_s --
@begin
abbreviation lli xs_unsignedInt_s
@end

WUR static inline int xs_unsignedInt_s_write(const xs_unsignedInt_s *this,bc_array_s *a_trg);
WUR static inline int xs_unsignedInt_s_read(xs_unsignedInt_s *this,var_s a_var);

// -- xs_unsignedLong_s --
@begin
abbreviation ulli xs_unsignedLong_s
@end

WUR static inline int xs_unsignedLong_s_write(const xs_unsignedLong_s *this,bc_array_s *a_trg);
WUR static inline int xs_unsignedLong_s_read(xs_unsignedLong_s *this,var_s a_var);

// -- xs_nonNegativeInteger_s --
@begin
abbreviation lli xs_nonNegativeInteger_s
@end

WUR static inline int xs_nonNegativeInteger_s_write(const xs_nonNegativeInteger_s *this,bc_array_s *a_trg);
WUR static inline int xs_nonNegativeInteger_s_read(xs_nonNegativeInteger_s *this,var_s a_var);

// -- xs_double_s --
@begin
abbreviation bd xs_double_s
@end

WUR static inline int xs_double_s_write(const xs_double_s *this,bc_array_s *a_trg);
WUR static inline int xs_double_s_read(xs_double_s *this,var_s a_var);

// -- xs_string_s --
@begin
abbreviation string_s xs_string_s
@end

WUR static inline int xs_string_s_write(const xs_string_s *this,bc_array_s *a_trg);
WUR static inline int xs_string_s_read(xs_string_s *this,var_s a_var);

// -- xs_language_s --
@begin
abbreviation string_s xs_language_s
@end

WUR static inline int xs_language_s_write(const xs_language_s *this,bc_array_s *a_trg);
WUR static inline int xs_language_s_read(xs_language_s *this,var_s a_var);

// -- xs_NMTOKEN_s --
@begin
abbreviation string_s xs_NMTOKEN_s
@end

WUR static inline int xs_NMTOKEN_s_write(const xs_NMTOKEN_s *this,bc_array_s *a_trg);
WUR static inline int xs_NMTOKEN_s_read(xs_NMTOKEN_s *this,var_s a_var);

// -- xs_dateTime_s --
@begin
abbreviation string_s xs_dateTime_s
@end

WUR static inline int xs_dateTime_s_write(const xs_dateTime_s *this,bc_array_s *a_trg);
WUR static inline int xs_dateTime_s_read(xs_dateTime_s *this,var_s a_var);

void xs_dateTime_s_create(xs_dateTime_s *this,time_s a_time);
WUR int xs_dateTime_s_parse(xs_dateTime_s *this,time_s *a_time);

// -- xs_duration_s --
@begin
abbreviation string_s xs_duration_s
@end

WUR static inline int xs_duration_s_write(const xs_duration_s *this,bc_array_s *a_trg);
WUR static inline int xs_duration_s_read(xs_duration_s *this,var_s a_var);

void xs_duration_s_create(xs_duration_s *this,lli a_period);
WUR int xs_duration_s_parse(xs_duration_s *this,lli *a_period);

// === global variables ========================================================

extern vdv301_operation_descr_tree_s g_vdv301_operation_descr_tree;

// === inline methods of structure vdv301_operation_s ===========================

static inline void vdv301_operation_s_init(vdv301_operation_s *this)
{/*{{{*/
  this->index = c_idx_not_exist;
  this->input = NULL;
  this->output = NULL;
  this->fault_index = c_idx_not_exist;
  this->fault = NULL;
}/*}}}*/

static inline void vdv301_operation_s_clear(vdv301_operation_s *this)
{/*{{{*/
  if (this->index != c_idx_not_exist)
  {
    vdv301_operation_descr_s *descr = vdv301_operation_descr_tree_s_at(&g_vdv301_operation_descr_tree,this->index);

    if (this->input != NULL)
    {
      ((vdv301_clear_t)(descr->input_clear))(this->input);
      cfree(this->input);
    }

    if (this->output != NULL)
    {
      ((vdv301_clear_t)(descr->output_clear))(this->output);
      cfree(this->output);
    }
  }

  vdv301_operation_s_init(this);
}/*}}}*/

static inline void vdv301_operation_s_flush_all(vdv301_operation_s *this)
{/*{{{*/
}/*}}}*/

static inline void vdv301_operation_s_swap(vdv301_operation_s *this,vdv301_operation_s *a_second)
{/*{{{*/
  vdv301_operation_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void vdv301_operation_s_copy(const vdv301_operation_s *this,const vdv301_operation_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int vdv301_operation_s_compare(const vdv301_operation_s *this,const vdv301_operation_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void vdv301_operation_s_to_string(const vdv301_operation_s *this,bc_array_s *a_trg)
{/*{{{*/
  (void)this;

  bc_array_s_append_ptr(a_trg,"vdv301_operation_s{}");
}/*}}}*/
#endif

// === inline methods of generated structures ==================================

// -- vdv301_operation_descr_s --
@begin
inlines vdv301_operation_descr_s
@end

// -- vdv301_operation_descr_tree_s --
@begin
inlines vdv301_operation_descr_tree_s
@end

static inline int vdv301_operation_descr_tree_s___compare_value(const vdv301_operation_descr_tree_s *this,const vdv301_operation_descr_s *a_first,const vdv301_operation_descr_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->name;
  const string_s *second = &a_second->name;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }
  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- lli_array_s --
@begin
inlines lli_array_s
@end

// -- bd_array_s --
@begin
inlines bd_array_s
@end

// -- xs_boolean_s --
static inline int xs_boolean_s_write(const xs_boolean_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,*this ? "true" : "false");

  return 0;
}/*}}}*/

static inline int xs_boolean_s_read(xs_boolean_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  *this = strcmp(loc_s_string_value(value_var)->data,"true") == 0;
  return 0;
}/*}}}*/

// -- xs_byte_s --
static inline int xs_byte_s_write(const xs_byte_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%d",*this);

  return 0;
}/*}}}*/

static inline int xs_byte_s_read(xs_byte_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);

  char *end_ptr;
  *this = strtol(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(VDV301_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_int_s --
static inline int xs_int_s_write(const xs_int_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%lld",*this);

  return 0;
}/*}}}*/

static inline int xs_int_s_read(xs_int_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);

  char *end_ptr;
  *this = strtoll(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(VDV301_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_unsignedInt_s --
static inline int xs_unsignedInt_s_write(const xs_unsignedInt_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (*this < 0)
  {
    throw_error(VDV301_INVALID_DATA_ERROR);
  }

  bc_array_s_append_format(a_trg,"%lld",*this);

  return 0;
}/*}}}*/

static inline int xs_unsignedInt_s_read(xs_unsignedInt_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);

  char *end_ptr;
  *this = strtoll(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1) ||
      *this < 0)
  {
    throw_error(VDV301_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_unsignedLong_s --
static inline int xs_unsignedLong_s_write(const xs_unsignedLong_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (*this < 0)
  {
    throw_error(VDV301_INVALID_DATA_ERROR);
  }

  bc_array_s_append_format(a_trg,"%llu",*this);

  return 0;
}/*}}}*/

static inline int xs_unsignedLong_s_read(xs_unsignedLong_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);

  char *end_ptr;
  *this = strtoll(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1) ||
      *this < 0)
  {
    throw_error(VDV301_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_nonNegativeInteger_s --
static inline int xs_nonNegativeInteger_s_write(const xs_nonNegativeInteger_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (*this < 0)
  {
    throw_error(VDV301_INVALID_DATA_ERROR);
  }

  bc_array_s_append_format(a_trg,"%lld",*this);

  return 0;
}/*}}}*/

static inline int xs_nonNegativeInteger_s_read(xs_nonNegativeInteger_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);

  char *end_ptr;
  *this = strtoll(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1) ||
      *this < 0)
  {
    throw_error(VDV301_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_double_s --
static inline int xs_double_s_write(const xs_double_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%f",*this);

  return 0;
}/*}}}*/

WUR static inline int xs_double_s_read(xs_double_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);
  char *end_ptr;
  *this = strtod(value_str->data,&end_ptr);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(VDV301_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_string_s --
static inline int xs_string_s_write(const xs_string_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

static inline int xs_string_s_read(xs_string_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

// -- xs_language_s --
static inline int xs_language_s_write(const xs_language_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

static inline int xs_language_s_read(xs_language_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

// -- xs_NMTOKEN_s --
static inline int xs_NMTOKEN_s_write(const xs_NMTOKEN_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

static inline int xs_NMTOKEN_s_read(xs_NMTOKEN_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

// -- xs_dateTime_s --
static inline int xs_dateTime_s_write(const xs_dateTime_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

static inline int xs_dateTime_s_read(xs_dateTime_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

// -- xs_duration_s --
static inline int xs_duration_s_write(const xs_duration_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

static inline int xs_duration_s_read(xs_duration_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

#endif

