
@begin
include "cl_crypto.h"
@end

const char c_base16_map[] = "0123456789abcdef";

// === methods of structure crypto_pkey_s ======================================

int crypto_pkey_s_password_cb(char *buf,int size,int rwflag,void *userdata)
{/*{{{*/
  (void)rwflag;

  const char *password = (const char *)userdata;
  int password_length = strlen(password);

  int pass_size = size < password_length ? size : password_length;
  memcpy(buf,password,pass_size*sizeof(char));

  return pass_size;
}/*}}}*/

int crypto_pkey_s_load_private(crypto_pkey_s *this,const char *a_path,char *a_password)
{/*{{{*/
  crypto_pkey_s_clear(this);

  // - ERROR -
  FILE *file = fopen(a_path,"rb");
  if (file == NULL)
  {
    throw_error(CRYPTO_PKEY_CANNOT_READ_KEY_FROM_FILE);
  }

  this->pkey = PEM_read_PrivateKey(file,NULL,crypto_pkey_s_password_cb,a_password);
  fclose(file);

  // - ERROR -
  if (this->pkey == NULL)
  {
    throw_error(CRYPTO_PKEY_CANNOT_READ_KEY_FROM_FILE);
  }

  this->ispub = 0;

  return 0;
}/*}}}*/

int crypto_pkey_s_load_public(crypto_pkey_s *this,const char *a_path,char *a_password)
{/*{{{*/
  crypto_pkey_s_clear(this);

  // - ERROR -
  FILE *file = fopen(a_path,"rb");
  if (file == NULL)
  {
    throw_error(CRYPTO_PKEY_CANNOT_READ_KEY_FROM_FILE);
  }

  this->pkey = PEM_read_PUBKEY(file,NULL,crypto_pkey_s_password_cb,a_password);
  fclose(file);

  // - ERROR -
  if (this->pkey == NULL)
  {
    throw_error(CRYPTO_PKEY_CANNOT_READ_KEY_FROM_FILE);
  }

  this->ispub = 1;

  return 0;
}/*}}}*/

// === methods of structure crypto_digest_info_s ===============================

// === methods of structure crypto_digest_s ====================================

int crypto_digest_s_create(crypto_digest_s *this,crypto_digest_info_s *a_digest_info)
{/*{{{*/
  crypto_digest_s_clear(this);

  // - ERROR -
  if ((this->context = EVP_MD_CTX_create()) == NULL)
  {
    throw_error(CRYPTO_DIGEST_CREATE_INIT_ERROR);
  }

  // - ERROR -
  if (EVP_DigestInit_ex(this->context,*a_digest_info,NULL) != 1)
  {
    crypto_digest_s_clear(this);

    throw_error(CRYPTO_DIGEST_CREATE_INIT_ERROR);
  }

  return 0;
}/*}}}*/

int crypto_digest_s_value(crypto_digest_s *this,bc_array_s *a_trg)
{/*{{{*/

  // - ERROR -
  EVP_MD_CTX *context_copy;
  if ((context_copy = EVP_MD_CTX_create()) == NULL)
  {
    throw_error(CRYPTO_DIGEST_VALUE_ERROR);
  }

  // - ERROR -
  if (EVP_DigestInit_ex(context_copy,EVP_MD_CTX_MD(this->context),NULL) != 1)
  {
    EVP_MD_CTX_destroy(context_copy);

    throw_error(CRYPTO_DIGEST_VALUE_ERROR);
  }

  // - ERROR -
  if (EVP_MD_CTX_copy_ex(context_copy,this->context) != 1)
  {
    EVP_MD_CTX_destroy(context_copy);

    throw_error(CRYPTO_DIGEST_VALUE_ERROR);
  }

  // - prepare result buffer -
  bc_array_s_reserve(a_trg,EVP_MD_CTX_size(context_copy));

  // - ERROR -
  unsigned dg_length;
  if (EVP_DigestFinal_ex(context_copy,(unsigned char *)a_trg->data + a_trg->used,&dg_length) != 1)
  {
    EVP_MD_CTX_destroy(context_copy);

    throw_error(CRYPTO_DIGEST_VALUE_ERROR);
  }

  EVP_MD_CTX_destroy(context_copy);

  // - adjust result buffer size -
  a_trg->used += dg_length;

  return 0;
}/*}}}*/

