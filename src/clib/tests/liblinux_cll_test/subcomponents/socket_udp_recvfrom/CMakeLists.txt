
if(TARGET liblinux_cll)
  set(CONF_DO_BUILD TRUE)
  set(CONF_HEADER_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libtest_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libbase_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libregex_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libsys_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/liblinux_cll/header_files")
  set(CONF_INCLUDE_DIRS)
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
    libtest_cll
    libregex_cll
    libsys_cll
    liblinux_cll)
endif()

