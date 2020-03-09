
@begin
include "cl_srtp.h"
@end

// === methods of structure srtp_policy_s ======================================

// === global functions ========================================================

void libsrtp_cll_init()
{/*{{{*/
  srtp_init();
}/*}}}*/

void libsrtp_cll_clear()
{/*{{{*/
  srtp_shutdown();
}/*}}}*/