// === methods of structure crypto_sign_s ======================================

int crypto_sign_s_create_pkey_ctx(crypto_sign_s *this,
    crypto_digest_info_s *a_digest_info,crypto_pkey_s *a_pkey,EVP_PKEY_CTX **a_pkey_ctx)
{/*{{{*/
  crypto_sign_s_clear(this);

  // - ERROR -
  if ((this->context = EVP_MD_CTX_create()) == NULL)
  {
    throw_error(CRYPTO_SIGN_CREATE_INIT_ERROR);
  }

  /* - ERROR - */
  if (EVP_DigestSignInit(this->context,a_pkey_ctx,*a_digest_info,NULL,a_pkey->pkey) != 1)
  {
    throw_error(CRYPTO_SIGN_CREATE_INIT_ERROR);
  }

  this->pkey = a_pkey->pkey;

  return 0;
}/*}}}*/

int crypto_sign_s_value(crypto_sign_s *this,bc_array_s *a_trg)
{/*{{{*/

  // - ERROR -
  EVP_MD_CTX *context_copy;
  if ((context_copy = EVP_MD_CTX_create()) == NULL)
  {
    throw_error(CRYPTO_SIGN_VALUE_ERROR);
  }

  // - ERROR -
  if (EVP_DigestSignInit(context_copy,NULL,EVP_MD_CTX_MD(this->context),NULL,this->pkey) != 1)
  {
    EVP_MD_CTX_destroy(context_copy);

    throw_error(CRYPTO_DIGEST_VALUE_ERROR);
  }

  // - ERROR -
  if (EVP_MD_CTX_copy_ex(context_copy,this->context) != 1)
  {
    EVP_MD_CTX_destroy(context_copy);

    throw_error(CRYPTO_DIGEST_VALUE_ERROR);
  }

  // - ERROR -
  size_t dg_length = 0;
  if (EVP_DigestSignFinal(context_copy,NULL,&dg_length) != 1)
  {
    EVP_MD_CTX_destroy(context_copy);

    throw_error(CRYPTO_DIGEST_VALUE_ERROR);
  }

  // - prepare result buffer -
  bc_array_s_reserve(a_trg,dg_length);

  // - ERROR -
  if (EVP_DigestSignFinal(context_copy,(unsigned char *)a_trg->data + a_trg->used,&dg_length) != 1)
  {
    EVP_MD_CTX_destroy(context_copy);

    throw_error(CRYPTO_DIGEST_VALUE_ERROR);
  }

  EVP_MD_CTX_destroy(context_copy);

  // - adjust result buffer size -
  a_trg->used += dg_length;

  return 0;
}/*}}}*/

// === methods of structure crypto_verify_s ====================================

int crypto_verify_s_create(crypto_verify_s *this,
    crypto_digest_info_s *a_digest_info,crypto_pkey_s *a_pkey)
{/*{{{*/
  crypto_verify_s_clear(this);

  // - ERROR -
  if ((this->context = EVP_MD_CTX_create()) == NULL)
  {
    throw_error(CRYPTO_VERIFY_CREATE_INIT_ERROR);
  }

  /* - ERROR - */
  if (EVP_DigestVerifyInit(this->context,NULL,*a_digest_info,NULL,a_pkey->pkey) != 1)
  {
    throw_error(CRYPTO_VERIFY_CREATE_INIT_ERROR);
  }

  this->pkey = a_pkey->pkey;

  return 0;
}/*}}}*/

