
#ifndef CL_LIB_CRYPTO_H
#define CL_LIB_CRYPTO_H

@begin
include "cl_struct.h"
@end

#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/opensslv.h>

// - replace deprecated function -
#if OPENSSL_VERSION_MAJOR >= 3
#define EVP_MD_CTX_MD EVP_MD_CTX_get0_md
#else
#define EVP_MD_CTX_MD EVP_MD_CTX_md
#endif

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

#define ERROR_CRYPTO_DIGEST_INVALID_ALGORITHM_NAME 1
#define ERROR_CRYPTO_DIGEST_CREATE_INIT_ERROR 2
#define ERROR_CRYPTO_DIGEST_UPDATE_ERROR 3
#define ERROR_CRYPTO_DIGEST_VALUE_ERROR 4

#define ERROR_CRYPTO_SIGN_CREATE_INIT_ERROR 1
#define ERROR_CRYPTO_SIGN_UPDATE_ERROR 2
#define ERROR_CRYPTO_SIGN_VALUE_ERROR 3

#define ERROR_CRYPTO_VERIFY_CREATE_INIT_ERROR 1
#define ERROR_CRYPTO_VERIFY_UPDATE_ERROR 2
#define ERROR_CRYPTO_VERIFY_VERIFY_ERROR 3

#define ERROR_CRYPTO_CIPHER_INVALID_ALGORITHM_NAME 1
#define ERROR_CRYPTO_CIPHER_INVALID_KEY_LENGTH 2
#define ERROR_CRYPTO_CIPHER_INVALID_INIT_VECTOR_LENGTH 3
#define ERROR_CRYPTO_CIPHER_NEW_INIT_ERROR 4
#define ERROR_CRYPTO_CIPHER_UPDATE_ERROR 5
#define ERROR_CRYPTO_CIPHER_FINALIZE_ERROR 6
#define ERROR_CRYPTO_CIPHER_INVALID_PRIVATE_PUBLIC_KEY 7

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

// === definition of structure crypto_digest_info_s ============================

typedef const EVP_MD *crypto_digest_info_s;
@begin
define crypto_digest_info_s dynamic
@end

