
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_pack.h"
@end

// === test execution functions ================================================

void test_pack_code();
void test_pack_decode();
void test_pack_roundtrip();
void test_pack_endian();
void test_pack_string();
void test_pack_count();
void test_pack_var();
void test_pack_calc_size();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

