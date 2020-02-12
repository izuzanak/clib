
@begin
include "cl_openssl.h"
@end

// === methods of structure ssl_context_s ======================================

// === methods of structure ssl_conn_s =========================================

int ssl_conn_s_create(ssl_conn_s *this,ssl_context_s *a_ctx,int a_fd)
{/*{{{*/
  ssl_conn_s_clear(this);

  *this = SSL_new(*a_ctx);

  // - ERROR -
  if (*this == NULL)
  {
    throw_error(SSL_CONN_CREATE_ERROR);
  }

  // - ERROR -
  if (SSL_set_fd(*this,a_fd) != 1)
  {
    ssl_conn_s_clear(this);

    throw_error(SSL_CONN_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

// === global functions ========================================================

void libopenssl_cll_init()
{/*{{{*/
  SSL_load_error_strings();
  OpenSSL_add_ssl_algorithms();
}/*}}}*/

void libopenssl_cll_clear()
{/*{{{*/
  EVP_cleanup();
}/*}}}*/

