
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_parser.h"
@end

// === definition of generated structures ======================================

// === inline methods of generated structures ==================================

// === test execution functions ================================================

void test_finite_automata_create();
void test_inverted_index_create();
void test_inverted_index_remove();
void test_inverted_index_tree();
void test_fa_states_recognize();
void test_fa_states_dot_code();
void test_parser_create();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

