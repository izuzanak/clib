
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_crypto.h"
@end

// === test execution functions ================================================

void test_random();
void test_base16();
void test_base64();
void test_pkey();
void test_digest();
void test_cipher();
void test_seal();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

