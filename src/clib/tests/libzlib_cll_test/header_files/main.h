
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_zlib.h"
@end

// === test execution functions ================================================

void test_compress();
void test_gz_file();
void test_gz_temp();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

