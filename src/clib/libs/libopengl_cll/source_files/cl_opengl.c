
@begin
include "cl_opengl.h"
@end

// === methods of structure gl_shader_s ========================================

int gl_shader_s_create(gl_shader_s *this,int a_type,const string_s *a_source)
{/*{{{*/
  gl_shader_s_clear(this);

  unsigned shader = glCreateShader(a_type);

  // - ERROR -
  if (shader == 0)
  {
    throw_error(GL_SHADER_CANNOT_CREATE_SHADER);
  }

  // - set source code of shader object -
  int length = a_source->size - 1;
  glShaderSource(shader,1,(const GLchar **)&a_source->data,&length);

  // - compile shader object -
  glCompileShader(shader);

  // - retrieve shader compile status -
  GLint compile_status;
  glGetShaderiv(shader,GL_COMPILE_STATUS,&compile_status);

  // - ERROR -
  if (compile_status != GL_TRUE)
  {
#ifndef NDEBUG
    // - retrieve error log size -
    GLint log_size;
    glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&log_size);

    CONT_INIT_CLEAR(string_s,err_string);

    if (log_size != 0)
    {
      string_s_create(&err_string,log_size - 1);

      // - retrieve error log -
      glGetShaderInfoLog(shader,log_size,NULL,err_string.data);
    }

    debug_message_1(fprintf(stderr,"SHADER COMPILE ERROR: %s",err_string.data));
#endif

    // - delete shader object -
    glDeleteShader(shader);

    throw_error(GL_SHADER_CANNOT_COMPILE_SHADER);
  }

  *this = shader;

  return 0;
}/*}}}*/

// === methods of structure gl_program_s =======================================

int gl_program_s_create(gl_program_s *this,
    const gl_shader_s *a_vertex_sh,const gl_shader_s *a_fragment_sh)
{/*{{{*/

  // - create shader program -
  long long int program = glCreateProgram();

  // - ERROR -
  if (program == 0)
  {
    throw_error(GL_PROGRAM_CANNOT_CREATE_PROGRAM);
  }

  // - attach shaders to program -
  glAttachShader(program,*a_vertex_sh);
  glAttachShader(program,*a_fragment_sh);

  // - link shader program -
  glLinkProgram(program);

  // - retrieve program link status -
  GLint link_status;
  glGetProgramiv(program,GL_LINK_STATUS,&link_status);

  // - ERROR -
  if (link_status != GL_TRUE)
  {
#ifndef NDEBUG
    // - retrieve error log size -
    GLint log_size;
    glGetProgramiv(program,GL_INFO_LOG_LENGTH,&log_size);

    CONT_INIT_CLEAR(string_s,err_string);

    if (log_size != 0)
    {
      string_s_create(&err_string,log_size - 1);

      // - retrieve error log -
      glGetProgramInfoLog(program,log_size,NULL,err_string.data);
    }

    debug_message_1(fprintf(stderr,"PROGRAM LINK ERROR: %s",err_string.data));
#endif

    // - delete shader program -
    glDeleteProgram(program);

    throw_error(GL_PROGRAM_CANNOT_LINK_PROGRAM);
  }

  *this = program;

  return 0;
}/*}}}*/

void gl_program_s_active_attributes(const gl_program_s *this,
    gl_attribute_tree_s *a_result)
{/*{{{*/
  gl_attribute_tree_s_clear(a_result);

  GLint attr_cnt;
  glGetProgramiv(*this,GL_ACTIVE_ATTRIBUTES,&attr_cnt);

  GLint buff_size;
  glGetProgramiv(*this,GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,&buff_size);

  char name_buff[buff_size];

  if (attr_cnt > 0)
  {
    GLint attr_idx = 0;
    do
    {
      GLsizei length;
      GLint size;
      GLenum type;

      glGetActiveAttrib(*this,attr_idx,buff_size,&length,&size,&type,name_buff);

      gl_attribute_s attribute = {{length + 1,name_buff},attr_idx,size,type};
      gl_attribute_tree_s_insert(a_result,&attribute);

    } while(++attr_idx < attr_cnt);
  }
}/*}}}*/

