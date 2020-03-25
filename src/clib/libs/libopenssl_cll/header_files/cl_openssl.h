
#ifndef __CL_OPENSSL_H
#define __CL_OPENSSL_H

@begin
include "cl_struct.h"
@end

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <sys/ioctl.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libopenssl_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libopenssl_cll_EXPORTS
#define libopenssl_cll_EXPORT __declspec(dllexport)
#else
#define libopenssl_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_SSL_CONTEXT_CREATE_ERROR 1
#define ERROR_SSL_CONTEXT_CERTIFICATE_FILE_ERROR 2
#define ERROR_SSL_CONTEXT_PRIVATE_KEY_FILE_ERROR 3

#define ERROR_SSL_CONN_CREATE_ERROR 1
#define ERROR_SSL_CONN_WRITE_ERROR 2
#define ERROR_SSL_CONN_READ_ERROR 3

// - ssl repeated actions -
enum
{
  SSL_ACTION_NONE = 0,
  SSL_ACTION_SEND_MSG,
  SSL_ACTION_RECV_MSG
};

// === definition of structure ssl_context_s ===================================

typedef SSL_CTX * ssl_context_s;
@begin
define ssl_context_s dynamic
@end

static inline void ssl_context_s_init(ssl_context_s *this);
static inline void ssl_context_s_clear(ssl_context_s *this);
static inline void ssl_context_s_flush_all(ssl_context_s *this);
static inline void ssl_context_s_swap(ssl_context_s *this,ssl_context_s *a_second);
static inline void ssl_context_s_copy(const ssl_context_s *this,const ssl_context_s *a_src);
static inline int ssl_context_s_compare(const ssl_context_s *this,const ssl_context_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void ssl_context_s_to_string(const ssl_context_s *this,bc_array_s *a_trg);
#endif

WUR static inline int ssl_context_s_create_server(ssl_context_s *this);
WUR static inline int ssl_context_s_create_client(ssl_context_s *this);
WUR static inline int ssl_context_s_use_certificate_file(ssl_context_s *this,
    const char *a_file_name,int a_file_type);
WUR static inline int ssl_context_s_use_private_key_file(ssl_context_s *this,
    const char *a_file_name,int a_file_type);

// === definition of structure ssl_conn_s ======================================

typedef SSL * ssl_conn_s;
@begin
define ssl_conn_s dynamic
@end

static inline void ssl_conn_s_init(ssl_conn_s *this);
static inline void ssl_conn_s_clear(ssl_conn_s *this);
static inline void ssl_conn_s_flush_all(ssl_conn_s *this);
static inline void ssl_conn_s_swap(ssl_conn_s *this,ssl_conn_s *a_second);
static inline void ssl_conn_s_copy(const ssl_conn_s *this,const ssl_conn_s *a_src);
static inline int ssl_conn_s_compare(const ssl_conn_s *this,const ssl_conn_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void ssl_conn_s_to_string(const ssl_conn_s *this,bc_array_s *a_trg);
#endif

WUR libopenssl_cll_EXPORT int ssl_conn_s_create(ssl_conn_s *this,
    ssl_context_s *a_ctx,int a_fd);
static inline void ssl_conn_s_set_accept_state(const ssl_conn_s *this);
static inline void ssl_conn_s_set_connect_state(const ssl_conn_s *this);
WUR libopenssl_cll_EXPORT int ssl_conn_s_write(ssl_conn_s *this,const void *a_src,int a_size);
WUR libopenssl_cll_EXPORT int ssl_conn_s_read(ssl_conn_s *this,int a_fd,bc_array_s *a_trg);

// === definition of global functions ==========================================

void libopenssl_cll_init();
void libopenssl_cll_clear();

// === inline methods of structure ssl_context_s ===============================

static inline void ssl_context_s_init(ssl_context_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void ssl_context_s_clear(ssl_context_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    SSL_CTX_free(*this);
  }

  ssl_context_s_init(this);
}/*}}}*/

static inline void ssl_context_s_flush_all(ssl_context_s *this)
{/*{{{*/
}/*}}}*/

static inline void ssl_context_s_swap(ssl_context_s *this,ssl_context_s *a_second)
{/*{{{*/
  ssl_context_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void ssl_context_s_copy(const ssl_context_s *this,const ssl_context_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int ssl_context_s_compare(const ssl_context_s *this,const ssl_context_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void ssl_context_s_to_string(const ssl_context_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"ssl_context_s{%p}",*this);
}/*}}}*/
#endif

static inline int ssl_context_s_create_server(ssl_context_s *this)
{/*{{{*/
  ssl_context_s_clear(this);

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
  const SSL_METHOD *method = TLS_server_method();
#else
  const SSL_METHOD *method = TLSv1_2_server_method();
#endif

  // - ERROR -
  if ((*this = SSL_CTX_new(method)) == NULL)
  {
    throw_error(SSL_CONTEXT_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int ssl_context_s_create_client(ssl_context_s *this)
{/*{{{*/
  ssl_context_s_clear(this);

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
    const SSL_METHOD *method = TLS_client_method();
#else
    const SSL_METHOD *method = TLSv1_2_client_method();
#endif

  // - ERROR -
  if ((*this = SSL_CTX_new(method)) == NULL)
  {
    throw_error(SSL_CONTEXT_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int ssl_context_s_use_certificate_file(ssl_context_s *this,
    const char *a_file_name,int a_file_type)
{/*{{{*/

  // - ERROR -
  if (SSL_CTX_use_certificate_file(*this,a_file_name,a_file_type) != 1)
  {
    throw_error(SSL_CONTEXT_CERTIFICATE_FILE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int ssl_context_s_use_private_key_file(ssl_context_s *this,
    const char *a_file_name,int a_file_type)
{/*{{{*/

  // - ERROR -
  if (SSL_CTX_use_PrivateKey_file(*this,a_file_name,a_file_type) != 1)
  {
    throw_error(SSL_CONTEXT_PRIVATE_KEY_FILE_ERROR);
  }

  return 0;
}/*}}}*/

// === inline methods of structure ssl_conn_s ==================================

static inline void ssl_conn_s_init(ssl_conn_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void ssl_conn_s_clear(ssl_conn_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    SSL_free(*this);
  }

  ssl_conn_s_init(this);
}/*}}}*/

static inline void ssl_conn_s_flush_all(ssl_conn_s *this)
{/*{{{*/
}/*}}}*/

static inline void ssl_conn_s_swap(ssl_conn_s *this,ssl_conn_s *a_second)
{/*{{{*/
  ssl_conn_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void ssl_conn_s_copy(const ssl_conn_s *this,const ssl_conn_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int ssl_conn_s_compare(const ssl_conn_s *this,const ssl_conn_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void ssl_conn_s_to_string(const ssl_conn_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"ssl_conn_s{%p}",*this);
}/*}}}*/
#endif

static inline void ssl_conn_s_set_accept_state(const ssl_conn_s *this)
{/*{{{*/
  debug_assert(*this != NULL);

  SSL_set_accept_state(*this);
}/*}}}*/

static inline void ssl_conn_s_set_connect_state(const ssl_conn_s *this)
{/*{{{*/
  debug_assert(*this != NULL);

  SSL_set_connect_state(*this);
}/*}}}*/

#endif

