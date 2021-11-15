
#ifndef __CL_VDV301_BASIC_H
#define __CL_VDV301_BASIC_H

@begin
include "cl_xsd.h"
@end

// - error codes -
#define ERROR_VDV301_INVALID_DATA_ERROR 1
#define ERROR_VDV301_WRITE_DATA_ERROR 2
#define ERROR_VDV301_READ_DATA_ERROR 3
#define ERROR_VDV301_DATE_TIME_PARSE_ERROR 4
#define ERROR_VDV301_DURATION_PARSE_ERROR 5

// === constants and definitions ===============================================

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

#endif

