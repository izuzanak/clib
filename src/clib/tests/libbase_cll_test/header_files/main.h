
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_atomic.h"
include "cl_test.h"
@end

// === definition of generated structures ======================================

// -- static_s --
@begin
struct
<
ui:first
usi:second
ui:third
>
options ( to_json to_json_nice )
static_s;
@end

// -- atomic_array_s --
@begin
array<atomic_s> atomic_array_s;
@end

// -- ui_queue_s --
@begin
queue<ui> ui_queue_s;
@end

// -- ui_list_s --
@begin
safe_list<ui> ui_list_s;
@end

// -- ui_tree_s --
@begin
safe_rb_tree<ui> ui_tree_s;
@end

// -- static_array_s --
@begin
array<static_s>
options ( to_json to_json_nice )
static_array_s;
@end

// -- static_queue_s --
@begin
queue<static_s>
options ( to_json to_json_nice )
static_queue_s;
@end

// -- static_list_s --
@begin
list<static_s>
options ( to_json to_json_nice )
static_list_s;
@end

// -- static_tree_s --
@begin
rb_tree<static_s>
options ( to_json to_json_nice )
static_tree_s;
@end

// -- string_queue_s --
@begin
queue<string_s> string_queue_s;
@end

// -- string_list_s --
@begin
list<string_s> string_list_s;
@end

// -- basic_choice_s --
@begin
choice
<
bi:integer
bf:real
lli:longint
>
options ( to_json to_json_nice )
basic_choice_s;
@end

// -- dynamic_choice_s --
@begin
choice
<
bi:integer
string_s:string
ui_array_s:array
>
options ( to_json to_json_nice )
dynamic_choice_s;
@end

// === inline methods of generated structures ==================================

// -- static_s --
@begin
inlines static_s
@end

// -- atomic_array_s --
@begin
inlines atomic_array_s
@end

// -- ui_queue_s --
@begin
inlines ui_queue_s
@end

// -- ui_list_s --
@begin
inlines ui_list_s
@end

// -- ui_tree_s --
@begin
inlines ui_tree_s
@end

static inline int ui_tree_s___compare_value(const ui_tree_s *this,const ui *a_first,const ui *a_second)
{/*{{{*/
  (void)this;

  return *a_first < *a_second ? -1 : *a_first > *a_second ? 1 : 0;
}/*}}}*/

// -- static_array_s --
@begin
inlines static_array_s
@end

// -- static_queue_s --
@begin
inlines static_queue_s
@end

// -- static_list_s --
@begin
inlines static_list_s
@end

// -- static_tree_s --
@begin
inlines static_tree_s
@end

static inline int static_tree_s___compare_value(const static_tree_s *this,const static_s *a_first,const static_s *a_second)
{/*{{{*/
  (void)this;

  return a_first->first < a_second->first ? -1 : a_first->first > a_second->first ? 1 : 0;
}/*}}}*/

// -- string_queue_s --
@begin
inlines string_queue_s
@end

// -- string_list_s --
@begin
inlines string_list_s
@end

// -- basic_choice_s --
@begin
inlines basic_choice_s
@end

// -- dynamic_choice_s --
@begin
inlines dynamic_choice_s
@end

// === test execution functions ================================================

void test_bc_array();
void test_atomic();
void test_static();
void test_string();
void test_basic_type_array();
void test_static_type_array();
void test_dynamic_type_array();
void test_basic_type_queue();
void test_static_type_queue();
void test_dynamic_type_queue();
void test_basic_type_list();
void test_static_type_list();
void test_dynamic_type_list();
void test_basic_type_tree();
void test_static_type_tree();
void test_dynamic_type_tree();
void test_basic_type_choice();
void test_dynamic_type_choice();
void test_doc_examples();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

