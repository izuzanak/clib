
if(PKG_CONFIG_FOUND)
  pkg_search_module(LIBPAM "pam")
endif()

if(NOT LIBPAM_FOUND)
  find_package(PAM CONFIG QUIET)
  if(PAM_FOUND)
    set(LIBPAM_FOUND TRUE)
  endif()
endif()

if(NOT LIBPAM_FOUND AND OS_LINUX)
  find_path(PATH_PAM_APPL_H NAMES "security/pam_appl.h")
  if(PATH_PAM_APPL_H)
    set(LIBPAM_FOUND TRUE)
  endif()
endif()

set(CONF_COMPONENTS
  "src/clib/libs/libbase_cll")
set(CONF_HEADER_DIRS)

set(CONF_DO_BUILD TRUE)
set(CONF_INCLUDE_DIRS)
set(CONF_COMP_DEFS)

if (";${PROJECT_DEFS};" MATCHES ";-DDISABLE_PAM_AUTH;")
  set(CONF_LINK_LIBS)
else()
  if(LIBPAM_FOUND)
    set(CONF_LINK_LIBS
      pam)
  else()
    set(CONF_LINK_LIBS)
  endif()
endif()

