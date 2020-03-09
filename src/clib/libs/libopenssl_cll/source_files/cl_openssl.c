
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

int ssl_conn_s_write(ssl_conn_s *this,const void *a_src,int a_size)
{/*{{{*/
  debug_assert(*this != NULL);

  if (a_size > 0)
  {
    int cnt;
    unsigned writed = 0;

    do
    {
      // - ERROR -
      if ((cnt = SSL_write(*this,(const char *)a_src + writed,a_size - writed)) <= 0)
      {
        throw_error(SSL_CONN_WRITE_ERROR);
      }
    }
    while((writed += cnt) < a_size);
  }

  return 0;
}/*}}}*/

int ssl_conn_s_read(ssl_conn_s *this,int a_fd,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(*this != NULL);

  const int c_buffer_add = 1024;

  int inq_cnt;
  int read_cnt;
  do
  {
    bc_array_s_reserve(a_trg,c_buffer_add);
    read_cnt = SSL_read(*this,a_trg->data + a_trg->used,c_buffer_add);

    // - ERROR -
    if (read_cnt <= 0)
    {
      throw_error(SSL_CONN_READ_ERROR);
    }

    a_trg->used += read_cnt;

    // - ERROR -
    if (ioctl(a_fd,TIOCINQ,&inq_cnt) == -1)
    {
      throw_error(SSL_CONN_READ_ERROR);
    }
  }
  while(inq_cnt > 0);

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

