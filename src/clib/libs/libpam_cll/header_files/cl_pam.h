
#ifndef CL_LIB_PAM_H
#define CL_LIB_PAM_H

@begin
include "cl_struct.h"
@end

#include <security/pam_appl.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libpam_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libpam_cll_EXPORTS
#define libpam_cll_EXPORT __declspec(dllexport)
#else
#define libpam_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_PAM_START_ERROR 1
#define ERROR_PAM_END_ERROR 2
#define ERROR_PAM_AUTHENTICATION_ERROR 3

// === definition of global functions ==========================================

WUR libpam_cll_EXPORT int pam_conv_fun(int num_msg,const struct pam_message **msg,
    struct pam_response **resp,void *appdata_ptr);
WUR libpam_cll_EXPORT int pam_user_auth(const char *a_service_name,
    const char *a_user,const char *a_pass);

#endif

