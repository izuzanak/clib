
find_package(LibXml2 2.9.1)

if(LIBXML2_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_HEADER_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libbase_cll/header_files"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/clib/libs/libvar_cll/header_files")
  set(CONF_INCLUDE_DIRS
    ${LIBXML2_INCLUDE_DIR})
  set(CONF_COMP_DEFS
    ${LIBXML2_DEFINITIONS})
  set(CONF_LINK_LIBS
    libvar_cll
    ${LIBXML2_LIBRARIES})
endif()

