
#ifndef CL_LIB_STRUCT_H
#define CL_LIB_STRUCT_H

@begin
include "cl_string.h"
@end

// === definition of generated structures ======================================

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

// -- pointer_list_s --
@begin
safe_list<pointer> pointer_list_s;
@end

// -- pointer_tree_s --
@begin
rb_tree<pointer> pointer_tree_s;
@end

// === inline methods of generated structures ==================================

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

// -- pointer_list_s --
@begin
inlines pointer_list_s
@end

// -- pointer_tree_s --
@begin
inlines pointer_tree_s
@end

static inline int pointer_tree_s___compare_value(const pointer_tree_s *this,const pointer *a_first,const pointer *a_second)
{/*{{{*/
  (void)this;

  return *a_first < *a_second ? -1 : *a_first > *a_second ? 1 : 0;
}/*}}}*/

#endif

