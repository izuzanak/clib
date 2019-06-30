
@begin
include "cl_crypto.h"
@end

const char c_base16_map[] = "0123456789abcdef";

// === methods of structure crypto_pkey_s ======================================

int crypto_pkey_s_password_cb(char *buf,int size,int rwflag,void *userdata)
{/*{{{*/
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
  if (EVP_DigestInit_ex(context_copy,EVP_MD_CTX_md(this->context),NULL) != 1)
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

// === methods of structure crypto_cipher_info_s ===============================

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

