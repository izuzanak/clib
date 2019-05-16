
if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBSQLITE3 "sqlite3 >= 3.8.7.1")
endif()

if(LIBSQLITE3_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_COMPONENTS
    "src/clib/libs/libvar_cll")
  set(CONF_INCLUDE_DIRS
    ${LIBSQLITE3_INCLUDE_DIRS})
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
    ${LIBSQLITE3_LIBRARIES})
endif()

