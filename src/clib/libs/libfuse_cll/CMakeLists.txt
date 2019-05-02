
if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBFUSE "fuse3 >= 3.0.0")
endif()

if(LIBFUSE_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_HEADER_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libbase_cll/header_files")
  set(CONF_INCLUDE_DIRS
    ${LIBFUSE_INCLUDE_DIRS})
  set(CONF_COMP_DEFS
    "-D_FILE_OFFSET_BITS=64 ")
  set(CONF_LINK_LIBS
    libvar_cll
    ${LIBFUSE_LIBRARIES})
endif()

