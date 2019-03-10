
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_pcre.h"
@end

// === definition of generated structures ======================================

// -- pcre_array_s --
@begin
array<pcre_s> pcre_array_s;
@end

// === inline methods of generated structures ==================================

// -- pcre_array_s --
@begin
inlines pcre_array_s
@end

// === test execution functions ================================================

void test_pcre();
void test_pcre_split();
void test_pcre_replace();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

