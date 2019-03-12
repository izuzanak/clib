
#ifndef __CL_STRUCT_H
#define __CL_STRUCT_H

@begin
include "cl_string.h"
@end

// === definition of generated structures ======================================

// -- json_nice_s --
@begin
struct
<
string_s:tabulator
bc_array_s:indent_buffer
ui:indent_size
>
json_nice_s;
@end

static inline void json_nice_s_create(json_nice_s *this,const char *a_tabulator,const char *a_indentation);
static inline void json_nice_s_push_indent(json_nice_s *this,bc_array_s *a_trg);
static inline void json_nice_s_pop_indent(json_nice_s *this,bc_array_s *a_trg);
static inline void json_nice_s_indent(const json_nice_s *this,bc_array_s *a_trg);

// -- lalr_stack_element_s --
@begin
struct
<
ui:lalr_state
ui:terminal_start
ui:terminal_end
>
lalr_stack_element_s;
@end

// -- lalr_stack_s --
@begin
array<lalr_stack_element_s>
lalr_stack_s;
@end

static inline void lalr_stack_s_push_state(lalr_stack_s *this,unsigned a_lalr_state);
static inline void lalr_stack_s_push_state_all(lalr_stack_s *this,unsigned a_lalr_state,unsigned a_terminal_start,unsigned a_terminal_end);

// -- pointer_array_s --
@begin
array<pointer> pointer_array_s;
@end

// === inline methods of generated structures ==================================

// -- json_nice_s --
@begin
inlines json_nice_s
@end

static inline void json_nice_s_create(json_nice_s *this,const char *a_tabulator,const char *a_indentation)
{/*{{{*/
  json_nice_s_clear(this);

  string_s_set_ptr(&this->tabulator,a_tabulator);
  bc_array_s_push(&this->indent_buffer,'\n');
  bc_array_s_append_ptr(&this->indent_buffer,a_indentation);
  this->indent_size = this->indent_buffer.used;
}/*}}}*/

static inline void json_nice_s_push_indent(json_nice_s *this,bc_array_s *a_trg)
{/*{{{*/
  if ((this->indent_size += this->tabulator.size - 1) > this->indent_buffer.used)
  {
    bc_array_s_append(&this->indent_buffer,this->tabulator.size - 1,this->tabulator.data);
  }

  bc_array_s_append(a_trg,this->indent_size,this->indent_buffer.data);
}/*}}}*/

static inline void json_nice_s_pop_indent(json_nice_s *this,bc_array_s *a_trg)
{/*{{{*/
  this->indent_size -= this->tabulator.size - 1;
  bc_array_s_append(a_trg,this->indent_size,this->indent_buffer.data);
}/*}}}*/

static inline void json_nice_s_indent(const json_nice_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append(a_trg,this->indent_size,this->indent_buffer.data);
}/*}}}*/

// -- lalr_stack_element_s --
@begin
inlines lalr_stack_element_s
@end

// -- lalr_stack_s --
@begin
inlines lalr_stack_s
@end

static inline void lalr_stack_s_push_state(lalr_stack_s *this,unsigned a_lalr_state)
{/*{{{*/
  lalr_stack_s_push_blank(this);
  lalr_stack_s_last(this)->lalr_state = a_lalr_state;
}/*}}}*/

static inline void lalr_stack_s_push_state_all(lalr_stack_s *this,unsigned a_lalr_state,unsigned a_terminal_start,unsigned a_terminal_end)
{/*{{{*/
  lalr_stack_s_push_blank(this);
  lalr_stack_element_s_set(lalr_stack_s_last(this),a_lalr_state,a_terminal_start,a_terminal_end);
}/*}}}*/

// -- pointer_array_s --
@begin
inlines pointer_array_s
@end

#endif

