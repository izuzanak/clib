
set(CONF_COMPONENTS
  "src/clib/libs/libvar_cll")
set(CONF_HEADER_DIRS)

find_package(LibXml2 2.9.1)

if(LIBXML2_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
    ${LIBXML2_INCLUDE_DIR})
  set(CONF_COMP_DEFS
    ${LIBXML2_DEFINITIONS})
  set(CONF_LINK_LIBS
    ${LIBXML2_LIBRARIES})
endif()

