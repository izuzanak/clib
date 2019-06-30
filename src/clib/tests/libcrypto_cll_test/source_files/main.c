
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
  "digest_info",
  "digest",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_random,
  test_base16,
  test_base64,
  test_pkey,
  test_digest_info,
  test_digest,
};/*}}}*/

// === methods of generated structures =========================================

// -- crypto_digest_info_array_s --
@begin
methods crypto_digest_info_array_s
@end

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
  cassert(crypto_pkey_s_load_public(&private,"tests/libcrypto_cll_test/resources/public.pem","password") == 0);
}/*}}}*/

void test_digest_info()
{/*{{{*/
  CONT_INIT_CLEAR(crypto_digest_info_s,digest_info);
  cassert(crypto_digest_info_s_get_by_name(&digest_info,"sha256") == 0);

  CONT_INIT_CLEAR(crypto_digest_info_array_s,digest_infos);
  unsigned idx = 0;
  do {
    crypto_digest_info_array_s_push(&digest_infos,&digest_info);
  } while(++idx < 10);
}/*}}}*/

void test_digest()
{/*{{{*/
  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libsnappy_cll_test/resources/file.xml","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,data);
  cassert(file_s_read_close(&file,&data) == 0);

  CONT_INIT_CLEAR(crypto_digest_info_s,digest_info);
  cassert(crypto_digest_info_s_get_by_name(&digest_info,"sha256") == 0);

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

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libcrypto_cll_init(),libcrypto_cll_clear());

  return 0;
}/*}}}*/

