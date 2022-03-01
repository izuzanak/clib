
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_json.h"
include "cl_sys.h"
@end

// === definition of generated structures ======================================

// -- basic_tree_s --
@begin
rb_tree<ui>
options ( from_json )
basic_tree_s;
@end

// -- static_s --
@begin
struct
<
ui:first
ui:second
ui:third
>
options ( from_json )
static_s;
@end

// -- static_array_s --
@begin
array<static_s>
options ( from_json )
static_array_s;
@end

// -- static_queue_s --
@begin
queue<static_s>
options ( from_json )
static_queue_s;
@end

// -- static_list_s --
@begin
list<static_s>
options ( from_json )
static_list_s;
@end

// -- static_tree_s --
@begin
rb_tree<static_s>
options ( from_json )
static_tree_s;
@end

// -- dynamic_s --
@begin
struct
<
ui:first
ui:second
static_array_s:array
>
options ( from_json )
dynamic_s;
@end

// -- dynamic_array_s --
@begin
array<dynamic_s>
options ( from_json )
dynamic_array_s;
@end

// -- dynamic_queue_s --
@begin
queue<dynamic_s>
options ( from_json )
dynamic_queue_s;
@end

// -- dynamic_list_s --
@begin
list<dynamic_s>
options ( from_json )
dynamic_list_s;
@end

// -- dynamic_tree_s --
@begin
rb_tree<dynamic_s>
options ( from_json )
dynamic_tree_s;
@end

// === inline methods of generated structures ==================================

// -- basic_tree_s --
@begin
inlines basic_tree_s
@end

static inline int basic_tree_s___compare_value(const basic_tree_s *this,const ui *a_first,const ui *a_second)
{/*{{{*/
  (void)this;

  return *a_first < *a_second ? -1 : *a_first > *a_second ? 1 : 0;
}/*}}}*/

// -- static_s --
@begin
inlines static_s
@end

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

// -- dynamic_s --
@begin
inlines dynamic_s
@end

// -- dynamic_array_s --
@begin
inlines dynamic_array_s
@end

// -- dynamic_queue_s --
@begin
inlines dynamic_queue_s
@end

// -- dynamic_list_s --
@begin
inlines dynamic_list_s
@end

// -- dynamic_tree_s --
@begin
inlines dynamic_tree_s
@end

static inline int dynamic_tree_s___compare_value(const dynamic_tree_s *this,const dynamic_s *a_first,const dynamic_s *a_second)
{/*{{{*/
  (void)this;

  return a_first->first < a_second->first ? -1 : a_first->first > a_second->first ? 1 : 0;
}/*}}}*/

// === test execution functions ================================================

void test_json_parse();
void test_json_parse_comments();
void test_json_create();
void test_from_json();
void test_load_json();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

