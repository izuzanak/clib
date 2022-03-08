
@begin
include "cl_ftgl.h"
@end

// === methods of structure gl_vertex_buffer_s =================================

int gl_vertex_buffer_s_create(gl_vertex_buffer_s *this,const char *a_format)
{/*{{{*/
  gl_vertex_buffer_s_clear(this);

  vertex_buffer_t *buffer_ptr = vertex_buffer_new(a_format);

  // - ERROR -
  if (buffer_ptr == NULL)
  {
    throw_error(GL_VERTEX_BUFFER_CANNOT_CREATE_NEW);
  }

  // - retrieve vertex attributes -
  vertex_attribute_t **attrs = buffer_ptr->attributes;

  // - vertex item count and vertex data size -
  unsigned vert_item_cnt = 0;
  unsigned vert_size = 0;

  CONT_INIT_CLEAR(ui_array_s,vert_item_types);

  unsigned attr_idx = 0;
  do
  {
    if (attrs[attr_idx] == NULL)
    {
      break;
    }

    // - retrieve attribute -
    vertex_attribute_t *attr_ptr = attrs[attr_idx];

    // - retrieve attribute size -
    unsigned attr_size;
    switch (attr_ptr->type)
    {
      case GL_BOOL:           attr_size = sizeof(GLboolean); break;
      case GL_BYTE:           attr_size = sizeof(GLbyte); break;
      case GL_UNSIGNED_BYTE:  attr_size = sizeof(GLubyte); break;
      case GL_SHORT:          attr_size = sizeof(GLshort); break;
      case GL_UNSIGNED_SHORT: attr_size = sizeof(GLushort); break;
      case GL_INT:            attr_size = sizeof(GLint); break;
      case GL_UNSIGNED_INT:   attr_size = sizeof(GLuint); break;
      case GL_FLOAT:          attr_size = sizeof(GLfloat); break;
      default:
        cassert(0);
    }

    // - increase vertex item count -
    vert_item_cnt += attr_ptr->size;

    // - insert types to vertex item types -
    for (int c_idx = 0;c_idx < attr_ptr->size;++c_idx)
    {
      ui_array_s_push(&vert_item_types,attr_ptr->type);
    }

    // - increate vertex size -
    vert_size += attr_ptr->size*attr_size;

  } while(++attr_idx < MAX_VERTEX_ATTRIBUTE);

  // - store vertex attribute count -
  unsigned vert_attr_cnt = attr_idx;

  // - fill vertex buffer object -
  this->buffer_ptr = buffer_ptr;
  this->vert_attr_cnt = vert_attr_cnt;
  this->vert_item_cnt = vert_item_cnt;
  ui_array_s_swap(&this->vert_item_types,&vert_item_types);
  this->vert_size = vert_size;

  return 0;
}/*}}}*/

// === methods of generated structures =========================================

