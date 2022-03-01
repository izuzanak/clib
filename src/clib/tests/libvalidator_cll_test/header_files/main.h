
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_validator.h"
@end

// === test execution functions ================================================

void test_prop_type();
void test_prop_equal();
void test_prop_not_equal();
void test_prop_lesser();
void test_prop_greater();
void test_prop_lesser_equal();
void test_prop_greater_equal();
void test_prop_length_equal();
void test_prop_length_not_equal();
void test_prop_length_lesser();
void test_prop_length_greater();
void test_prop_length_lesser_equal();
void test_prop_length_greater_equal();
void test_prop_reference();
void test_prop_regex();
void test_prop_items();
void test_prop_opt_items();
void test_prop_all_items();
void test_prop_all_keys();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

