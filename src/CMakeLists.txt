
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

project(clib)

set(PROC_DIR "${CMAKE_CURRENT_BINARY_DIR}/proc_files")
set(TEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/tests")
file(MAKE_DIRECTORY "${PROC_DIR}" "${TEST_DIR}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()

# - process target source file -
function(process_source TARGET)
  execute_process(COMMAND process_c --deps "${SOURCE_DIR}/${TARGET}.c" ${COMP_HEADER_DIRS}
      OUTPUT_VARIABLE DEPS
      ERROR_QUIET)
  add_custom_command(OUTPUT "${PROC_DIR}/p.${TARGET}.c"
      DEPENDS ${DEPS}
      COMMAND process_c "--output" "${PROC_DIR}/p.${TARGET}.c" "${SOURCE_DIR}/${TARGET}.c" ${COMP_HEADER_DIRS})
endfunction()

# - process component -
function(process_component)
  set(PROC_DIR "${PROC_DIR}/${COMP_NAME}")
  file(MAKE_DIRECTORY ${PROC_DIR})

  set(PROC_SOURCE_FILES)
  foreach(SOURCE_DIR ${COMP_SOURCE_DIRS})
    set(SOURCE_FILES)
    aux_source_directory(${SOURCE_DIR} SOURCE_FILES)

    foreach(SOURCE_FILE ${SOURCE_FILES})
      get_filename_component(TARGET ${SOURCE_FILE} NAME_WE)
      process_source(${TARGET})

      list(APPEND PROC_SOURCE_FILES "${PROC_DIR}/p.${TARGET}.c")
    endforeach()
  endforeach()

  if(${COMP_NAME} MATCHES ${COMP_LIB_MATCH})
    add_library(${COMP_NAME} SHARED ${PROC_SOURCE_FILES})
    set_target_properties(${COMP_NAME} PROPERTIES PREFIX "")
  else()
    add_executable(${COMP_NAME} ${PROC_SOURCE_FILES})

    if(${COMP_NAME} MATCHES "_test$")
      set(TEST_DIR "${TEST_DIR}/${COMP_NAME}")
      file(MAKE_DIRECTORY "${TEST_DIR}")

      if(EXISTS "${COMP_DIR}/resources")
        file(COPY "${COMP_DIR}/resources" DESTINATION "${TEST_DIR}")
      endif()

      add_test("${COMP_NAME}_all" ${COMP_NAME} all)
    endif()
  endif()

  target_include_directories(${COMP_NAME} PUBLIC ${COMP_INCLUDE_DIRS})
  target_compile_definitions(${COMP_NAME} PUBLIC ${COMP_COMP_DEFS})
  target_link_libraries(${COMP_NAME} ${COMP_LINK_LIBS})

  if(EXISTS "${COMP_DIR}/subcomponents")
    set(PARENT_COMP_NAME "${COMP_NAME}_")
    string(REGEX REPLACE "^${CMAKE_CURRENT_SOURCE_DIR}/" "" SUBCOMPONENTS_DIR "${COMP_DIR}/subcomponents")
    process_components("${SUBCOMPONENTS_DIR}" "${COMP_LIB_MATCH}")
  endif()
endfunction()

# - process components -
function(process_components COMPS_DIR LIB_MATCH)
file(GLOB MODULE_DIRS "" "${CMAKE_CURRENT_SOURCE_DIR}/${COMPS_DIR}/*")
foreach(COMP_DIR ${MODULE_DIRS})
  get_filename_component(COMP_NAME ${COMP_DIR} NAME)
  set(COMP_NAME "${PARENT_COMP_NAME}${COMP_NAME}")

  set(CONF_DO_BUILD FALSE)
  set(COMP_DO_BUILD FALSE)

  if(EXISTS ${COMP_DIR}/CMakeLists.txt)
    include(${COMP_DIR}/CMakeLists.txt)

    if(CONF_DO_BUILD)
      set(COMP_DO_BUILD TRUE)
    endif()

    if(COMP_DO_BUILD)
      set(COMP_HEADER_DIRS "${COMP_DIR}/header_files" ${CONF_HEADER_DIRS})
      set(COMP_SOURCE_DIRS "${COMP_DIR}/source_files")
      set(COMP_INCLUDE_DIRS ${CONF_INCLUDE_DIRS})
      set(COMP_COMP_DEFS ${CONF_COMP_DEFS})
      set(COMP_LINK_LIBS ${CONF_LINK_LIBS})
      set(COMP_LIB_MATCH ${LIB_MATCH})

      process_component()
    endif()
  endif()
endforeach()
endfunction()

# - set global variables -
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(OS_LINUX TRUE)
  set(OS_WINDOWS FALSE)
  add_definitions(-g -Wall -Wfatal-errors -DLINUX)
  #add_definitions(-g -fsanitize=address -fsanitize=undefined -Wall -Wfatal-errors -DLINUX)
  #add_definitions(-O3 -DNDEBUG -Wall -Wfatal-errors -DLINUX)
else()
  set(OS_LINUX FALSE)
  set(OS_WINDOWS TRUE)
  add_definitions("/DWINDOWS /D_CRT_SECURE_NO_WARNINGS /wd4244")
endif()

# - process libraries -
process_components("clib/libs" "_cll$")

# - process tests -
process_components("clib/tests" "_cll$")

# - process programs -

