
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
  cassert(strcmp(data.data,"05e60383fdbeb9784b9b9f50f8897ef0e10872853db311fa0e0a0c98e07b1d8b") == 0);
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
  printf("signature: %.*s\n",signature_base16.used,signature_base16.data);
  cassert(strcmp(signature_base16.data,
        "32461f41d2784b3ddae89c34d87a89129481782d16c5d9e57c993b101989cfa83fb7af5735dfd444"
        "581734a54e2e780f954bb2f937e868e2e139d0740d5ac0e3057b273b2b17812e2a655ef16b021b1f"
        "74bebc2e2307ca5f55f5ac3197e202bd0242670c7df4ed40fa0775cfd124922da0a6bbe152adb558"
        "2913c8376b1b8384") == 0);

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