int crypto_verify_s_verify(crypto_verify_s *this,const char *a_data,unsigned a_size,int *a_result)
{/*{{{*/

  // - ERROR -
  EVP_MD_CTX *context_copy;
  if ((context_copy = EVP_MD_CTX_create()) == NULL)
  {
    throw_error(CRYPTO_VERIFY_VERIFY_ERROR);
  }

  // - ERROR -
  if (EVP_DigestVerifyInit(context_copy,NULL,EVP_MD_CTX_MD(this->context),NULL,this->pkey) != 1)
  {
    EVP_MD_CTX_destroy(context_copy);

    throw_error(CRYPTO_VERIFY_VERIFY_ERROR);
  }

  // - ERROR -
  if (EVP_MD_CTX_copy_ex(context_copy,this->context) != 1)
  {
    EVP_MD_CTX_destroy(context_copy);

    throw_error(CRYPTO_VERIFY_VERIFY_ERROR);
  }

  long long result = EVP_DigestVerifyFinal(context_copy,(unsigned char *)a_data,a_size);

  EVP_MD_CTX_destroy(context_copy);

  // - ERROR -
  if (result & ~0x01)
  {
    throw_error(CRYPTO_VERIFY_VERIFY_ERROR);
  }

  *a_result = !!result;

  return 0;
}/*}}}*/

// === methods of structure crypto_cipher_info_s ===============================

// === methods of structure crypto_encrypt_s ===================================

int crypto_encrypt_s_create(crypto_encrypt_s *this,
    crypto_cipher_info_s *a_cipher_info,
    const char *a_key,unsigned a_key_length,
    const char *a_iv,unsigned a_iv_length)
{/*{{{*/
  crypto_encrypt_s_clear(this);

  // - ERROR -
  if (a_key_length != (unsigned)EVP_CIPHER_key_length(*a_cipher_info))
  {
    throw_error(CRYPTO_CIPHER_INVALID_KEY_LENGTH);
  }

  // - ERROR -
  if (a_iv_length != (unsigned)EVP_CIPHER_iv_length(*a_cipher_info))
  {
    throw_error(CRYPTO_CIPHER_INVALID_INIT_VECTOR_LENGTH);
  }

  // - ERROR -
  if ((this->context = EVP_CIPHER_CTX_new()) == NULL)
  {
    throw_error(CRYPTO_CIPHER_NEW_INIT_ERROR);
  }

  // - ERROR -
  if (EVP_EncryptInit_ex(this->context,*a_cipher_info,NULL,
        (unsigned char *)a_key,(unsigned char *)a_iv) != 1)
  {
    crypto_encrypt_s_clear(this);

    throw_error(CRYPTO_CIPHER_NEW_INIT_ERROR);
  }

  return 0;
}/*}}}*/

