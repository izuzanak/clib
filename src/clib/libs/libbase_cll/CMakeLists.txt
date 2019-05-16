
set(CONF_DO_BUILD TRUE)
set(CONF_COMPONENTS)
set(CONF_INCLUDE_DIRS)
set(CONF_COMP_DEFS)

if(OS_LINUX)
  set(CONF_LINK_LIBS
    -fsanitize=address
    -fsanitize=undefined
    m)
elseif(OS_WINDOWS)
  set(CONF_LINK_LIBS)
endif()


