
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libftgl_cll_test";

const char *test_names[] =
{/*{{{*/
  "shader",
  "vertex_buffer",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_shader,
  test_vertex_buffer,
};/*}}}*/

// === test execution functions ================================================

void prepare_shader_program(gl_program_s *a_program)
{/*{{{*/

  // - read vertex shader -
  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libftgl_cll_test/resources/video.vert","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,data);
  cassert(file_s_read_close(&file,&data) == 0);
  bc_array_s_push(&data,'\0');

  string_s vertex_code = {data.used,data.data};

  CONT_INIT_CLEAR(gl_shader_s,vertex_sh);
  cassert(gl_shader_s_create(&vertex_sh,GL_VERTEX_SHADER,&vertex_code) == 0);

  // - read fragment shader -
  cassert(file_s_open(&file,"tests/libftgl_cll_test/resources/video.frag","r") == 0);

  data.used = 0;
  cassert(file_s_read_close(&file,&data) == 0);
  bc_array_s_push(&data,'\0');

  string_s fragment_code = {data.used,data.data};

  CONT_INIT_CLEAR(gl_shader_s,fragment_sh);
  cassert(gl_shader_s_create(&fragment_sh,GL_FRAGMENT_SHADER,&fragment_code) == 0);

  // - create program -
  cassert(gl_program_s_create(a_program,&vertex_sh,&fragment_sh) == 0);
}/*}}}*/

void test_shader()
{/*{{{*/
  CONT_INIT_CLEAR(glfw_window_s,window);
  cassert(glfw_window_s_create(&window,640,480,"Test window") == 0);

  glfwMakeContextCurrent(window.window);

  // - read vertex shader -
  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libftgl_cll_test/resources/video.vert","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,data);
  cassert(file_s_read_close(&file,&data) == 0);
  bc_array_s_push(&data,'\0');

  string_s vertex_code = {data.used,data.data};

  CONT_INIT_CLEAR(gl_shader_s,vertex_sh);
  cassert(gl_shader_s_create(&vertex_sh,GL_VERTEX_SHADER,&vertex_code) == 0);

  // - read fragment shader -
  cassert(file_s_open(&file,"tests/libftgl_cll_test/resources/video.frag","r") == 0);

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

void test_vertex_buffer()
{/*{{{*/
  int width = 640;
  int height = 480;

  CONT_INIT_CLEAR(glfw_window_s,window);
  cassert(glfw_window_s_create(&window,width,height,"Test window") == 0);

  glfwMakeContextCurrent(window.window);

  // - prepare shader program -
  CONT_INIT_CLEAR(gl_program_s,shader_program);
  prepare_shader_program(&shader_program);
  gl_program_s_use(&shader_program);

  // - retrieve attributes -
  CONT_INIT_CLEAR(gl_attribute_tree_s,attributes);
  gl_program_s_active_attributes(&shader_program,&attributes);

  // - retrieve uniforms -
  CONT_INIT_CLEAR(gl_uniform_tree_s,uniforms);
  gl_program_s_active_uniforms(&shader_program,&uniforms);

  // - prepare vertex buffer -
  CONT_INIT_CLEAR(gl_vertex_buffer_s,vertex_buffer);
  cassert(gl_vertex_buffer_s_create(&vertex_buffer,
        "vertex:3f,tex_coord:2f,color:4f") == 0);

  CONT_INIT_CLEAR(string_s,format);
  gl_vertex_buffer_s_format(&vertex_buffer,&format);
  fprintf(stderr,"format: %s\n",format.data);

  float xmax = (float)width/height;

  float pt0[] = {-xmax,-1};
  float pt1[] = {xmax,1};

  float vertices[] =
  {
    pt0[0],pt0[1],0, 0,1, 1,0,0,1,
    pt0[0],pt1[1],0, 0,0, 1,0,0,1,
    pt1[0],pt1[1],0, 1,0, 1,0,0,1,
    pt1[0],pt0[1],0, 1,1, 1,0,0,1
  };

  GLuint indices[] =
  {
    0,1,2,
    0,2,3
  };

  gl_vertex_buffer_s_push_back(&vertex_buffer,vertices,4,indices,6);

  glfwGetFramebufferSize(window.window,&width,&height);
  glViewport(0,0,width,height);

  // - compute field of view and camera distance -
  float fov = M_PI/3.0;
  float distance = 2.0/(2.0*tan(fov/2.0));

  mat4 projection;
  glm_perspective(fov,(float)width/height,1.0,10.0,projection);

  mat4 view;
  glm_lookat(
    (vec3){0.0,0.0,distance},
    (vec3){0.0,0.0,0.0},
    (vec3){0.0,1.0,0.0},
    view);

  // FIXME TODO model
  // FIXME TODO view
  gl_uniform_s_write_mat4(gl_uniform_tree_s_get(&uniforms,"projection"),projection);

  while (!glfwWindowShouldClose(window.window))
  {
    glClear(GL_COLOR_BUFFER_BIT);

    gl_vertex_buffer_s_render(&vertex_buffer,GL_TRIANGLES);

    glfwSwapBuffers(window.window);
    glfwPollEvents();
  }
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libglfw_cll_init(),libglfw_cll_clear());

  return 0;
}/*}}}*/

