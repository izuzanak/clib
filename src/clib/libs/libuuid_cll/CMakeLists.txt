
set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBUUID "uuid >= 2.33.0")
endif()

if(LIBUUID_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
    ${LIBUUID_INCLUDE_DIRS})
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
    ${LIBUUID_LIBRARIES})
endif()

