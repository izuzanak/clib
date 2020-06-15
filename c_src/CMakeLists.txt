
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

project(clib)

set(CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(PROC_DIR "${CMAKE_CURRENT_BINARY_DIR}/proc_files")
set(TESTS_DIR "${CMAKE_CURRENT_BINARY_DIR}/tests")
file(MAKE_DIRECTORY "${PROC_DIR}" "${TESTS_DIR}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()

define_property(GLOBAL PROPERTY GP_INCLUDE_COMP_REL_DIRS
    BRIEF_DOCS "Global include components relative directories"
    FULL_DOCS "Global include components relative directories")

define_property(GLOBAL PROPERTY GP_INCLUDE_COMP_HEADER_DIRS
    BRIEF_DOCS "Global include components header directories"
    FULL_DOCS "Global include components header directories")

set_property(GLOBAL PROPERTY GP_INCLUDE_COMP_REL_DIRS)
set_property(GLOBAL PROPERTY GP_INCLUDE_COMP_HEADER_DIRS)

# - process target source file -
function(process_source SOURCE TARGET)
# {{{
  execute_process(COMMAND ${PROCESS_BIN} --output "${TARGET}" "${SOURCE}" ${COMP_HEADER_DIRS}
      OUTPUT_VARIABLE DEPS
      ERROR_QUIET)
# }}}
endfunction()

# - process component -
function(process_component)
# {{{
  foreach(SOURCE_DIR "header_files" "source_files")
    file(MAKE_DIRECTORY "${COMP_PROC_DIR}/${SOURCE_DIR}")
    file(GLOB SOURCE_FILES "" "${COMP_DIR}/${SOURCE_DIR}/*.[hc]")

    foreach(SOURCE_FILE ${SOURCE_FILES})
      get_filename_component(SOURCE_NAME ${SOURCE_FILE} NAME)
      process_source("${COMP_DIR}/${SOURCE_DIR}/${SOURCE_NAME}" "${COMP_PROC_DIR}/${SOURCE_DIR}/${SOURCE_NAME}")
    endforeach()
  endforeach()

  if(${COMP_NAME} MATCHES "_[a-z]*l$")
    # - ignore library -
  else()
    if(${COMP_NAME} MATCHES "_test$")
      if(EXISTS "${COMP_DIR}/resources")
        file(COPY "${COMP_DIR}/resources" DESTINATION "${COMP_PROC_DIR}")
      endif()
    endif()
  endif()

  if(EXISTS "${COMP_DIR}/subcomponents")
    set(PARENT_COMP_NAME "${COMP_NAME}_")
    string(REGEX REPLACE "^${CURRENT_SOURCE_DIR}/" "" SUBCOMPONENTS_DIR "${COMP_DIR}/subcomponents")
    include_component_dir("${SUBCOMPONENTS_DIR}")
  endif()
# }}}
endfunction()

# - include component -
function(include_component COMP_REL_DIR)
# {{{
  get_property(INCLUDE_COMP_REL_DIRS GLOBAL PROPERTY GP_INCLUDE_COMP_REL_DIRS)

  # - component was already included -
  list(FIND INCLUDE_COMP_REL_DIRS ${COMP_REL_DIR} ID_INDEX)
  if(NOT ${ID_INDEX} EQUAL "-1")
    return()
  endif()

  set(COMP_DIR "${CURRENT_SOURCE_DIR}/${COMP_REL_DIR}")
  get_filename_component(COMP_NAME ${COMP_DIR} NAME)
  set(COMP_NAME "${PARENT_COMP_NAME}${COMP_NAME}")
  set(PARENT_COMP_NAME)

  if(EXISTS ${COMP_DIR}/CMakeLists.txt)
    string(REPLACE "${CURRENT_SOURCE_DIR}" "${PROC_DIR}" COMP_PROC_DIR "${COMP_DIR}")

    # - copy component cmake file -
    file(COPY "${COMP_DIR}/CMakeLists.txt" DESTINATION "${COMP_PROC_DIR}")

    # - read component cmake file -
    set(CONF_DO_BUILD FALSE)
    include("${COMP_DIR}/CMakeLists.txt")

    # - append this component header directory -
    list(APPEND CONF_HEADER_DIRS "${COMP_DIR}/header_files")

    # - process component dependencies -
    foreach(COMP_COMP_REL_DIR ${CONF_COMPONENTS})
      include_component(${COMP_COMP_REL_DIR})

      get_property(INCLUDE_COMP_REL_DIRS GLOBAL PROPERTY GP_INCLUDE_COMP_REL_DIRS)
      get_property(INCLUDE_COMP_HEADER_DIRS GLOBAL PROPERTY GP_INCLUDE_COMP_HEADER_DIRS)

      list(FIND INCLUDE_COMP_REL_DIRS ${COMP_COMP_REL_DIR} ID_INDEX)

      if(${ID_INDEX} EQUAL "-1")
        message("WARNING: Component ${COMP_NAME} depends on unavailable component: ${COMP_COMP_REL_DIR}")
        return()
      endif()

      # - update component header directories -
      list(GET INCLUDE_COMP_HEADER_DIRS ${ID_INDEX} COMP_COMP_INCLUDE_DIRS_STR)
      string(REPLACE "," ";" COMP_COMP_INCLUDE_DIRS "${COMP_COMP_INCLUDE_DIRS_STR}")
      list(APPEND CONF_HEADER_DIRS ${COMP_COMP_INCLUDE_DIRS})

    endforeach()

    # - remove header directories duplicities -
    list(REMOVE_DUPLICATES CONF_HEADER_DIRS)
    string(REPLACE ";" "," CONF_HEADER_DIRS_STR "${CONF_HEADER_DIRS}")

    # FIXME debug output header directories
    #message("${COMP_NAME} -> ${CONF_HEADER_DIRS_STR}")

    set_property(GLOBAL APPEND PROPERTY GP_INCLUDE_COMP_REL_DIRS ${COMP_REL_DIR})
    set_property(GLOBAL APPEND PROPERTY GP_INCLUDE_COMP_HEADER_DIRS "${CONF_HEADER_DIRS_STR}")

    set(COMP_HEADER_DIRS ${CONF_HEADER_DIRS})

    process_component()
  endif()
# }}}
endfunction()

# - include component directory -
function(include_component_dir COMPS_DIR)
# {{{
file(GLOB MODULE_DIRS "" "${CURRENT_SOURCE_DIR}/${COMPS_DIR}/*")
foreach(COMP_DIR ${MODULE_DIRS})
  string(REPLACE "${CURRENT_SOURCE_DIR}/" "" COMP_DIR_WP "${COMP_DIR}")
  include_component(${COMP_DIR_WP})
endforeach()
# }}}
endfunction()

# - find process binary -
find_program(PROCESS_BIN cont_c)

if(NOT PROCESS_BIN)
  message("WARNING: Cannot found container generator")
  return()
endif()

# - process component directories -
if(NOT DEFINED CS_COMPONENT_DIRS)
  set(CS_COMPONENT_DIRS
      "src/clib/tests"
      "src/traced/tests"
      "src/traced/progs"
      "src/segmentd/tests"
      "src/segmentd/progs"
      "src/camsys/tests"
      "src/camsys/progs"
      "src/objectdb/tests"
      "src/objectdb/progs"
      "src/macs/tests"
      "src/macs/progs")
endif()

foreach(COMPONENT_DIR ${CS_COMPONENT_DIRS})
  include_component_dir("${COMPONENT_DIR}")
endforeach()

# - process components -
if(NOT DEFINED CS_COMPONENTS)
  set(CS_COMPONENTS)
endif()

foreach(COMPONENT ${CS_COMPONENTS})
  include_component("${COMPONENT}")
endforeach()

