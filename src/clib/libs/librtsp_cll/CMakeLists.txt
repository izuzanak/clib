
set(CONF_COMPONENTS
  "src/clib/libs/libtime_cll"
  "src/clib/libs/liblinux_cll")
set(CONF_HEADER_DIRS)

if(OS_LINUX)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS)
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS)
endif()

