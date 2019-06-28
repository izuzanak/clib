
@begin
include "cl_crypto.h"
@end

// === global functions ========================================================

void libcrypto_cll_init()
{/*{{{*/
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
}/*}}}*/

void libcrypto_cll_clear()
{/*{{{*/
  EVP_cleanup();
  CRYPTO_cleanup_all_ex_data();
  ERR_free_strings();
}/*}}}*/

