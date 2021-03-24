
if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBSNAPPY "snappy >= 1.0.0")
endif()

if(FORCE_CLIB_LIBSNAPPY)
  set(LIBSNAPPY_FOUND TRUE)
  set(LIBSNAPPY_INCLUDE_DIRS "")
  set(LIBSNAPPY_LIBRARIES "snappy")
endif()

set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

if(LIBSNAPPY_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
      ${LIBSNAPPY_INCLUDE_DIRS})
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
      ${LIBSNAPPY_LIBRARIES})
endif()

