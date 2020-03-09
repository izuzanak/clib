
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libsrtp_cll_test";

const char *test_names[] =
{/*{{{*/
  "srtp",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_srtp,
};/*}}}*/

// === test execution functions ================================================

void test_srtp()
{/*{{{*/
  CONT_INIT_CLEAR(srtp_policy_s,policy);
  srtp_policy_s_crypto_rtp_default(&policy);
  srtp_policy_s_crypto_rtcp_default(&policy);

  uint8_t key[30] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D};

  policy.ssrc.type = ssrc_any_inbound;
  policy.ssrc.value = 0;

  policy.key = key;

  // FIXME TODO continue ...

}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libsrtp_cll_init(),libsrtp_cll_clear());

  return 0;
}/*}}}*/

