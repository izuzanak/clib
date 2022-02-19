
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libopengl_cll_test";

const char *test_names[] =
{/*{{{*/
  "window",
  "shader",
  "texture",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_window,
  test_shader,
  test_texture,
};/*}}}*/

// === test execution functions ================================================

void test_window()
{/*{{{*/
  CONT_INIT_CLEAR(glfw_window_s,window);
  cassert(glfw_window_s_create(&window,640,480,"Test window") == 0);

  glfwMakeContextCurrent(window.window);

  //while (!glfwWindowShouldClose(window.window))
  {
    int width;
    int height;

    glfwGetFramebufferSize(window.window,&width,&height);
    glViewport(0,0,width,height);

    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window.window);
    glfwPollEvents();
  }
}/*}}}*/

void test_shader()
{/*{{{*/
  CONT_INIT_CLEAR(glfw_window_s,window);
  cassert(glfw_window_s_create(&window,640,480,"Test window") == 0);

  glfwMakeContextCurrent(window.window);

  // - read vertex shader -
  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libopengl_cll_test/resources/video.vert","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,data);
  cassert(file_s_read_close(&file,&data) == 0);
  bc_array_s_push(&data,'\0');

  string_s vertex_code = {data.used,data.data};

  CONT_INIT_CLEAR(gl_shader_s,vertex_sh);
  cassert(gl_shader_s_create(&vertex_sh,GL_VERTEX_SHADER,&vertex_code) == 0);

  // - read fragment shader -
  cassert(file_s_open(&file,"tests/libopengl_cll_test/resources/video.frag","r") == 0);

  data.used = 0;
  cassert(file_s_read_close(&file,&data) == 0);
  bc_array_s_push(&data,'\0');

  string_s fragment_code = {data.used,data.data};

  CONT_INIT_CLEAR(gl_shader_s,fragment_sh);
  cassert(gl_shader_s_create(&fragment_sh,GL_FRAGMENT_SHADER,&fragment_code) == 0);

  // - create program -
  CONT_INIT_CLEAR(gl_program_s,program);
  cassert(gl_program_s_create(&program,&vertex_sh,&fragment_sh) == 0);

  // - retrieve attributes -
  CONT_INIT_CLEAR(gl_attribute_tree_s,attributes);
  gl_program_s_active_attributes(&program,&attributes);

  // - retrieve uniforms -
  CONT_INIT_CLEAR(gl_uniform_tree_s,uniforms);
  gl_program_s_active_uniforms(&program,&uniforms);

  // - use programm -
  gl_program_s_use(&program);
}/*}}}*/

void test_texture()
{/*{{{*/
  CONT_INIT_CLEAR(glfw_window_s,window);
  cassert(glfw_window_s_create(&window,640,480,"Test window") == 0);

  glfwMakeContextCurrent(window.window);

  // - read texture image -
  CONT_INIT_CLEAR(image_s,image);
  cassert(image_s_read_png_file(&image,"tests/libopengl_cll_test/resources/texture.png") == 0);

  // - create texture -
  CONT_INIT_CLEAR(gl_texture_s,texture);
  cassert(gl_texture_s_create(&texture,GL_RGBA,image.width,image.height,image.image_data_ptr->data) == 0);

  // - reset texture data -
  cassert(gl_texture_s_reset(&texture) == 0);

  // - update texture data -
  cassert(gl_texture_s_create(&texture,GL_RGBA,image.width,image.height,image.image_data_ptr->data) == 0);
  cassert(gl_texture_s_update(&texture,image.image_data_ptr->data) == 0);

  // - bind texture -
  gl_texture_s_bind(&texture);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libglfw_cll_init(),libglfw_cll_clear());

  return 0;
}/*}}}*/

