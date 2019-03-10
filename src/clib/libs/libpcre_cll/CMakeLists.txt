
set(PCRE_DEFS)

if(PKG_CONFIG_FOUND)
  pkg_search_module(PCRE "libpcreposix")
else()
  if(OS_WINDOWS AND PCRE_INCLUDE_DIRS AND PCRE_LIBRARIES)
    set(PCRE_FOUND TRUE)
    set(PCRE_DEFS "-DPCRE_STATIC")
  endif()
endif()

if(PCRE_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_HEADER_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/clib/libs/libbase_cll/header_files")
  set(CONF_INCLUDE_DIRS
    ${PCRE_INCLUDE_DIRS})
  set(CONF_COMP_DEFS
    ${PCRE_DEFS})
  set(CONF_LINK_LIBS
    libbase_cll
    ${PCRE_LIBRARIES})
endif()

