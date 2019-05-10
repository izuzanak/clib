
if(OS_LINUX)
  set(CONF_DO_BUILD TRUE)
  set(CONF_HEADER_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libbase_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libtime_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/liblinux_cll/header_files")
  set(CONF_INCLUDE_DIRS)
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
    libtime_cll
    liblinux_cll)
endif()