static inline void crypto_digest_info_s_init(crypto_digest_info_s *this);
static inline void crypto_digest_info_s_clear(crypto_digest_info_s *this);
static inline void crypto_digest_info_s_flush_all(crypto_digest_info_s *this);
static inline void crypto_digest_info_s_swap(crypto_digest_info_s *this,crypto_digest_info_s *a_second);
static inline void crypto_digest_info_s_copy(crypto_digest_info_s *this,const crypto_digest_info_s *a_src);
static inline int crypto_digest_info_s_compare(const crypto_digest_info_s *this,const crypto_digest_info_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void crypto_digest_info_s_to_string(const crypto_digest_info_s *this,bc_array_s *a_trg);
#endif

WUR static inline int crypto_digest_info_s_get_by_name(crypto_digest_info_s *this,const char *a_name);
static inline const char *crypto_digest_info_s_name(crypto_digest_info_s *this);

// === definition of structure crypto_digest_s =================================

typedef struct crypto_digest_s
{
  EVP_MD_CTX *context;
} crypto_digest_s;

@begin
define crypto_digest_s dynamic
@end

static inline void crypto_digest_s_init(crypto_digest_s *this);
static inline void crypto_digest_s_clear(crypto_digest_s *this);
static inline void crypto_digest_s_flush_all(crypto_digest_s *this);
static inline void crypto_digest_s_swap(crypto_digest_s *this,crypto_digest_s *a_second);
static inline void crypto_digest_s_copy(const crypto_digest_s *this,const crypto_digest_s *a_src);
static inline int crypto_digest_s_compare(const crypto_digest_s *this,const crypto_digest_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void crypto_digest_s_to_string(const crypto_digest_s *this,bc_array_s *a_trg);
#endif

WUR libcrypto_cll_EXPORT int crypto_digest_s_create(crypto_digest_s *this,crypto_digest_info_s *a_digest_info);
WUR static inline int crypto_digest_s_update(crypto_digest_s *this,const char *a_data,unsigned a_size);
WUR libcrypto_cll_EXPORT int crypto_digest_s_value(crypto_digest_s *this,bc_array_s *a_trg);

// === definition of structure crypto_sign_s ===================================

typedef struct crypto_sign_s
{
  EVP_MD_CTX *context;
  EVP_PKEY *pkey;
} crypto_sign_s;

@begin
define crypto_sign_s dynamic
@end

static inline void crypto_sign_s_init(crypto_sign_s *this);
static inline void crypto_sign_s_clear(crypto_sign_s *this);
static inline void crypto_sign_s_flush_all(crypto_sign_s *this);
static inline void crypto_sign_s_swap(crypto_sign_s *this,crypto_sign_s *a_second);
static inline void crypto_sign_s_copy(const crypto_sign_s *this,const crypto_sign_s *a_src);
static inline int crypto_sign_s_compare(const crypto_sign_s *this,const crypto_sign_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void crypto_sign_s_to_string(const crypto_sign_s *this,bc_array_s *a_trg);
#endif

WUR static inline int crypto_sign_s_create(crypto_sign_s *this,
    crypto_digest_info_s *a_digest_info,crypto_pkey_s *a_pkey);
WUR libcrypto_cll_EXPORT int crypto_sign_s_create_pkey_ctx(crypto_sign_s *this,
    crypto_digest_info_s *a_digest_info,crypto_pkey_s *a_pkey,EVP_PKEY_CTX **a_pkey_ctx);
WUR static inline int crypto_sign_s_update(crypto_sign_s *this,const char *a_data,unsigned a_size);
WUR libcrypto_cll_EXPORT int crypto_sign_s_value(crypto_sign_s *this,bc_array_s *a_trg);

// === definition of structure crypto_verify_s =================================

typedef struct crypto_verify_s
{
  EVP_MD_CTX *context;
  EVP_PKEY *pkey;
} crypto_verify_s;

@begin
define crypto_verify_s dynamic
@end

static inline void crypto_verify_s_init(crypto_verify_s *this);
static inline void crypto_verify_s_clear(crypto_verify_s *this);
static inline void crypto_verify_s_flush_all(crypto_verify_s *this);
static inline void crypto_verify_s_swap(crypto_verify_s *this,crypto_verify_s *a_second);
static inline void crypto_verify_s_copy(const crypto_verify_s *this,const crypto_verify_s *a_src);
static inline int crypto_verify_s_compare(const crypto_verify_s *this,const crypto_verify_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void crypto_verify_s_to_string(const crypto_verify_s *this,bc_array_s *a_trg);
#endif

WUR libcrypto_cll_EXPORT int crypto_verify_s_create(crypto_verify_s *this,
    crypto_digest_info_s *a_digest_info,crypto_pkey_s *a_pkey);
WUR static inline int crypto_verify_s_update(crypto_verify_s *this,const char *a_data,unsigned a_size);
WUR libcrypto_cll_EXPORT int crypto_verify_s_verify(crypto_verify_s *this,
    const char *a_data,unsigned a_size,int *a_result);

// === definition of structure crypto_cipher_info_s ============================

typedef const EVP_CIPHER *crypto_cipher_info_s;
@begin
define crypto_cipher_info_s dynamic
@end

static inline void crypto_cipher_info_s_init(crypto_cipher_info_s *this);
static inline void crypto_cipher_info_s_clear(crypto_cipher_info_s *this);
static inline void crypto_cipher_info_s_flush_all(crypto_cipher_info_s *this);
static inline void crypto_cipher_info_s_swap(crypto_cipher_info_s *this,crypto_cipher_info_s *a_second);
static inline void crypto_cipher_info_s_copy(crypto_cipher_info_s *this,const crypto_cipher_info_s *a_src);
static inline int crypto_cipher_info_s_compare(const crypto_cipher_info_s *this,
    const crypto_cipher_info_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void crypto_cipher_info_s_to_string(const crypto_cipher_info_s *this,bc_array_s *a_trg);
#endif

WUR static inline int crypto_cipher_info_s_get_by_name(crypto_cipher_info_s *this,const char *a_name);
static inline const char *crypto_cipher_info_s_name(crypto_cipher_info_s *this);
static inline unsigned crypto_cipher_info_s_block_size(crypto_cipher_info_s *this);
static inline unsigned crypto_cipher_info_s_key_length(crypto_cipher_info_s *this);
static inline unsigned crypto_cipher_info_s_iv_length(crypto_cipher_info_s *this);

// === definition of structure crypto_encrypt_s ================================

typedef struct crypto_encrypt_s
{
  EVP_CIPHER_CTX *context;
} crypto_encrypt_s;

@begin
define crypto_encrypt_s dynamic
@end

static inline void crypto_encrypt_s_init(crypto_encrypt_s *this);
static inline void crypto_encrypt_s_clear(crypto_encrypt_s *this);
static inline void crypto_encrypt_s_flush_all(crypto_encrypt_s *this);
static inline void crypto_encrypt_s_swap(crypto_encrypt_s *this,crypto_encrypt_s *a_second);
static inline void crypto_encrypt_s_copy(const crypto_encrypt_s *this,const crypto_encrypt_s *a_src);
static inline int crypto_encrypt_s_compare(const crypto_encrypt_s *this,const crypto_encrypt_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void crypto_encrypt_s_to_string(const crypto_encrypt_s *this,bc_array_s *a_trg);
#endif

WUR libcrypto_cll_EXPORT int crypto_encrypt_s_create(crypto_encrypt_s *this,
    crypto_cipher_info_s *a_cipher_info,
    const char *a_key,unsigned a_key_length,
    const char *a_iv,unsigned a_iv_length);
WUR static inline int crypto_encrypt_s_update(crypto_encrypt_s *this,
    const char *a_data,unsigned a_size,bc_array_s *a_trg);
WUR libcrypto_cll_EXPORT int crypto_encrypt_s_finalize(crypto_encrypt_s *this,bc_array_s *a_trg);

// === definition of structure crypto_decrypt_s ================================

typedef struct crypto_decrypt_s
{
  EVP_CIPHER_CTX *context;
} crypto_decrypt_s;

@begin
define crypto_decrypt_s dynamic
@end

static inline void crypto_decrypt_s_init(crypto_decrypt_s *this);
static inline void crypto_decrypt_s_clear(crypto_decrypt_s *this);
static inline void crypto_decrypt_s_flush_all(crypto_decrypt_s *this);
static inline void crypto_decrypt_s_swap(crypto_decrypt_s *this,crypto_decrypt_s *a_second);
static inline void crypto_decrypt_s_copy(const crypto_decrypt_s *this,const crypto_decrypt_s *a_src);
static inline int crypto_decrypt_s_compare(const crypto_decrypt_s *this,const crypto_decrypt_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void crypto_decrypt_s_to_string(const crypto_decrypt_s *this,bc_array_s *a_trg);
#endif

WUR libcrypto_cll_EXPORT int crypto_decrypt_s_create(crypto_decrypt_s *this,
    crypto_cipher_info_s *a_cipher_info,
    const char *a_key,unsigned a_key_length,
    const char *a_iv,unsigned a_iv_length);
WUR static inline int crypto_decrypt_s_update(crypto_decrypt_s *this,
    const char *a_data,unsigned a_size,bc_array_s *a_trg);
WUR libcrypto_cll_EXPORT int crypto_decrypt_s_finalize(crypto_decrypt_s *this,bc_array_s *a_trg);

// === definition of structure crypto_seal_s ===================================

typedef struct crypto_seal_s
{
  EVP_CIPHER_CTX *context;
  bc_arrays_s keys;
  bc_array_s iv;
} crypto_seal_s;

@begin
define crypto_seal_s dynamic
@end

static inline void crypto_seal_s_init(crypto_seal_s *this);
static inline void crypto_seal_s_clear(crypto_seal_s *this);
static inline void crypto_seal_s_flush_all(crypto_seal_s *this);
static inline void crypto_seal_s_swap(crypto_seal_s *this,crypto_seal_s *a_second);
static inline void crypto_seal_s_copy(const crypto_seal_s *this,const crypto_seal_s *a_src);
static inline int crypto_seal_s_compare(const crypto_seal_s *this,const crypto_seal_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void crypto_seal_s_to_string(const crypto_seal_s *this,bc_array_s *a_trg);
#endif

WUR libcrypto_cll_EXPORT int crypto_seal_s_create(crypto_seal_s *this,
    crypto_cipher_info_s *a_cipher_info,crypto_pkey_s *a_pubk,unsigned a_npubk);
WUR static inline int crypto_seal_s_update(crypto_seal_s *this,
    const char *a_data,unsigned a_size,bc_array_s *a_trg);
WUR libcrypto_cll_EXPORT int crypto_seal_s_finalize(crypto_seal_s *this,bc_array_s *a_trg);

// === definition of structure crypto_open_s ===================================

typedef struct crypto_open_s
{
  EVP_CIPHER_CTX *context;
} crypto_open_s;

@begin
define crypto_open_s dynamic
@end

static inline void crypto_open_s_init(crypto_open_s *this);
static inline void crypto_open_s_clear(crypto_open_s *this);
static inline void crypto_open_s_flush_all(crypto_open_s *this);
static inline void crypto_open_s_swap(crypto_open_s *this,crypto_open_s *a_second);
static inline void crypto_open_s_copy(const crypto_open_s *this,const crypto_open_s *a_src);
static inline int crypto_open_s_compare(const crypto_open_s *this,const crypto_open_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void crypto_open_s_to_string(const crypto_open_s *this,bc_array_s *a_trg);
#endif

WUR libcrypto_cll_EXPORT int crypto_open_s_create(crypto_open_s *this,
    crypto_cipher_info_s *a_cipher_info,
    const char *a_key,unsigned a_key_length,
    const char *a_iv,unsigned a_iv_length,
    crypto_pkey_s *a_priv);
WUR static inline int crypto_open_s_update(crypto_open_s *this,
    const char *a_data,unsigned a_size,bc_array_s *a_trg);
WUR libcrypto_cll_EXPORT int crypto_open_s_finalize(crypto_open_s *this,bc_array_s *a_trg);

// === definition of global functions ==========================================

void crypto_random(int a_count,bc_array_s *a_trg);
libcrypto_cll_EXPORT void crypto_encode_base16(const char *a_data,unsigned a_size,bc_array_s *a_trg);
WUR libcrypto_cll_EXPORT int crypto_decode_base16(const char *a_data,unsigned a_size,bc_array_s *a_trg);
libcrypto_cll_EXPORT void crypto_encode_base64(const char *a_data,unsigned a_size,bc_array_s *a_trg);
WUR libcrypto_cll_EXPORT int crypto_decode_base64(const char *a_data,unsigned a_size,bc_array_s *a_trg);

void libcrypto_cll_init();
void libcrypto_cll_clear();

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

// === inline methods of structure crypto_digest_info_s ========================

static inline void crypto_digest_info_s_init(crypto_digest_info_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void crypto_digest_info_s_clear(crypto_digest_info_s *this)
{/*{{{*/
  crypto_digest_info_s_init(this);
}/*}}}*/

static inline void crypto_digest_info_s_flush_all(crypto_digest_info_s *this)
{/*{{{*/
}/*}}}*/

static inline void crypto_digest_info_s_swap(crypto_digest_info_s *this,crypto_digest_info_s *a_second)
{/*{{{*/
  crypto_digest_info_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void crypto_digest_info_s_copy(crypto_digest_info_s *this,const crypto_digest_info_s *a_src)
{/*{{{*/
  *this = *a_src;
}/*}}}*/

static inline int crypto_digest_info_s_compare(const crypto_digest_info_s *this,const crypto_digest_info_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void crypto_digest_info_s_to_string(const crypto_digest_info_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"crypto_digest_info_s{%p}",*this);
}/*}}}*/
#endif

static inline int crypto_digest_info_s_get_by_name(crypto_digest_info_s *this,const char *a_name)
{/*{{{*/
  crypto_digest_info_s_clear(this);

  // - ERROR -
  if ((*this = EVP_get_digestbyname(a_name)) == NULL)
  {
    throw_error(CRYPTO_DIGEST_INVALID_ALGORITHM_NAME);
  }

  return 0;
}/*}}}*/

static inline const char *crypto_digest_info_s_name(crypto_digest_info_s *this)
{/*{{{*/
  debug_assert(*this != NULL);

  return EVP_MD_name(*this);
}/*}}}*/

// === inline methods of structure crypto_digest_s =============================

static inline void crypto_digest_s_init(crypto_digest_s *this)
{/*{{{*/
  this->context = NULL;
}/*}}}*/

static inline void crypto_digest_s_clear(crypto_digest_s *this)
{/*{{{*/
  if (this->context != NULL)
  {
    EVP_MD_CTX_destroy(this->context);
  }

  crypto_digest_s_init(this);
}/*}}}*/

static inline void crypto_digest_s_flush_all(crypto_digest_s *this)
{/*{{{*/
}/*}}}*/

static inline void crypto_digest_s_swap(crypto_digest_s *this,crypto_digest_s *a_second)
{/*{{{*/
  crypto_digest_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void crypto_digest_s_copy(const crypto_digest_s *this,const crypto_digest_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int crypto_digest_s_compare(const crypto_digest_s *this,const crypto_digest_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void crypto_digest_s_to_string(const crypto_digest_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"crypto_digest_s{%p}",this);
}/*}}}*/
#endif

static inline int crypto_digest_s_update(crypto_digest_s *this,const char *a_data,unsigned a_size)
{/*{{{*/

  // - ERROR -
  if (EVP_DigestUpdate(this->context,a_data,a_size) != 1)
  {
    throw_error(CRYPTO_DIGEST_UPDATE_ERROR);
  }

  return 0;
}/*}}}*/

// === inline methods of structure crypto_sign_s ===============================

static inline void crypto_sign_s_init(crypto_sign_s *this)
{/*{{{*/
  this->context = NULL;
  this->pkey = NULL;
}/*}}}*/

static inline void crypto_sign_s_clear(crypto_sign_s *this)
{/*{{{*/
  if (this->context != NULL)
  {
    EVP_MD_CTX_destroy(this->context);
  }

  crypto_sign_s_init(this);
}/*}}}*/

static inline void crypto_sign_s_flush_all(crypto_sign_s *this)
{/*{{{*/
}/*}}}*/

static inline void crypto_sign_s_swap(crypto_sign_s *this,crypto_sign_s *a_second)
{/*{{{*/
  crypto_sign_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void crypto_sign_s_copy(const crypto_sign_s *this,const crypto_sign_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int crypto_sign_s_compare(const crypto_sign_s *this,const crypto_sign_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void crypto_sign_s_to_string(const crypto_sign_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"crypto_sign_s{%p}",this);
}/*}}}*/
#endif

static inline int crypto_sign_s_create(crypto_sign_s *this,
    crypto_digest_info_s *a_digest_info,crypto_pkey_s *a_pkey)
{/*{{{*/
  return crypto_sign_s_create_pkey_ctx(this,a_digest_info,a_pkey,NULL);
}/*}}}*/

static inline int crypto_sign_s_update(crypto_sign_s *this,const char *a_data,unsigned a_size)
{/*{{{*/

  // - ERROR -
  if (EVP_DigestSignUpdate(this->context,a_data,a_size) != 1)
  {
    throw_error(CRYPTO_SIGN_UPDATE_ERROR);
  }

  return 0;
}/*}}}*/

// === inline methods of structure crypto_verify_s =============================

static inline void crypto_verify_s_init(crypto_verify_s *this)
{/*{{{*/
  this->context = NULL;
  this->pkey = NULL;
}/*}}}*/

static inline void crypto_verify_s_clear(crypto_verify_s *this)
{/*{{{*/
  if (this->context != NULL)
  {
    EVP_MD_CTX_destroy(this->context);
  }

  crypto_verify_s_init(this);
}/*}}}*/

static inline void crypto_verify_s_flush_all(crypto_verify_s *this)
{/*{{{*/
}/*}}}*/

static inline void crypto_verify_s_swap(crypto_verify_s *this,crypto_verify_s *a_second)
{/*{{{*/
  crypto_verify_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void crypto_verify_s_copy(const crypto_verify_s *this,const crypto_verify_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int crypto_verify_s_compare(const crypto_verify_s *this,const crypto_verify_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void crypto_verify_s_to_string(const crypto_verify_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"crypto_verify_s{%p}",this);
}/*}}}*/
#endif

static inline int crypto_verify_s_update(crypto_verify_s *this,const char *a_data,unsigned a_size)
{/*{{{*/

  // - ERROR -
  if (EVP_DigestVerifyUpdate(this->context,a_data,a_size) != 1)
  {
    throw_error(CRYPTO_VERIFY_UPDATE_ERROR);
  }

  return 0;
}/*}}}*/

// === inline methods of structure crypto_cipher_info_s ========================

static inline void crypto_cipher_info_s_init(crypto_cipher_info_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void crypto_cipher_info_s_clear(crypto_cipher_info_s *this)
{/*{{{*/
  crypto_cipher_info_s_init(this);
}/*}}}*/

static inline void crypto_cipher_info_s_flush_all(crypto_cipher_info_s *this)
{/*{{{*/
}/*}}}*/

static inline void crypto_cipher_info_s_swap(crypto_cipher_info_s *this,crypto_cipher_info_s *a_second)
{/*{{{*/
  crypto_cipher_info_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void crypto_cipher_info_s_copy(crypto_cipher_info_s *this,const crypto_cipher_info_s *a_src)
{/*{{{*/
  *this = *a_src;
}/*}}}*/

static inline int crypto_cipher_info_s_compare(const crypto_cipher_info_s *this,const crypto_cipher_info_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void crypto_cipher_info_s_to_string(const crypto_cipher_info_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"crypto_cipher_info_s{%p}",*this);
}/*}}}*/
#endif

static inline int crypto_cipher_info_s_get_by_name(crypto_cipher_info_s *this,const char *a_name)
{/*{{{*/
  crypto_cipher_info_s_clear(this);

  // - ERROR -
  if ((*this = EVP_get_cipherbyname(a_name)) == NULL)
  {
    throw_error(CRYPTO_CIPHER_INVALID_ALGORITHM_NAME);
  }

  return 0;
}/*}}}*/

static inline const char *crypto_cipher_info_s_name(crypto_cipher_info_s *this)
{/*{{{*/
  debug_assert(*this != NULL);

  return EVP_CIPHER_name(*this);
}/*}}}*/

static inline unsigned crypto_cipher_info_s_block_size(crypto_cipher_info_s *this)
{/*{{{*/
  debug_assert(*this != NULL);

  return EVP_CIPHER_block_size(*this);
}/*}}}*/

static inline unsigned crypto_cipher_info_s_key_length(crypto_cipher_info_s *this)
{/*{{{*/
  debug_assert(*this != NULL);

  return EVP_CIPHER_key_length(*this);
}/*}}}*/

static inline unsigned crypto_cipher_info_s_iv_length(crypto_cipher_info_s *this)
{/*{{{*/
  debug_assert(*this != NULL);

  return EVP_CIPHER_iv_length(*this);
}/*}}}*/

// === inline methods of structure crypto_encrypt_s ============================

static inline void crypto_encrypt_s_init(crypto_encrypt_s *this)
{/*{{{*/
  this->context = NULL;
}/*}}}*/

static inline void crypto_encrypt_s_clear(crypto_encrypt_s *this)
{/*{{{*/
  if (this->context != NULL)
  {
    EVP_CIPHER_CTX_free(this->context);
  }

  crypto_encrypt_s_init(this);
}/*}}}*/

static inline void crypto_encrypt_s_flush_all(crypto_encrypt_s *this)
{/*{{{*/
}/*}}}*/

static inline void crypto_encrypt_s_swap(crypto_encrypt_s *this,crypto_encrypt_s *a_second)
{/*{{{*/
  crypto_encrypt_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void crypto_encrypt_s_copy(const crypto_encrypt_s *this,const crypto_encrypt_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int crypto_encrypt_s_compare(const crypto_encrypt_s *this,const crypto_encrypt_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void crypto_encrypt_s_to_string(const crypto_encrypt_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"crypto_encrypt_s{%p}",this);
}/*}}}*/
#endif

static inline int crypto_encrypt_s_update(crypto_encrypt_s *this,
    const char *a_data,unsigned a_size,bc_array_s *a_trg)
{/*{{{*/
  unsigned block_size = EVP_CIPHER_CTX_block_size(this->context);
  bc_array_s_reserve(a_trg,a_size + (block_size - a_size % block_size));

  // - ERROR -
  int result_length;
  if (EVP_EncryptUpdate(this->context,
      (unsigned char *)a_trg->data + a_trg->used,&result_length,
      (unsigned char *)a_data,a_size) != 1)
  {
    throw_error(CRYPTO_CIPHER_UPDATE_ERROR);
  }

  a_trg->used += result_length;

  return 0;
}/*}}}*/

// === inline methods of structure crypto_decrypt_s ============================

static inline void crypto_decrypt_s_init(crypto_decrypt_s *this)
{/*{{{*/
  this->context = NULL;
}/*}}}*/

static inline void crypto_decrypt_s_clear(crypto_decrypt_s *this)
{/*{{{*/
  if (this->context != NULL)
  {
    EVP_CIPHER_CTX_free(this->context);
  }

  crypto_decrypt_s_init(this);
}/*}}}*/

static inline void crypto_decrypt_s_flush_all(crypto_decrypt_s *this)
{/*{{{*/
}/*}}}*/

static inline void crypto_decrypt_s_swap(crypto_decrypt_s *this,crypto_decrypt_s *a_second)
{/*{{{*/
  crypto_decrypt_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void crypto_decrypt_s_copy(const crypto_decrypt_s *this,const crypto_decrypt_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int crypto_decrypt_s_compare(const crypto_decrypt_s *this,const crypto_decrypt_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void crypto_decrypt_s_to_string(const crypto_decrypt_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"crypto_decrypt_s{%p}",this);
}/*}}}*/
#endif

static inline int crypto_decrypt_s_update(crypto_decrypt_s *this,
    const char *a_data,unsigned a_size,bc_array_s *a_trg)
{/*{{{*/
  unsigned block_size = EVP_CIPHER_CTX_block_size(this->context);
  bc_array_s_reserve(a_trg,a_size + (block_size - a_size % block_size));

  // - ERROR -
  int result_length;
  if (EVP_DecryptUpdate(this->context,
      (unsigned char *)a_trg->data + a_trg->used,&result_length,
      (unsigned char *)a_data,a_size) != 1)
  {
    throw_error(CRYPTO_CIPHER_UPDATE_ERROR);
  }

  a_trg->used += result_length;

  return 0;
}/*}}}*/

// === inline methods of structure crypto_seal_s ============================

static inline void crypto_seal_s_init(crypto_seal_s *this)
{/*{{{*/
  this->context = NULL;
  bc_arrays_s_init(&this->keys);
  bc_array_s_init(&this->iv);
}/*}}}*/

static inline void crypto_seal_s_clear(crypto_seal_s *this)
{/*{{{*/
  if (this->context != NULL)
  {
    EVP_CIPHER_CTX_free(this->context);
  }

  bc_arrays_s_clear(&this->keys);
  bc_array_s_clear(&this->iv);

  crypto_seal_s_init(this);
}/*}}}*/

static inline void crypto_seal_s_flush_all(crypto_seal_s *this)
{/*{{{*/
}/*}}}*/

static inline void crypto_seal_s_swap(crypto_seal_s *this,crypto_seal_s *a_second)
{/*{{{*/
  crypto_seal_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void crypto_seal_s_copy(const crypto_seal_s *this,const crypto_seal_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int crypto_seal_s_compare(const crypto_seal_s *this,const crypto_seal_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void crypto_seal_s_to_string(const crypto_seal_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"crypto_seal_s{%p}",this);
}/*}}}*/
#endif

static inline int crypto_seal_s_update(crypto_seal_s *this,
    const char *a_data,unsigned a_size,bc_array_s *a_trg)
{/*{{{*/
  unsigned block_size = EVP_CIPHER_CTX_block_size(this->context);
  bc_array_s_reserve(a_trg,a_size + (block_size - a_size % block_size));

  // - ERROR -
  int result_length;
  if (EVP_SealUpdate(this->context,
      (unsigned char *)a_trg->data + a_trg->used,&result_length,
      (unsigned char *)a_data,a_size) != 1)
  {
    throw_error(CRYPTO_CIPHER_UPDATE_ERROR);
  }

  a_trg->used += result_length;

  return 0;
}/*}}}*/

// === inline methods of structure crypto_open_s ===============================

static inline void crypto_open_s_init(crypto_open_s *this)
{/*{{{*/
  this->context = NULL;
}/*}}}*/

static inline void crypto_open_s_clear(crypto_open_s *this)
{/*{{{*/
  if (this->context != NULL)
  {
    EVP_CIPHER_CTX_free(this->context);
  }

  crypto_open_s_init(this);
}/*}}}*/

static inline void crypto_open_s_flush_all(crypto_open_s *this)
{/*{{{*/
}/*}}}*/

static inline void crypto_open_s_swap(crypto_open_s *this,crypto_open_s *a_second)
{/*{{{*/
  crypto_open_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void crypto_open_s_copy(const crypto_open_s *this,const crypto_open_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int crypto_open_s_compare(const crypto_open_s *this,const crypto_open_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void crypto_open_s_to_string(const crypto_open_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"crypto_open_s{%p}",this);
}/*}}}*/
#endif

static inline int crypto_open_s_update(crypto_open_s *this,
    const char *a_data,unsigned a_size,bc_array_s *a_trg)
{/*{{{*/
  unsigned block_size = EVP_CIPHER_CTX_block_size(this->context);
  bc_array_s_reserve(a_trg,a_size + (block_size - a_size % block_size));

  // - ERROR -
  int result_length;
  if (EVP_OpenUpdate(this->context,
      (unsigned char *)a_trg->data + a_trg->used,&result_length,
      (unsigned char *)a_data,a_size) != 1)
  {
    throw_error(CRYPTO_CIPHER_UPDATE_ERROR);
  }

  a_trg->used += result_length;

  return 0;
}/*}}}*/

#endif

