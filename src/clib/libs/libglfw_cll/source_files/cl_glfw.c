
@begin
include "cl_glfw.h"
@end

// === methods of structure glfw_window_s ======================================

// === methods of generated structures =========================================

// === global functions ========================================================

void libglfw_cll_init()
{/*{{{*/
  glfwInit();
}/*}}}*/

void libglfw_cll_clear()
{/*{{{*/
  glfwTerminate();
}/*}}}*/