void gl_program_s_active_uniforms(const gl_program_s *this,
    gl_uniform_tree_s *a_result)
{/*{{{*/
  gl_uniform_tree_s_clear(a_result);

  GLint uni_cnt;
  glGetProgramiv(*this,GL_ACTIVE_UNIFORMS,&uni_cnt);

  GLint buff_size;
  glGetProgramiv(*this,GL_ACTIVE_UNIFORM_MAX_LENGTH,&buff_size);

  char name_buff[buff_size];

  if (uni_cnt > 0)
  {
    GLint uni_idx = 0;
    do {
      GLsizei length;
      GLint size;
      GLenum type;

      glGetActiveUniform(*this,uni_idx,buff_size,&length,&size,&type,name_buff);

      gl_uniform_s uniform = {{length + 1,name_buff},uni_idx,size,type};
      gl_uniform_tree_s_insert(a_result,&uniform);

    } while(++uni_idx < uni_cnt);
  }
}/*}}}*/

// === methods of structure gl_texture_s =======================================

int gl_texture_s_create(gl_texture_s *this,
    unsigned a_format,unsigned a_width,unsigned a_height,void *a_data)
{/*{{{*/
  gl_texture_s_clear(this);

  unsigned pixel_size;
  switch (a_format)
  {
  case GL_RGB:
    pixel_size = 3;
    break;

  case GL_RGBA:
    pixel_size = 4;
    break;

  // - ERROR -
  default:
    
    throw_error(GL_TEXTURE_UNSUPPORTED_PIXEL_FORMAT);
  }

  GLint mt_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE,&mt_size);

  // - ERROR -
  if (a_width <= 0 || a_width > (unsigned)mt_size || a_height <= 0 || a_height > (unsigned)mt_size)
  {
    throw_error(GL_TEXTURE_INVALID_DIMENSIONS);
  }

  // - create gl texture object -
  glGenTextures(1,&this->index);
  glBindTexture(GL_TEXTURE_2D,this->index);

  // - set default texture parameters -
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D,0,a_format,a_width,a_height,0,a_format,GL_UNSIGNED_BYTE,a_data);

  // - ERROR -
  if (glGetError() != GL_NO_ERROR)
  {
    glBindTexture(GL_TEXTURE_2D,0);
    glDeleteTextures(1,&this->index);

    throw_error(GL_TEXTURE_IMAGE_LOAD_ERROR);
  }

  this->format = a_format;
  this->pixel_size = pixel_size;
  this->width = a_width;
  this->height = a_height;

  return 0;
}/*}}}*/

int gl_texture_s_update(gl_texture_s *this,void *a_data)
{/*{{{*/
  glBindTexture(GL_TEXTURE_2D,this->index);
  glTexSubImage2D(GL_TEXTURE_2D,0,0,0,this->width,this->height,this->format,GL_UNSIGNED_BYTE,a_data);

  // - ERROR -
  if (glGetError() != GL_NO_ERROR)
  {
    throw_error(GL_TEXTURE_IMAGE_LOAD_ERROR);
  }

  return 0;
}/*}}}*/

int gl_texture_s_reset(gl_texture_s *this)
{/*{{{*/
  glBindTexture(GL_TEXTURE_2D,this->index);
  glTexImage2D(GL_TEXTURE_2D,0,this->format,0,0,0,this->format,GL_UNSIGNED_BYTE,NULL);

  // - ERROR -
  if (glGetError() != GL_NO_ERROR)
  {
    throw_error(GL_TEXTURE_IMAGE_CLEAR_ERROR);
  }

  return 0;
}/*}}}*/

// === methods of generated structures =========================================

// -- gl_attribute_s --
@begin
methods gl_attribute_s
@end

// -- gl_attribute_tree_s --
@begin
methods gl_attribute_tree_s
@end

// -- gl_uniform_s --
@begin
methods gl_uniform_s
@end

// -- gl_uniform_tree_s --
@begin
methods gl_uniform_tree_s
@end

