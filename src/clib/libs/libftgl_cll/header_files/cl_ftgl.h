
#ifndef CL_LIB_FTGL_H
#define CL_LIB_FTGL_H

@begin
include "cl_struct.h"
include "freetype-gl.h"
include "vertex-buffer.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libftgl_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libftgl_cll_EXPORTS
#define libftgl_cll_EXPORT __declspec(dllexport)
#else
#define libftgl_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_GL_VERTEX_BUFFER_CANNOT_CREATE_NEW 1

// === definition of structure gl_vertex_buffer_s ==============================

typedef struct gl_vertex_buffer_s
{
  vertex_buffer_t *buffer_ptr;
  unsigned vert_attr_cnt;
  unsigned vert_item_cnt;
  ui_array_s vert_item_types;
  unsigned vert_size;
}
gl_vertex_buffer_s;

@begin
define gl_vertex_buffer_s dynamic
@end

static inline void gl_vertex_buffer_s_init(gl_vertex_buffer_s *this);
static inline void gl_vertex_buffer_s_clear(gl_vertex_buffer_s *this);
static inline void gl_vertex_buffer_s_flush_all(gl_vertex_buffer_s *this);
static inline void gl_vertex_buffer_s_swap(gl_vertex_buffer_s *this,gl_vertex_buffer_s *a_second);
static inline void gl_vertex_buffer_s_copy(const gl_vertex_buffer_s *this,const gl_vertex_buffer_s *a_src);
static inline int gl_vertex_buffer_s_compare(const gl_vertex_buffer_s *this,const gl_vertex_buffer_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void gl_vertex_buffer_s_to_string(const gl_vertex_buffer_s *this,bc_array_s *a_trg);
#endif

WUR int gl_vertex_buffer_s_create(gl_vertex_buffer_s *this,const char *a_format);
static inline void gl_vertex_buffer_s_format(gl_vertex_buffer_s *this,string_s *a_string);
static inline void gl_vertex_buffer_s_push_back(gl_vertex_buffer_s *this,
    const float *a_vertices,int a_vertex_cnt,const GLuint *a_indices,int a_index_cnt);
static inline void gl_vertex_buffer_s_render(gl_vertex_buffer_s *this,GLenum a_mode);

// === definition of generated structures ======================================

// === inline methods of structure gl_vertex_buffer_s ==========================

static inline void gl_vertex_buffer_s_init(gl_vertex_buffer_s *this)
{/*{{{*/
  this->buffer_ptr = NULL;
  this->vert_attr_cnt = 0;
  this->vert_item_cnt = 0;
  ui_array_s_init(&this->vert_item_types);
  this->vert_size = 0;
}/*}}}*/

static inline void gl_vertex_buffer_s_clear(gl_vertex_buffer_s *this)
{/*{{{*/
  if (this->buffer_ptr != NULL)
  {
    vertex_buffer_delete(this->buffer_ptr);
  }

  ui_array_s_clear(&this->vert_item_types);

  gl_vertex_buffer_s_init(this);
}/*}}}*/

static inline void gl_vertex_buffer_s_flush_all(gl_vertex_buffer_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

static inline void gl_vertex_buffer_s_swap(gl_vertex_buffer_s *this,gl_vertex_buffer_s *a_second)
{/*{{{*/
  gl_vertex_buffer_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void gl_vertex_buffer_s_copy(const gl_vertex_buffer_s *this,const gl_vertex_buffer_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int gl_vertex_buffer_s_compare(const gl_vertex_buffer_s *this,const gl_vertex_buffer_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void gl_vertex_buffer_s_to_string(const gl_vertex_buffer_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"gl_vertex_buffer_s{%p}",this->buffer_ptr);
}/*}}}*/
#endif

static inline void gl_vertex_buffer_s_format(gl_vertex_buffer_s *this,string_s *a_string)
{/*{{{*/
  const char *format = vertex_buffer_format(this->buffer_ptr);
  string_s_set_ptr(a_string,format);
}/*}}}*/

static inline void gl_vertex_buffer_s_push_back(gl_vertex_buffer_s *this,
    const float *a_vertices,int a_vertex_cnt,const GLuint *a_indices,int a_index_cnt)
{/*{{{*/
#if OPTION_BRUTAL_ASSERT == ENABLED
  // - retrieve item types array -
  ui_array_s *vert_item_types = &this->vert_item_types;

  // - actual item index -
  unsigned item_idx = 0;
  unsigned vert_item_cnt = this->vert_item_cnt;
  while (item_idx < vert_item_cnt)
  {
    brutal_assert(*ui_array_s_at(vert_item_types,item_idx) == GL_FLOAT);
    ++item_idx;
  }
#endif

  // - push data to vertex buffer -
  int result = vertex_buffer_push_back(this->buffer_ptr,
      a_vertices,a_vertex_cnt,a_indices,a_index_cnt);

  (void)result;
}/*}}}*/

static inline void gl_vertex_buffer_s_render(gl_vertex_buffer_s *this,GLenum a_mode)
{/*{{{*/
  debug_assert(this->buffer_ptr != NULL);

  vertex_buffer_render(this->buffer_ptr,a_mode);
}/*}}}*/

// === inline methods of generated structures ==================================

#endif

