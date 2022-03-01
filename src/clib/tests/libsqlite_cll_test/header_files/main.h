
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_sqlite.h"
@end

// === definition of generated structures ======================================

// -- sqlite_array_s --
@begin
array<sqlite_s> sqlite_array_s;
@end

// === inline methods of generated structures ==================================

// -- sqlite_array_s --
@begin
inlines sqlite_array_s
@end

// === test execution functions ================================================

void test_sqlite();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

