
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_crypto.h"
@end

// === test execution functions ================================================

void test_random();
void test_base16();
void test_base64();
void test_pkey_load();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

