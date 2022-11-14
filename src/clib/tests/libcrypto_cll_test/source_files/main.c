
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libcrypto_cll_test";

const char *test_names[] =
{/*{{{*/
  "random",
  "base16",
  "base64",
  "pkey",
  "digest",
  "sign",
  "cipher",
  "seal",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_random,
  test_base16,
  test_base64,
  test_pkey,
  test_digest,
  test_sign,
  test_cipher,
  test_seal,
};/*}}}*/

// === test execution functions ================================================

void test_random()
{/*{{{*/
  CONT_INIT_CLEAR(bc_array_s,buffer);
  crypto_random(256,&buffer);
}/*}}}*/

void test_base16()
{/*{{{*/
  CONT_INIT_CLEAR(bc_array_s,data);
  crypto_random(512,&data);

  CONT_INIT_CLEAR(bc_array_s,base16);
  crypto_encode_base16(data.data,data.used,&base16);

  printf("base16: %.*s\n",base16.used,base16.data);

  CONT_INIT_CLEAR(bc_array_s,decoded);
  cassert(crypto_decode_base16(base16.data,base16.used,&decoded) == 0)
  cassert(bc_array_s_compare(&data,&decoded));
}/*}}}*/

void test_base64()
{/*{{{*/
  CONT_INIT_CLEAR(bc_array_s,data);
  crypto_random(513,&data);

  CONT_INIT_CLEAR(bc_array_s,base64);
  crypto_encode_base64(data.data,data.used,&base64);

  printf("base64: %.*s\n",base64.used,base64.data);

  CONT_INIT_CLEAR(bc_array_s,decoded);
  cassert(crypto_decode_base64(base64.data,base64.used,&decoded) == 0)
  cassert(bc_array_s_compare(&data,&decoded));
}/*}}}*/

void test_pkey()
{/*{{{*/
  CONT_INIT_CLEAR(crypto_pkey_s,private);
  cassert(crypto_pkey_s_load_private(&private,"tests/libcrypto_cll_test/resources/private.pem","password") == 0);

  CONT_INIT_CLEAR(crypto_pkey_s,public);
  cassert(crypto_pkey_s_load_public(&public,"tests/libcrypto_cll_test/resources/public.pem","password") == 0);
}/*}}}*/

void test_digest()
{/*{{{*/
  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libcrypto_cll_test/resources/file.xml","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,data);
  cassert(file_s_read_close(&file,&data) == 0);

  CONT_INIT_CLEAR(crypto_digest_info_s,digest_info);
  cassert(crypto_digest_info_s_get_by_name(&digest_info,"SHA256") == 0);

  printf("digest name: %s\n",crypto_digest_info_s_name(&digest_info));

  CONT_INIT_CLEAR(crypto_digest_s,digest);
  cassert(crypto_digest_s_create(&digest,&digest_info) == 0);
  cassert(crypto_digest_s_update(&digest,data.data,data.used) == 0);

  CONT_INIT_CLEAR(bc_array_s,digest_value);
  cassert(crypto_digest_s_value(&digest,&digest_value) == 0);

  data.used = 0;
  crypto_encode_base16(digest_value.data,digest_value.used,&data);
  bc_array_s_push(&data,'\0');
  printf("digest: %.*s\n",data.used,data.data);
  cassert(strcmp(data.data,"1bd5ec15650c4dc0c3b1cf48c8d32cc05664aa807a8d11651133a9fe3e6076be") == 0);
}/*}}}*/

void test_sign()
{/*{{{*/
  CONT_INIT_CLEAR(crypto_pkey_s,private);
  cassert(crypto_pkey_s_load_private(&private,"tests/libcrypto_cll_test/resources/private.pem","password") == 0);

  CONT_INIT_CLEAR(crypto_pkey_s,public);
  cassert(crypto_pkey_s_load_public(&public,"tests/libcrypto_cll_test/resources/public.pem","password") == 0);

  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libcrypto_cll_test/resources/file.xml","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,data);
  cassert(file_s_read_close(&file,&data) == 0);

  CONT_INIT_CLEAR(crypto_digest_info_s,digest_info);
  cassert(crypto_digest_info_s_get_by_name(&digest_info,"RSA-SHA256") == 0);

  printf("digest name: %s\n",crypto_digest_info_s_name(&digest_info));

  // - create signature -
  CONT_INIT_CLEAR(crypto_sign_s,sign);
  cassert(crypto_sign_s_create(&sign,&digest_info,&private) == 0);
  cassert(crypto_sign_s_update(&sign,data.data,data.used) == 0);

  CONT_INIT_CLEAR(bc_array_s,signature);
  cassert(crypto_sign_s_value(&sign,&signature) == 0);

  CONT_INIT_CLEAR(bc_array_s,signature_base16);
  crypto_encode_base16(signature.data,signature.used,&signature_base16);
  bc_array_s_push(&signature_base16,'\0');
  printf("singature: %.*s\n",signature_base16.used,signature_base16.data);
  cassert(strcmp(signature_base16.data,
"15476b81b7da6fac5a3bd341266c37f4e7f9179efec5a5454d8d5b93fae02194f34b08b94af713b12e09baab4b43b39d14826"
"0fd348ad983ada3cafec39ad49860a72c69e1947ced821780548b3592a473af2a1142865af6d9400933e93ca564d55e7fff5c"
"0e27990db365b93e4dcfd98a2375a32656c41ad525394d1e64920a") == 0);

  // - verify signature -
  CONT_INIT_CLEAR(crypto_verify_s,verify);
  cassert(crypto_verify_s_create(&verify,&digest_info,&public) == 0);
  cassert(crypto_verify_s_update(&verify,data.data,data.used) == 0);

  int result;
  cassert(crypto_verify_s_verify(&verify,signature.data,signature.used,&result) == 0);
  printf("verification result: %d\n",result);
  cassert(result);
}/*}}}*/

