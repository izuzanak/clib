
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_linux.h"
include "cl_fuse.h"
@end

// === test execution functions ================================================

void test_ino_size();
void test_create();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

