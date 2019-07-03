
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_image.h"
@end

// === test execution functions ================================================

void test_create();
void test_create_data();
void test_png_format();
void test_jpeg_format();
void test_operations();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

