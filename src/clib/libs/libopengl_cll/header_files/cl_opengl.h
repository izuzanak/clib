
#ifndef CL_LIB_OPENGL_H
#define CL_LIB_OPENGL_H

@begin
include "cl_struct.h"
@end

#define GL_GLEXT_PROTOTYPES

#if defined(UCBOX_IMX6) || defined(ANDROID) || defined(NACL) || defined(EMSCRIPTEN)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(LINUX)
#include <GL/gl.h>
#include <GL/glext.h>
#endif

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libopengl_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libopengl_cll_EXPORTS
#define libopengl_cll_EXPORT __declspec(dllexport)
#else
#define libopengl_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_GL_SHADER_CANNOT_CREATE_SHADER 1
#define ERROR_GL_SHADER_CANNOT_COMPILE_SHADER 2
#define ERROR_GL_PROGRAM_CANNOT_CREATE_PROGRAM 3
#define ERROR_GL_PROGRAM_CANNOT_LINK_PROGRAM 4
#define ERROR_GL_TEXTURE_UNSUPPORTED_PIXEL_FORMAT 5
#define ERROR_GL_TEXTURE_INVALID_DIMENSIONS 6
#define ERROR_GL_TEXTURE_IMAGE_LOAD_ERROR 7
#define ERROR_GL_TEXTURE_IMAGE_CLEAR_ERROR 8

typedef struct gl_attribute_tree_s gl_attribute_tree_s;
typedef struct gl_uniform_tree_s gl_uniform_tree_s;

// === definition of structure gl_shader_s =====================================

typedef unsigned gl_shader_s;
@begin
define gl_shader_s dynamic
@end

