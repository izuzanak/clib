
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
@end

// === definition of generated structures ======================================

// -- pollfd_tree_s --
@begin
rb_tree<pollfd_s> pollfd_tree_s;
@end

// -- file_array_s --
@begin
array<file_s> file_array_s;
@end

// -- pipe_array_s --
@begin
array<pipe_s> pipe_array_s;
@end

// === inline methods of generated structures ==================================

// -- pollfd_tree_s --
@begin
inlines pollfd_tree_s
@end

static inline int pollfd_tree_s___compare_value(const pollfd_tree_s *this,const pollfd_s *a_first,const pollfd_s *a_second)
{/*{{{*/
  (void)this;

  return a_first->fd < a_second->fd ? -1 : a_first->fd > a_second->fd ? 1 : 0;
}/*}}}*/

// -- file_array_s --
@begin
inlines file_array_s
@end

// -- pipe_array_s --
@begin
inlines pipe_array_s
@end

// === test execution functions ================================================

void test_pollfd();
void test_file();
void test_pipe();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

