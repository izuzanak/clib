
if(OS_LINUX)
  set(LIBREGEX_FOUND TRUE)
endif()

if(LIBREGEX_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_COMPONENTS
    "src/clib/libs/libbase_cll")
  set(CONF_HEADER_DIRS)
  set(CONF_INCLUDE_DIRS)
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS)
endif()

