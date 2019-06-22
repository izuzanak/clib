
find_package(CURL 7.38.0)

if(OS_WINDOWS)
  set(WSOCK32_LIBS
    wsock32
    ws2_32)
endif()

set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

if(CURL_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
      ${CURL_INCLUDE_DIRS})
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
      ${CURL_LIBRARIES}
      ${WSOCK32_LIBS})
endif()

