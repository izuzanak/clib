
#ifndef __CL_CRYPTO_H
#define __CL_CRYPTO_H

@begin
include "cl_struct.h"
@end

#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libcrypto_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libcrypto_cll_EXPORTS
#define libcrypto_cll_EXPORT __declspec(dllexport)
#else
#define libcrypto_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_CRYPTO_PKEY_CANNOT_READ_KEY_FROM_FILE 1
#define ERROR_CRYPTO_INVALID_BASE_DATA_SIZE 2
#define ERROR_CRYPTO_INVALID_BASE_DATA 3

// === definition of structure crypto_pkey_s ===================================

typedef struct crypto_pkey_s
{
  EVP_PKEY *pkey;
  int ispub;
} crypto_pkey_s;

@begin
define crypto_pkey_s dynamic
@end

static inline void crypto_pkey_s_init(crypto_pkey_s *this);
static inline void crypto_pkey_s_clear(crypto_pkey_s *this);
static inline void crypto_pkey_s_flush_all(crypto_pkey_s *this);
static inline void crypto_pkey_s_swap(crypto_pkey_s *this,crypto_pkey_s *a_second);
static inline void crypto_pkey_s_copy(const crypto_pkey_s *this,const crypto_pkey_s *a_src);
static inline int crypto_pkey_s_compare(const crypto_pkey_s *this,const crypto_pkey_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void crypto_pkey_s_to_string(const crypto_pkey_s *this,bc_array_s *a_trg);
#endif

int crypto_pkey_s_password_cb(char *buf,int size,int rwflag,void *userdata);
WUR libcrypto_cll_EXPORT int crypto_pkey_s_load_private(crypto_pkey_s *this,const char *a_path,char *a_password);
WUR libcrypto_cll_EXPORT int crypto_pkey_s_load_public(crypto_pkey_s *this,const char *a_path,char *a_password);

// === inline methods of structure crypto_pkey_s ===============================

static inline void crypto_pkey_s_init(crypto_pkey_s *this)
{/*{{{*/
  this->pkey = NULL;
}/*}}}*/

static inline void crypto_pkey_s_clear(crypto_pkey_s *this)
{/*{{{*/
  if (this->pkey != NULL)
  {
    EVP_PKEY_free(this->pkey);
  }

  crypto_pkey_s_init(this);
}/*}}}*/

static inline void crypto_pkey_s_flush_all(crypto_pkey_s *this)
{/*{{{*/
}/*}}}*/

static inline void crypto_pkey_s_swap(crypto_pkey_s *this,crypto_pkey_s *a_second)
{/*{{{*/
  crypto_pkey_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void crypto_pkey_s_copy(const crypto_pkey_s *this,const crypto_pkey_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int crypto_pkey_s_compare(const crypto_pkey_s *this,const crypto_pkey_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void crypto_pkey_s_to_string(const crypto_pkey_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"crypto_pkey_s{%p}",this);
}/*}}}*/
#endif

// === definition of global functions ==========================================

void crypto_random(int a_count,bc_array_s *a_trg);
libcrypto_cll_EXPORT void crypto_encode_base16(const char *a_data,unsigned a_size,bc_array_s *a_trg);
WUR libcrypto_cll_EXPORT int crypto_decode_base16(const char *a_data,unsigned a_size,bc_array_s *a_trg);
libcrypto_cll_EXPORT void crypto_encode_base64(const char *a_data,unsigned a_size,bc_array_s *a_trg);
WUR libcrypto_cll_EXPORT int crypto_decode_base64(const char *a_data,unsigned a_size,bc_array_s *a_trg);

void libcrypto_cll_init();
void libcrypto_cll_clear();

#endif

