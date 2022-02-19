
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL)

if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBCGLM "cglm")
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

