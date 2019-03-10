
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_regex.h"
@end

// === definition of generated structures ======================================

// -- regex_array_s --
@begin
array<regex_s> regex_array_s;
@end

// === inline methods of generated structures ==================================

// -- regex_array_s --
@begin
inlines regex_array_s
@end

// === test execution functions ================================================

void test_regex();
void test_regex_split();
void test_regex_replace();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

