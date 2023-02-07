
if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBFUSE "fuse3 >= 3.0.0")
endif()

if(FORCE_CLIB_LIBFUSE)
  set(LIBFUSE_FOUND TRUE)
  set(LIBFUSE_INCLUDE_DIRS "")
  set(LIBFUSE_LIBRARIES "fuse3;pthread")
endif()

set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

if(LIBFUSE_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
    ${LIBFUSE_INCLUDE_DIRS})
  set(CONF_COMP_DEFS
    "-DCYGFUSE -D_FILE_OFFSET_BITS=64")
  set(CONF_LINK_LIBS
    ${LIBFUSE_LIBRARIES})
endif()

