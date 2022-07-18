
set(CONF_COMPONENTS
  "src/clib/libs/libtest_cll"
  "src/clib/libs/libsys_cll"
  "src/clib/libs/libopengl_cll"
  "src/clib/libs/libglfw_cll"
  "src/clib/libs/libftgl_cll")
set(CONF_HEADER_DIRS)

if(DEFINED ENV{DISPLAY})
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS)
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS)
endif()

