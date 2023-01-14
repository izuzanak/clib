
set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

set(CONF_DO_BUILD TRUE)
set(CONF_INCLUDE_DIRS)
set(CONF_COMP_DEFS)

if (";${PROJECT_DEFS};" MATCHES ";-DDISABLE_PAM_AUTH;")
  set(CONF_LINK_LIBS)
else()
  set(CONF_LINK_LIBS
    pam)
endif()

