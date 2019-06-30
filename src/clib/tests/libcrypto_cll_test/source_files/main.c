
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
  "pkey_load",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_random,
  test_base16,
  test_base64,
  test_pkey_load,
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

void test_pkey_load()
{/*{{{*/
  CONT_INIT_CLEAR(crypto_pkey_s,private);
  cassert(crypto_pkey_s_load_private(&private,"tests/libcrypto_cll_test/resources/private.pem","password") == 0);

  CONT_INIT_CLEAR(crypto_pkey_s,public);
  cassert(crypto_pkey_s_load_public(&private,"tests/libcrypto_cll_test/resources/public.pem","password") == 0);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libcrypto_cll_init(),libcrypto_cll_clear());

  return 0;
}/*}}}*/

