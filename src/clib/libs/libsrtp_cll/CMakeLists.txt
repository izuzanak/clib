
if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBSRTP "libsrtp2 >= 2.2.0")
endif()

set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

if(LIBSRTP_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
      ${LIBSRTP_INCLUDE_DIRS})
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
      ${LIBSRTP_LIBRARIES})
endif()

