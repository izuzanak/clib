
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_utf8proc.h"
@end

// === test execution functions ================================================

void test_version();
void test_utf8_to_lower();
void test_utf8_to_upper();
void test_utf8_map();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

