
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_crypto.h"
@end

// === definition of generated structures ======================================

// -- crypto_digest_info_array_s --
@begin
array<crypto_digest_info_s> crypto_digest_info_array_s;
@end

// === inline methods of generated structures ==================================

// -- crypto_digest_info_array_s --
@begin
inlines crypto_digest_info_array_s
@end

// === test execution functions ================================================

void test_random();
void test_base16();
void test_base64();
void test_pkey();
void test_digest_info();
void test_digest();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

