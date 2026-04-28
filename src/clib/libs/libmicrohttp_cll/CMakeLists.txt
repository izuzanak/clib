
if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBMICROHTTPD "libmicrohttpd >= 0.9.37")
endif()

if(NOT LIBMICROHTTPD_FOUND)
  find_package(MHD CONFIG QUIET)
  if(MHD_FOUND)
    set(LIBMICROHTTPD_FOUND TRUE)
    if(TARGET MHD::microhttpd)
      get_target_property(LIBMICROHTTPD_INCLUDE_DIRS MHD::microhttpd
          INTERFACE_INCLUDE_DIRECTORIES)
      set(LIBMICROHTTPD_LIBRARIES MHD::microhttpd)
    endif()
  endif()
endif()

if(NOT LIBMICROHTTPD_FOUND)
  find_path(LIBMICROHTTPD_INCLUDE_DIR microhttpd.h)
  find_library(LIBMICROHTTPD_LIBRARY NAMES microhttpd)
  if(LIBMICROHTTPD_INCLUDE_DIR AND LIBMICROHTTPD_LIBRARY)
    set(LIBMICROHTTPD_FOUND TRUE)
    set(LIBMICROHTTPD_INCLUDE_DIRS ${LIBMICROHTTPD_INCLUDE_DIR})
    set(LIBMICROHTTPD_LIBRARIES ${LIBMICROHTTPD_LIBRARY})
  endif()
endif()

if(OS_WINDOWS)
  set(WSOCK32_LIBS
    wsock32
    ws2_32)
endif()

set(CONF_COMPONENTS
  "src/clib/libs/libsys_cll"
  "src/clib/libs/liblinux_cll")
set(CONF_HEADER_DIRS)

if(LIBMICROHTTPD_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
      ${LIBMICROHTTPD_INCLUDE_DIRS})
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
      ${LIBMICROHTTPD_LIBRARIES}
      ${WSOCK32_LIBS})
endif()

