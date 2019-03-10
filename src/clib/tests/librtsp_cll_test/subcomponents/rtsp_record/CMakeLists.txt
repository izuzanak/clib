
if(TARGET librtsp_cll)
  set(CONF_DO_BUILD TRUE)
  set(CONF_HEADER_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/clib/libs/libtest_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/clib/libs/libbase_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/clib/libs/libsys_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/clib/libs/liblinux_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/clib/libs/libtime_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/clib/libs/librtsp_cll/header_files")
  set(CONF_INCLUDE_DIRS)
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
    libtest_cll
    libsys_cll
    librtsp_cll)
endif()