int crypto_encrypt_s_finalize(crypto_encrypt_s *this,bc_array_s *a_trg)
{/*{{{*/

  // - ERROR -
  EVP_CIPHER_CTX *context_copy;
  if ((context_copy = EVP_CIPHER_CTX_new()) == NULL)
  {
    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  // - ERROR -
  if (EVP_EncryptInit_ex(context_copy,EVP_CIPHER_CTX_cipher(this->context),NULL,NULL,NULL) != 1)
  {
    EVP_CIPHER_CTX_free(context_copy);

    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  // - ERROR -
  if (EVP_CIPHER_CTX_copy(context_copy,this->context) != 1)
  {
    EVP_CIPHER_CTX_free(context_copy);

    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  bc_array_s_reserve(a_trg,EVP_CIPHER_CTX_block_size(context_copy));

  // - ERROR -
  int result_length;
  if (EVP_EncryptFinal_ex(context_copy,(unsigned char *)a_trg->data + a_trg->used,&result_length) != 1)
  {
    EVP_CIPHER_CTX_free(context_copy);

    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  EVP_CIPHER_CTX_free(context_copy);

  a_trg->used += result_length;

  return 0;
}/*}}}*/

// === methods of structure crypto_decrypt_s ===================================

int crypto_decrypt_s_create(crypto_decrypt_s *this,
    crypto_cipher_info_s *a_cipher_info,
    const char *a_key,unsigned a_key_length,
    const char *a_iv,unsigned a_iv_length)
{/*{{{*/
  crypto_decrypt_s_clear(this);

  // - ERROR -
  if (a_key_length != (unsigned)EVP_CIPHER_key_length(*a_cipher_info))
  {
    throw_error(CRYPTO_CIPHER_INVALID_KEY_LENGTH);
  }

  // - ERROR -
  if (a_iv_length != (unsigned)EVP_CIPHER_iv_length(*a_cipher_info))
  {
    throw_error(CRYPTO_CIPHER_INVALID_INIT_VECTOR_LENGTH);
  }

  // - ERROR -
  if ((this->context = EVP_CIPHER_CTX_new()) == NULL)
  {
    throw_error(CRYPTO_CIPHER_NEW_INIT_ERROR);
  }

  // - ERROR -
  if (EVP_DecryptInit_ex(this->context,*a_cipher_info,NULL,
        (unsigned char *)a_key,(unsigned char *)a_iv) != 1)
  {
    crypto_decrypt_s_clear(this);

    throw_error(CRYPTO_CIPHER_NEW_INIT_ERROR);
  }

  return 0;
}/*}}}*/

int crypto_decrypt_s_finalize(crypto_decrypt_s *this,bc_array_s *a_trg)
{/*{{{*/

  // - ERROR -
  EVP_CIPHER_CTX *context_copy;
  if ((context_copy = EVP_CIPHER_CTX_new()) == NULL)
  {
    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  // - ERROR -
  if (EVP_DecryptInit_ex(context_copy,EVP_CIPHER_CTX_cipher(this->context),NULL,NULL,NULL) != 1)
  {
    EVP_CIPHER_CTX_free(context_copy);

    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  // - ERROR -
  if (EVP_CIPHER_CTX_copy(context_copy,this->context) != 1)
  {
    EVP_CIPHER_CTX_free(context_copy);

    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  bc_array_s_reserve(a_trg,EVP_CIPHER_CTX_block_size(context_copy));

  // - ERROR -
  int result_length;
  if (EVP_DecryptFinal_ex(context_copy,(unsigned char *)a_trg->data + a_trg->used,&result_length) != 1)
  {
    EVP_CIPHER_CTX_free(context_copy);

    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  EVP_CIPHER_CTX_free(context_copy);

  a_trg->used += result_length;

  return 0;
}/*}}}*/

// === methods of structure crypto_seal_s ======================================

int crypto_seal_s_create(crypto_seal_s *this,
    crypto_cipher_info_s *a_cipher_info,crypto_pkey_s *a_pubk,unsigned a_npubk)
{/*{{{*/
  crypto_seal_s_clear(this);

  debug_assert(a_npubk >= 1);

  EVP_PKEY *pubk[a_npubk];
  unsigned char *ek[a_npubk];
  int ekl[a_npubk];

  {
    crypto_pkey_s *pk_ptr = a_pubk;
    crypto_pkey_s *pk_ptr_end = pk_ptr + a_npubk;
    EVP_PKEY **pubk_ptr = pubk;
    unsigned char **ek_ptr = ek;
    do {

      // - ERROR -
      if (!pk_ptr->ispub)
      {
        crypto_seal_s_clear(this);

        throw_error(CRYPTO_CIPHER_INVALID_PRIVATE_PUBLIC_KEY);
      }

      bc_arrays_s_push_blank(&this->keys);
      bc_array_s *key_ptr = bc_arrays_s_last(&this->keys);

      bc_array_s_copy_resize(key_ptr,EVP_PKEY_size(pk_ptr->pkey));
      key_ptr->used = key_ptr->size;

      *pubk_ptr = pk_ptr->pkey;
      *ek_ptr = (unsigned char *)key_ptr->data;

    } while(++pubk_ptr,++ek_ptr,++pk_ptr < pk_ptr_end);
  }

  bc_array_s_copy_resize(&this->iv,crypto_cipher_info_s_iv_length(a_cipher_info));
  this->iv.used = this->iv.size;

  // - ERROR -
  if ((this->context = EVP_CIPHER_CTX_new()) == NULL)
  {
    crypto_seal_s_clear(this);

    throw_error(CRYPTO_CIPHER_NEW_INIT_ERROR);
  }

  // - ERROR -
  if (EVP_SealInit(this->context,*a_cipher_info,ek,ekl,(unsigned char *)this->iv.data,pubk,a_npubk) == 0)
  {
    crypto_seal_s_clear(this);

    throw_error(CRYPTO_CIPHER_NEW_INIT_ERROR);
  }

  // - adjust keys lengths -
  {
    bc_array_s *key_ptr = this->keys.data;
    bc_array_s *key_ptr_end = key_ptr + a_npubk;
    int *ekl_ptr = ekl;
    do {
      key_ptr->used = *ekl_ptr;
    } while(++ekl_ptr,++key_ptr < key_ptr_end);
  }

  return 0;
}/*}}}*/

int crypto_seal_s_finalize(crypto_seal_s *this,bc_array_s *a_trg)
{/*{{{*/

  // - ERROR -
  EVP_CIPHER_CTX *context_copy;
  if ((context_copy = EVP_CIPHER_CTX_new()) == NULL)
  {
    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  // - ERROR -
  if (EVP_SealInit(context_copy,EVP_CIPHER_CTX_cipher(this->context),NULL,NULL,NULL,NULL,0) == 0)
  {
    EVP_CIPHER_CTX_free(context_copy);

    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  // - ERROR -
  if (EVP_CIPHER_CTX_copy(context_copy,this->context) != 1)
  {
    EVP_CIPHER_CTX_free(context_copy);

    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  bc_array_s_reserve(a_trg,EVP_CIPHER_CTX_block_size(context_copy));

  // - ERROR -
  int result_length;
  if (EVP_SealFinal(context_copy,(unsigned char *)a_trg->data + a_trg->used,&result_length) != 1)
  {
    EVP_CIPHER_CTX_free(context_copy);

    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  EVP_CIPHER_CTX_free(context_copy);

  a_trg->used += result_length;

  return 0;
}/*}}}*/

// === methods of structure crypto_open_s ======================================

int crypto_open_s_create(crypto_open_s *this,
    crypto_cipher_info_s *a_cipher_info,
    const char *a_key,unsigned a_key_length,
    const char *a_iv,unsigned a_iv_length,
    crypto_pkey_s *a_priv)
{/*{{{*/
  crypto_open_s_clear(this);

  // - ERROR -
  if (a_iv_length != crypto_cipher_info_s_iv_length(a_cipher_info))
  {
    throw_error(CRYPTO_CIPHER_INVALID_INIT_VECTOR_LENGTH);
  }

  // - ERROR -
  if (a_priv->ispub)
  {
    throw_error(CRYPTO_CIPHER_INVALID_PRIVATE_PUBLIC_KEY);
  }

  // - ERROR -
  if ((this->context = EVP_CIPHER_CTX_new()) == NULL)
  {
    throw_error(CRYPTO_CIPHER_NEW_INIT_ERROR);
  }

  // - ERROR -
  if (EVP_OpenInit(this->context,*a_cipher_info,
        (unsigned char *)a_key,a_key_length,(unsigned char *)a_iv,a_priv->pkey) == 0)
  {
    crypto_open_s_clear(this);

    throw_error(CRYPTO_CIPHER_NEW_INIT_ERROR);
  }

  return 0;
}/*}}}*/

int crypto_open_s_finalize(crypto_open_s *this,bc_array_s *a_trg)
{/*{{{*/

  // - ERROR -
  EVP_CIPHER_CTX *context_copy;
  if ((context_copy = EVP_CIPHER_CTX_new()) == NULL)
  {
    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  // - ERROR -
  if (EVP_OpenInit(context_copy,EVP_CIPHER_CTX_cipher(this->context),NULL,0,NULL,NULL) == 0)
  {
    EVP_CIPHER_CTX_free(context_copy);

    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  // - ERROR -
  if (EVP_CIPHER_CTX_copy(context_copy,this->context) != 1)
  {
    EVP_CIPHER_CTX_free(context_copy);

    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  bc_array_s_reserve(a_trg,EVP_CIPHER_CTX_block_size(context_copy));

  // - ERROR -
  int result_length;
  if (EVP_OpenFinal(context_copy,(unsigned char *)a_trg->data + a_trg->used,&result_length) != 1)
  {
    EVP_CIPHER_CTX_free(context_copy);

    throw_error(CRYPTO_CIPHER_FINALIZE_ERROR);
  }

  EVP_CIPHER_CTX_free(context_copy);

  a_trg->used += result_length;

  return 0;
}/*}}}*/

// === global functions ========================================================

void crypto_random(int a_count,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_reserve(a_trg,a_count);

  RAND_bytes((unsigned char *)a_trg->data + a_trg->used,a_count);

  a_trg->used += a_count;
}/*}}}*/

void crypto_encode_base16(const char *a_data,unsigned a_size,bc_array_s *a_trg)
{/*{{{*/
  if (a_size != 0)
  {
    bc_array_s_reserve(a_trg,a_size << 1);

    unsigned char *s_ptr = (unsigned char *)a_data;
    unsigned char *s_ptr_end = s_ptr + a_size;
    unsigned char *t_ptr = (unsigned char *)a_trg->data + a_trg->used;

    do {
      *t_ptr++ = c_base16_map[*s_ptr >> 4];
      *t_ptr++ = c_base16_map[*s_ptr & 0x0f];
    } while(++s_ptr < s_ptr_end);

    a_trg->used += a_size << 1;
  }
}/*}}}*/

int crypto_decode_base16(const char *a_data,unsigned a_size,bc_array_s *a_trg)
{/*{{{*/

  // - ERROR -
  if (a_size & 0x01)
  {
    throw_error(CRYPTO_INVALID_BASE_DATA_SIZE);
  }

  if (a_size != 0)
  {
    bc_array_s_reserve(a_trg,a_size >> 1);

    unsigned char *s_ptr = (unsigned char *)a_data;
    unsigned char *s_ptr_end = s_ptr + a_size;
    unsigned char *t_ptr = (unsigned char *)a_trg->data + a_trg->used;

#define BIC_CRYPTO_METHOD_DECODE_BASE16_CHARACTER(OPERATOR) \
{/*{{{*/\
  unsigned char ch = *s_ptr++;\
\
  if (ch >= '0' && ch <= '9')\
  {\
    *t_ptr OPERATOR ch - '0';\
  }\
  else if (ch >= 'a' && ch <= 'f')\
  {\
    *t_ptr OPERATOR 10 + (ch - 'a');\
  }\
  else if (ch >= 'A' && ch <= 'F')\
  {\
    *t_ptr OPERATOR 10 + (ch - 'A');\
  }\
\
  /* - ERROR - */\
  else\
  {\
    throw_error(CRYPTO_INVALID_BASE_DATA);\
  }\
}/*}}}*/

    do {
      BIC_CRYPTO_METHOD_DECODE_BASE16_CHARACTER(=);
      *t_ptr <<= 4;
      BIC_CRYPTO_METHOD_DECODE_BASE16_CHARACTER(+=);
    } while(++t_ptr,s_ptr < s_ptr_end);

    a_trg->used += a_size >> 1;
  }

  return 0;
}/*}}}*/

void crypto_encode_base64(const char *a_data,unsigned a_size,bc_array_s *a_trg)
{/*{{{*/
  if (a_size != 0)
  {
    unsigned encode_size = ((a_size/3 + 1) << 2) + 1;
    bc_array_s_reserve(a_trg,encode_size);

    a_trg->used += EVP_EncodeBlock(
        (unsigned char *)a_trg->data + a_trg->used,
        (unsigned char *)a_data,
        a_size);
  }
}/*}}}*/

int crypto_decode_base64(const char *a_data,unsigned a_size,bc_array_s *a_trg)
{/*{{{*/

  // - ERROR -
  if (a_size & 0x03)
  {
    throw_error(CRYPTO_INVALID_BASE_DATA_SIZE);
  }

  if (a_size != 0)
  {
    unsigned decode_size = ((a_size >> 2) * 3) + 1;
    bc_array_s_reserve(a_trg,decode_size);

    int length = EVP_DecodeBlock(
        (unsigned char *)a_trg->data + a_trg->used,
        (unsigned char *)a_data,
        a_size);

    // - ERROR -
    if (length == -1)
    {
      throw_error(CRYPTO_INVALID_BASE_DATA);
    }

    // - adjust target length according to input padding characters -
    if (a_data[a_size - 2] == '=')
    {
      length -= 2;
    }
    else if (a_data[a_size - 1] == '=')
    {
      length -= 1;
    }

    a_trg->used += length;
  }

  return 0;
}/*}}}*/

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

