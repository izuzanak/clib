
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_rtsp.h"
@end

// === test execution functions ================================================

void test_rtsp_parse_range_time();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

