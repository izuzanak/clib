
#ifndef CL_LIB_GLFW_H
#define CL_LIB_GLFW_H

@begin
include "cl_struct.h"
@end

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libglfw_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libglfw_cll_EXPORTS
#define libglfw_cll_EXPORT __declspec(dllexport)
#else
#define libglfw_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_GLFW_WINDOW_CREATE_ERROR 1

// === definition of structure glfw_window_s ===================================

typedef struct glfw_window_s
{
  GLFWwindow *window;
}
glfw_window_s;

@begin
define glfw_window_s dynamic
@end

static inline void glfw_window_s_init(glfw_window_s *this);
static inline void glfw_window_s_clear(glfw_window_s *this);
static inline void glfw_window_s_flush_all(glfw_window_s *this);
static inline void glfw_window_s_swap(glfw_window_s *this,glfw_window_s *a_second);
static inline void glfw_window_s_copy(const glfw_window_s *this,const glfw_window_s *a_src);
static inline int glfw_window_s_compare(const glfw_window_s *this,const glfw_window_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void glfw_window_s_to_string(const glfw_window_s *this,bc_array_s *a_trg);
#endif

WUR static inline int glfw_window_s_create(glfw_window_s *this,
    int a_width,int a_height,const char *a_title);

// === definition of generated structures ======================================

// === definition of global functions ==========================================

void libglfw_cll_init();
void libglfw_cll_clear();

// === inline methods of structure glfw_window_s ===============================

static inline void glfw_window_s_init(glfw_window_s *this)
{/*{{{*/
  this->window = NULL;
}/*}}}*/

static inline void glfw_window_s_clear(glfw_window_s *this)
{/*{{{*/
  if (this->window != NULL)
  {
    glfwDestroyWindow(this->window);
  }

  glfw_window_s_init(this);
}/*}}}*/

static inline void glfw_window_s_flush_all(glfw_window_s *this)
{/*{{{*/
}/*}}}*/

static inline void glfw_window_s_swap(glfw_window_s *this,glfw_window_s *a_second)
{/*{{{*/
  glfw_window_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void glfw_window_s_copy(const glfw_window_s *this,const glfw_window_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int glfw_window_s_compare(const glfw_window_s *this,const glfw_window_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void glfw_window_s_to_string(const glfw_window_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"glfw_window_s{%p}",this->window);
}/*}}}*/
#endif

static inline int glfw_window_s_create(glfw_window_s *this,
    int a_width,int a_height,const char *a_title)
{/*{{{*/
  glfw_window_s_clear(this);

  this->window = glfwCreateWindow(a_width,a_height,a_title,NULL,NULL);

  if (this->window == NULL)
  {
    throw_error(GLFW_WINDOW_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

// === inline methods of generated structures ==================================

#endif