static inline void gl_shader_s_init(gl_shader_s *this);
static inline void gl_shader_s_clear(gl_shader_s *this);
static inline void gl_shader_s_flush_all(gl_shader_s *this);
static inline void gl_shader_s_swap(gl_shader_s *this,gl_shader_s *a_second);
static inline void gl_shader_s_copy(const gl_shader_s *this,const gl_shader_s *a_src);
static inline int gl_shader_s_compare(const gl_shader_s *this,const gl_shader_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void gl_shader_s_to_string(const gl_shader_s *this,bc_array_s *a_trg);
#endif

WUR int gl_shader_s_create(gl_shader_s *this,int a_type,const string_s *a_source);

// === definition of structure gl_program_s ====================================

typedef unsigned gl_program_s;
@begin
define gl_program_s dynamic
@end

static inline void gl_program_s_init(gl_program_s *this);
static inline void gl_program_s_clear(gl_program_s *this);
static inline void gl_program_s_flush_all(gl_program_s *this);
static inline void gl_program_s_swap(gl_program_s *this,gl_program_s *a_second);
static inline void gl_program_s_copy(const gl_program_s *this,const gl_program_s *a_src);
static inline int gl_program_s_compare(const gl_program_s *this,const gl_program_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void gl_program_s_to_string(const gl_program_s *this,bc_array_s *a_trg);
#endif

WUR int gl_program_s_create(gl_program_s *this,
    const gl_shader_s *a_vertex_sh,const gl_shader_s *a_fragment_sh);
void gl_program_s_active_attributes(const gl_program_s *this,gl_attribute_tree_s *a_result);
void gl_program_s_active_uniforms(const gl_program_s *this,gl_uniform_tree_s *a_result);
static inline void gl_program_s_use(const gl_program_s *this);

// === definition of structure gl_texture_s ====================================

typedef struct gl_texture_s
{
  GLuint index;
  unsigned format;
  unsigned pixel_size;
  unsigned width;
  unsigned height;
}
gl_texture_s;

@begin
define gl_texture_s dynamic
@end

static inline void gl_texture_s_init(gl_texture_s *this);
static inline void gl_texture_s_clear(gl_texture_s *this);
static inline void gl_texture_s_flush_all(gl_texture_s *this);
static inline void gl_texture_s_swap(gl_texture_s *this,gl_texture_s *a_second);
static inline void gl_texture_s_copy(const gl_texture_s *this,const gl_texture_s *a_src);
static inline int gl_texture_s_compare(const gl_texture_s *this,const gl_texture_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void gl_texture_s_to_string(const gl_texture_s *this,bc_array_s *a_trg);
#endif

WUR int gl_texture_s_create(gl_texture_s *this,
    unsigned a_format,unsigned a_width,unsigned a_height,void *a_data);
WUR int gl_texture_s_update(gl_texture_s *this,void *a_data);
WUR int gl_texture_s_reset(gl_texture_s *this);
static inline void gl_texture_s_bind(gl_texture_s *this);

// === definition of generated structures ======================================

// -- gl_attribute_s --
@begin
struct
<
string_s:name
ui:index
bi:size
bi:type
>
gl_attribute_s;
@end

// -- gl_attribute_tree_s --
@begin
rb_tree<gl_attribute_s> gl_attribute_tree_s;
@end

// -- gl_uniform_s --
@begin
struct
<
string_s:name
ui:index
bi:size
bi:type
>
gl_uniform_s;
@end

// -- gl_uniform_tree_s --
@begin
rb_tree<gl_uniform_s> gl_uniform_tree_s;
@end

// === inline methods of structure gl_shader_s =================================

static inline void gl_shader_s_init(gl_shader_s *this)
{/*{{{*/
  *this = 0;
}/*}}}*/

static inline void gl_shader_s_clear(gl_shader_s *this)
{/*{{{*/
  if (*this != 0)
  {
    glDeleteShader(*this);
  }

  gl_shader_s_init(this);
}/*}}}*/

static inline void gl_shader_s_flush_all(gl_shader_s *this)
{/*{{{*/
}/*}}}*/

static inline void gl_shader_s_swap(gl_shader_s *this,gl_shader_s *a_second)
{/*{{{*/
  gl_shader_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void gl_shader_s_copy(const gl_shader_s *this,const gl_shader_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int gl_shader_s_compare(const gl_shader_s *this,const gl_shader_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void gl_shader_s_to_string(const gl_shader_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"gl_shader_s{%u}",*this);
}/*}}}*/
#endif

// === inline methods of structure gl_program_s ================================

static inline void gl_program_s_init(gl_program_s *this)
{/*{{{*/
  *this = 0;
}/*}}}*/

static inline void gl_program_s_clear(gl_program_s *this)
{/*{{{*/
  if (*this != 0)
  {
    glDeleteProgram(*this);
  }

  gl_program_s_init(this);
}/*}}}*/

static inline void gl_program_s_flush_all(gl_program_s *this)
{/*{{{*/
}/*}}}*/

static inline void gl_program_s_swap(gl_program_s *this,gl_program_s *a_second)
{/*{{{*/
  gl_program_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void gl_program_s_copy(const gl_program_s *this,const gl_program_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int gl_program_s_compare(const gl_program_s *this,const gl_program_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void gl_program_s_to_string(const gl_program_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"gl_program_s{%u}",*this);
}/*}}}*/
#endif

static inline void gl_program_s_use(const gl_program_s *this)
{/*{{{*/
  debug_assert(*this != 0);

  glUseProgram(*this);
}/*}}}*/

// === inline methods of structure gl_texture_s ================================

static inline void gl_texture_s_init(gl_texture_s *this)
{/*{{{*/
  this->index = 0;
}/*}}}*/

static inline void gl_texture_s_clear(gl_texture_s *this)
{/*{{{*/
  if (this->index != 0)
  {
    glDeleteTextures(1,&this->index);
  }

  gl_texture_s_init(this);
}/*}}}*/

static inline void gl_texture_s_flush_all(gl_texture_s *this)
{/*{{{*/
}/*}}}*/

static inline void gl_texture_s_swap(gl_texture_s *this,gl_texture_s *a_second)
{/*{{{*/
  gl_texture_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void gl_texture_s_copy(const gl_texture_s *this,const gl_texture_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int gl_texture_s_compare(const gl_texture_s *this,const gl_texture_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void gl_texture_s_to_string(const gl_texture_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"gl_texture_s{%u}",this->index);
}/*}}}*/
#endif

static inline void gl_texture_s_bind(gl_texture_s *this)
{/*{{{*/
  debug_assert(this->index != 0);

  glBindTexture(GL_TEXTURE_2D,this->index);
}/*}}}*/

// === inline methods of generated structures ==================================

// -- gl_attribute_s --
@begin
inlines gl_attribute_s
@end

// -- gl_attribute_tree_s --
@begin
inlines gl_attribute_tree_s
@end

static inline int gl_attribute_tree_s___compare_value(const gl_attribute_tree_s *this,const gl_attribute_s *a_first,const gl_attribute_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->name;
  const string_s *second = &a_second->name;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }
  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- gl_uniform_s --
@begin
inlines gl_uniform_s
@end

// -- gl_uniform_tree_s --
@begin
inlines gl_uniform_tree_s
@end

static inline int gl_uniform_tree_s___compare_value(const gl_uniform_tree_s *this,const gl_uniform_s *a_first,const gl_uniform_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->name;
  const string_s *second = &a_second->name;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }
  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

#endif

