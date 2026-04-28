
if(PKG_CONFIG_FOUND)
  pkg_search_module(ISAL "libisal")
  if(ISAL_FOUND)
    set(ISAL_LIBRARY ${ISAL_LIBRARIES})
  endif()
endif()

if(NOT ISAL_LIBRARY)
  find_package(ISAL CONFIG QUIET)
  if(ISAL_FOUND)
    if(TARGET ISAL::isal)
      set(ISAL_LIBRARY ISAL::isal)
    endif()
  endif()
endif()

if(NOT ISAL_LIBRARY)
  find_library(ISAL_LIBRARY NAMES isal)
endif()

set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

if(ISAL_LIBRARY)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS)
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
    ${ISAL_LIBRARY})
endif()

