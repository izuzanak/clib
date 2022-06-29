
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_xsd.h"
@end

// === test execution functions ================================================

void test_xs_date();
void test_xs_time();
void test_xs_datetime();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

