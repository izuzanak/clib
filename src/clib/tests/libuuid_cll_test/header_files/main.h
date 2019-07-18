
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_uuid.h"
@end

// === definition of generated structures ======================================

// -- uuid_array_s --
@begin
array<uuid_s> uuid_array_s;
@end

// === inline methods of generated structures ==================================

// -- uuid_array_s --
@begin
inlines uuid_array_s
@end

// === test execution functions ================================================

void test_create();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

