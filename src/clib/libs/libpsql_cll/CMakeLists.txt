
set(CONF_COMPONENTS
  "src/clib/libs/libsys_cll"
  "src/clib/libs/liblinux_cll"
  "src/clib/libs/libvar_cll")
set(CONF_HEADER_DIRS)

if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBPQ "libpq >= 10.0")
endif()

if(LIBPQ_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
      ${LIBPQ_INCLUDE_DIRS})
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
      ${LIBPQ_LIBRARIES})
endif()

#set(CONF_DO_BUILD TRUE)
#set(CONF_INCLUDE_DIRS
#  "${EXT_LIBS}/postgresql-15-15.6/install/include")
#set(CONF_COMP_DEFS)
#set(CONF_LINK_LIBS
#  "${EXT_LIBS}/postgresql-15-15.6/install/lib/libpq.so")

