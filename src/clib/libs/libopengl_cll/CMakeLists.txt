
set(OpenGL_GL_PREFERENCE "GLVND")

if(PKG_CONFIG_FOUND)
  pkg_search_module(OPENGL "gl")
endif()

if(NOT OPENGL_FOUND)
  find_package(OpenGL QUIET)
endif()

if(NOT OPENGL_FOUND)
  find_package(OpenGL CONFIG QUIET)
  if(OpenGL_FOUND)
    set(OPENGL_FOUND TRUE)
    if(TARGET OpenGL::GL)
      get_target_property(OPENGL_INCLUDE_DIR OpenGL::GL
          INTERFACE_INCLUDE_DIRECTORIES)
      set(OPENGL_LIBRARIES OpenGL::GL)
    endif()
  endif()
endif()

if(NOT OPENGL_FOUND)
  find_path(OPENGL_INCLUDE_DIR GL/gl.h)
  find_library(OPENGL_LIBRARY NAMES GL)
  if(OPENGL_INCLUDE_DIR AND OPENGL_LIBRARY)
    set(OPENGL_FOUND TRUE)
    set(OPENGL_LIBRARIES ${OPENGL_LIBRARY})
  endif()
endif()

if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBCGLM "cglm")
endif()

if(NOT LIBCGLM_FOUND)
  find_package(cglm CONFIG QUIET)
  if(cglm_FOUND)
    set(LIBCGLM_FOUND TRUE)
    if(TARGET cglm::cglm)
      get_target_property(LIBCGLM_INCLUDE_DIRS cglm::cglm
          INTERFACE_INCLUDE_DIRECTORIES)
      set(LIBCGLM_LIBRARIES cglm::cglm)
    endif()
  endif()
endif()

if(NOT LIBCGLM_FOUND)
  find_path(LIBCGLM_INCLUDE_DIR cglm/cglm.h)
  find_library(LIBCGLM_LIBRARY NAMES cglm)
  if(LIBCGLM_INCLUDE_DIR AND LIBCGLM_LIBRARY)
    set(LIBCGLM_FOUND TRUE)
    set(LIBCGLM_INCLUDE_DIRS ${LIBCGLM_INCLUDE_DIR})
    set(LIBCGLM_LIBRARIES ${LIBCGLM_LIBRARY})
  endif()
endif()

set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

if(OPENGL_FOUND AND LIBCGLM_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
      ${LIBCGLM_INCLUDE_DIRS}
      ${OPENGL_INCLUDE_DIR})
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
      ${LIBCGLM_LIBRARIES}
      ${OPENGL_LIBRARIES})
endif()

