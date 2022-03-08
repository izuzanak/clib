
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_opengl.h"
include "cl_glfw.h"
include "cl_ftgl.h"
@end

// === test execution functions ================================================

void prepare_shader_program(gl_program_s *a_program);

void test_shader();
void test_vertex_buffer();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

