
if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBSQLITE3 "sqlite3 >= 3.8.7.1")
endif()

if(LIBSQLITE3_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_HEADER_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libbase_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libvar_cll/header_files")
  set(CONF_INCLUDE_DIRS
    ${LIBSQLITE3_INCLUDE_DIRS})
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
    libvar_cll
    ${LIBSQLITE3_LIBRARIES})
endif()

