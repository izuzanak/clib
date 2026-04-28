
if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBGLFW "glfw3 >= 3.2.0")
endif()

if(NOT LIBGLFW_FOUND)
  find_package(glfw3 CONFIG QUIET)
  if(glfw3_FOUND)
    set(LIBGLFW_FOUND TRUE)
    if(TARGET glfw)
      get_target_property(LIBGLFW_INCLUDE_DIRS glfw
          INTERFACE_INCLUDE_DIRECTORIES)
      set(LIBGLFW_LIBRARIES glfw)
    endif()
  endif()
endif()

if(NOT LIBGLFW_FOUND)
  find_path(LIBGLFW_INCLUDE_DIR GLFW/glfw3.h)
  find_library(LIBGLFW_LIBRARY NAMES glfw glfw3)
  if(LIBGLFW_INCLUDE_DIR AND LIBGLFW_LIBRARY)
    set(LIBGLFW_FOUND TRUE)
    set(LIBGLFW_INCLUDE_DIRS ${LIBGLFW_INCLUDE_DIR})
    set(LIBGLFW_LIBRARIES ${LIBGLFW_LIBRARY})
  endif()
endif()

set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

if(LIBGLFW_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
      ${LIBGLFW_INCLUDE_DIRS})
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
      ${LIBGLFW_LIBRARIES})
endif()

