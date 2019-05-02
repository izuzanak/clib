
set(CONF_DO_BUILD TRUE)
set(CONF_HEADER_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libtest_cll/header_files"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libbase_cll/header_files"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libvar_cll/header_files"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libjson_cll/header_files")
set(CONF_INCLUDE_DIRS)
set(CONF_COMP_DEFS)
set(CONF_LINK_LIBS
  libtest_cll
  libjson_cll)

