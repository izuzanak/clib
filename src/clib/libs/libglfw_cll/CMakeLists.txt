
if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBGLFW "glfw3 >= 3.2.0")
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

