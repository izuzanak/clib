
if(PKG_CONFIG_FOUND)
  pkg_search_module(CURSES "ncurses >= 6.0.0")
endif()

if(NOT CURSES_FOUND)
  find_package(Curses 6.0.0 QUIET)
endif()

if(NOT CURSES_FOUND)
  find_package(Curses CONFIG QUIET)
endif()

if(NOT CURSES_FOUND)
  find_path(CURSES_INCLUDE_DIR curses.h)
  find_library(CURSES_LIBRARY NAMES ncurses curses)
  if(CURSES_INCLUDE_DIR AND CURSES_LIBRARY)
    set(CURSES_FOUND TRUE)
    set(CURSES_INCLUDE_DIRS ${CURSES_INCLUDE_DIR})
    set(CURSES_LIBRARIES ${CURSES_LIBRARY})
  endif()
endif()

set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

if(CURSES_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
      ${CURSES_INCLUDE_DIRS})
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
      ${CURSES_LIBRARIES})
endif()

