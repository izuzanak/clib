
set(CONF_COMPONENTS
  "src/clib/libs/libtime_cll"
  "src/clib/libs/libcrc_cll")

if (";${PROJECT_DEFS};" MATCHES ";-DCLIB_SEGMENTD_ISAL_CRC16;")
  list(APPEND CONF_COMPONENTS
    "src/clib/libs/libisal_cll")
endif()

list(APPEND CONF_COMPONENTS
  "src/clib/libs/liblinux_cll"
  "src/clib/libs/liblogger_cll"
  "src/segmentd/libs/libconf_sdl")
set(CONF_HEADER_DIRS)

set(CONF_DO_BUILD TRUE)
set(CONF_INCLUDE_DIRS)
set(CONF_COMP_DEFS)
set(CONF_LINK_LIBS)

