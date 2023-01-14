
@begin
include "cl_pam.h"
@end

// === global functions ========================================================

#ifndef DISABLE_PAM_AUTH
int pam_conv_fun(int num_msg,const struct pam_message **msg,
    struct pam_response **resp,void *appdata_ptr)
{/*{{{*/
  (void)num_msg;
  (void)msg;

  *resp = *((struct pam_response **)appdata_ptr);

  return PAM_SUCCESS;
}/*}}}*/
#endif

int pam_user_auth(const char *a_service_name,
    const char *a_user,const char *a_pass)
{/*{{{*/
#ifndef DISABLE_PAM_AUTH
  struct pam_response *response;
  const struct pam_conv conversation = {pam_conv_fun,&response};

  pam_handle_t *handle = NULL;
  if (pam_start(a_service_name,a_user,&conversation,&handle) != PAM_SUCCESS)
  {
    throw_error(PAM_START_ERROR);
  }

  response = malloc(sizeof(struct pam_response));
  response->resp = strdup(a_pass);
  response->resp_retcode = 0;

  int auth_res = pam_authenticate(handle,0); // NOLINT

  if (pam_end(handle,auth_res) != PAM_SUCCESS)
  {
    throw_error(PAM_END_ERROR);
  }

  if (auth_res != PAM_SUCCESS)
  {
    throw_error(PAM_AUTHENTICATION_ERROR);
  }
#endif

  return 0;
}/*}}}*/