void test_cipher()
{/*{{{*/
  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libcrypto_cll_test/resources/file.xml","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,data);
  cassert(file_s_read_close(&file,&data) == 0);

  CONT_INIT_CLEAR(crypto_cipher_info_s,cipher_info);
  cassert(crypto_cipher_info_s_get_by_name(&cipher_info,"AES-256-CBC") == 0);

  printf("cipher name: %s\n",crypto_cipher_info_s_name(&cipher_info));
  printf("cipher block_size: %u\n",crypto_cipher_info_s_block_size(&cipher_info));
  printf("cipher key_length: %u\n",crypto_cipher_info_s_key_length(&cipher_info));
  printf("cipher iv_length: %u\n",crypto_cipher_info_s_iv_length(&cipher_info));

  CONT_INIT_CLEAR(bc_array_s,key);
  crypto_random(32,&key);

  CONT_INIT_CLEAR(bc_array_s,iv);
  crypto_random(16,&iv);

  CONT_INIT_CLEAR(crypto_encrypt_s,encrypt);
  cassert(crypto_encrypt_s_create(&encrypt,&cipher_info,key.data,key.used,iv.data,iv.used) == 0);

  CONT_INIT_CLEAR(bc_array_s,encrypted_data);
  cassert(crypto_encrypt_s_update(&encrypt,data.data,data.used,&encrypted_data) == 0);
  cassert(crypto_encrypt_s_finalize(&encrypt,&encrypted_data) == 0);

  CONT_INIT_CLEAR(crypto_decrypt_s,decrypt);
  cassert(crypto_decrypt_s_create(&decrypt,&cipher_info,key.data,key.used,iv.data,iv.used) == 0);

  CONT_INIT_CLEAR(bc_array_s,decrypted_data);
  cassert(crypto_decrypt_s_update(&decrypt,encrypted_data.data,encrypted_data.used,&decrypted_data) == 0);
  cassert(crypto_decrypt_s_finalize(&decrypt,&decrypted_data) == 0);

  cassert(bc_array_s_compare(&data,&decrypted_data));
}/*}}}*/

void test_seal()
{/*{{{*/
  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libcrypto_cll_test/resources/file.xml","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,data);
  cassert(file_s_read_close(&file,&data) == 0);

  CONT_INIT_CLEAR(crypto_cipher_info_s,cipher_info);
  cassert(crypto_cipher_info_s_get_by_name(&cipher_info,"AES-256-CBC") == 0);

  printf("cipher name: %s\n",crypto_cipher_info_s_name(&cipher_info));
  printf("cipher block_size: %u\n",crypto_cipher_info_s_block_size(&cipher_info));
  printf("cipher key_length: %u\n",crypto_cipher_info_s_key_length(&cipher_info));
  printf("cipher iv_length: %u\n",crypto_cipher_info_s_iv_length(&cipher_info));

  CONT_INIT_CLEAR(crypto_pkey_s,private);
  cassert(crypto_pkey_s_load_private(&private,"tests/libcrypto_cll_test/resources/private.pem","password") == 0);

  CONT_INIT_CLEAR(crypto_pkey_s,public);
  cassert(crypto_pkey_s_load_public(&public,"tests/libcrypto_cll_test/resources/public.pem","password") == 0);

  CONT_INIT_CLEAR(crypto_seal_s,seal);
  cassert(crypto_seal_s_create(&seal,&cipher_info,&public,1) == 0);

  CONT_INIT_CLEAR(bc_array_s,sealed_data);
  cassert(crypto_seal_s_update(&seal,data.data,data.used,&sealed_data) == 0);
  cassert(crypto_seal_s_finalize(&seal,&sealed_data) == 0);

  bc_array_s *key = seal.keys.data;
  bc_array_s *iv = &seal.iv;

  CONT_INIT_CLEAR(crypto_open_s,open);
  cassert(crypto_open_s_create(&open,&cipher_info,key->data,key->used,iv->data,iv->used,&private) == 0);

  CONT_INIT_CLEAR(bc_array_s,decrypted_data);
  cassert(crypto_open_s_update(&open,sealed_data.data,sealed_data.used,&decrypted_data) == 0);
  cassert(crypto_open_s_finalize(&open,&decrypted_data) == 0);

  cassert(bc_array_s_compare(&data,&decrypted_data));
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libcrypto_cll_init(),libcrypto_cll_clear());

  return 0;
}/*}}}*/

