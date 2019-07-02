
find_library(QUIRC_LIBRARY quirc "${EXT_LIBS}/quirc/quirc-1.0")

set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

if(QUIRC_LIBRARY)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
      "${EXT_LIBS}/quirc/quirc-1.0/lib")
  set(CONF_COMP_DEFS)
  set(CONF_LINK_LIBS
      ${QUIRC_LIBRARY})
endif()

