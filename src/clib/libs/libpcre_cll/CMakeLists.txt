
set(PCRE_DEFS)

# - prefer PCRE2 posix wrapper (PCRE1 is end-of-life and dropped by newer distributions) -
if(PKG_CONFIG_FOUND)
  pkg_search_module(PCRE "libpcre2-posix")
  if(PCRE_FOUND)
    set(PCRE_DEFS "-DCLIB_PCRE2")
  endif()
endif()

if(NOT PCRE_FOUND)
  find_path(PCRE2_INCLUDE_DIR pcre2posix.h)
  find_library(PCRE2_LIBRARY NAMES pcre2-posix)
  if(PCRE2_INCLUDE_DIR AND PCRE2_LIBRARY)
    set(PCRE_FOUND TRUE)
    set(PCRE_INCLUDE_DIRS ${PCRE2_INCLUDE_DIR})
    set(PCRE_LIBRARIES ${PCRE2_LIBRARY})
    set(PCRE_DEFS "-DCLIB_PCRE2")
  endif()
endif()

# - fallback to legacy PCRE1 posix wrapper -
if(NOT PCRE_FOUND AND PKG_CONFIG_FOUND)
  pkg_search_module(PCRE "libpcreposix")
endif()

if(NOT PCRE_FOUND)
  find_package(PCRE CONFIG QUIET)
  if(PCRE_FOUND)
    if(TARGET PCRE::pcreposix)
      get_target_property(PCRE_INCLUDE_DIRS PCRE::pcreposix
          INTERFACE_INCLUDE_DIRECTORIES)
      set(PCRE_LIBRARIES PCRE::pcreposix)
    endif()
  endif()
endif()

if(NOT PCRE_FOUND)
  find_path(PCRE_INCLUDE_DIR pcreposix.h)
  find_library(PCRE_LIBRARY NAMES pcreposix)
  if(PCRE_INCLUDE_DIR AND PCRE_LIBRARY)
    set(PCRE_FOUND TRUE)
    set(PCRE_INCLUDE_DIRS ${PCRE_INCLUDE_DIR})
    set(PCRE_LIBRARIES ${PCRE_LIBRARY})
    if(OS_WINDOWS)
      set(PCRE_DEFS "-DPCRE_STATIC")
    endif()
  endif()
endif()

set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

if(PCRE_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
    ${PCRE_INCLUDE_DIRS})
  set(CONF_COMP_DEFS
    ${PCRE_DEFS})
  set(CONF_LINK_LIBS
    ${PCRE_LIBRARIES})
endif()

