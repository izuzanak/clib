
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libglfw_cll_test";

const char *test_names[] =
{/*{{{*/
  "window",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_window,
};/*}}}*/

// === test execution functions ================================================

void test_window()
{/*{{{*/
  CONT_INIT_CLEAR(glfw_window_s,window);
  cassert(glfw_window_s_create(&window,640,480,"Test window") == 0);

  glfwMakeContextCurrent(window.window);

  //while (!glfwWindowShouldClose(window.window))
  //{
  //  //glClear(GL_COLOR_BUFFER_BIT);
  //  glfwSwapBuffers(window.window);
  //  glfwPollEvents();
  //}
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libglfw_cll_init(),libglfw_cll_clear());

  return 0;
}/*}}}*/

