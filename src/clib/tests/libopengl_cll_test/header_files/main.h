
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_image.h"
include "cl_glfw.h"
include "cl_opengl.h"
@end

// === test execution functions ================================================

void test_window();
void test_shader();
void test_texture();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

