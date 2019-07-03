
find_package(PNG 1.2.50)
find_package(JPEG)

set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

if(PNG_FOUND AND JPEG_FOUND)
  set(CONF_DO_BUILD TRUE)
  set(CONF_INCLUDE_DIRS
    ${PNG_INCLUDE_DIRS}
    ${JPEG_INCLUDE_DIRS})
  set(CONF_COMP_DEFS
    ${PNG_DEFINITIONS})
  set(CONF_LINK_LIBS
    ${PNG_LIBRARIES}
    ${JPEG_LIBRARIES})
endif()

