
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_time.h"
@end

// === definition of generated structures ======================================

// -- datetime_array_s --
@begin
array<datetime_s> datetime_array_s;
@end

// -- time_array_s --
@begin
array<time_s> time_array_s;
@end

// === inline methods of generated structures ==================================

// -- datetime_array_s --
@begin
inlines datetime_array_s
@end

// -- time_array_s --
@begin
inlines time_array_s
@end

// === test execution functions ================================================

void test_time();
void test_clock();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

