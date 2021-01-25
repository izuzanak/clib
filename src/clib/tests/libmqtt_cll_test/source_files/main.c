
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libmqtt_cll_test";

const char *test_names[] =
{/*{{{*/
  "varint",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_varint,
};/*}}}*/

// === test execution functions ================================================

void test_varint()
{/*{{{*/
  CONT_INIT_CLEAR(bc_array_s,buffer);
  const char *end;
  uint32_t value;

  buffer.used = 0;
  cassert(mqtt_var_byte_enc(0,&buffer) == 0);
  cassert(memcmp(buffer.data,"\x00",1) == 0);
  cassert(mqtt_var_byte_dec(buffer.data,buffer.data + buffer.used,&end,&value) == 0);
  cassert(value == 0);

  buffer.used = 0;
  cassert(mqtt_var_byte_enc(127,&buffer) == 0);
  cassert(memcmp(buffer.data,"\x7f",1) == 0);
  cassert(mqtt_var_byte_dec(buffer.data,buffer.data + buffer.used,&end,&value) == 0);
  cassert(value == 127);

  buffer.used = 0;
  cassert(mqtt_var_byte_enc(128,&buffer) == 0);
  cassert(memcmp(buffer.data,"\x80\x01",2) == 0);
  cassert(mqtt_var_byte_dec(buffer.data,buffer.data + buffer.used,&end,&value) == 0);
  cassert(value == 128);

  buffer.used = 0;
  cassert(mqtt_var_byte_enc(16383,&buffer) == 0);
  cassert(memcmp(buffer.data,"\xff\x7f",2) == 0);
  cassert(mqtt_var_byte_dec(buffer.data,buffer.data + buffer.used,&end,&value) == 0);
  cassert(value == 16383);

  buffer.used = 0;
  cassert(mqtt_var_byte_enc(16384,&buffer) == 0);
  cassert(memcmp(buffer.data,"\x80\x80\x01",3) == 0);
  cassert(mqtt_var_byte_dec(buffer.data,buffer.data + buffer.used,&end,&value) == 0);
  cassert(value == 16384);

  buffer.used = 0;
  cassert(mqtt_var_byte_enc(2097151,&buffer) == 0);
  cassert(memcmp(buffer.data,"\xff\xff\x7f",3) == 0);
  cassert(mqtt_var_byte_dec(buffer.data,buffer.data + buffer.used,&end,&value) == 0);
  cassert(value == 2097151);

  buffer.used = 0;
  cassert(mqtt_var_byte_enc(2097152,&buffer) == 0);
  cassert(memcmp(buffer.data,"\x80\x80\x80\x01",4) == 0);
  cassert(mqtt_var_byte_dec(buffer.data,buffer.data + buffer.used,&end,&value) == 0);
  cassert(value == 2097152);

  buffer.used = 0;
  cassert(mqtt_var_byte_enc(268435455,&buffer) == 0);
  cassert(memcmp(buffer.data,"\xff\xff\xff\x7f",4) == 0);
  cassert(mqtt_var_byte_dec(buffer.data,buffer.data + buffer.used,&end,&value) == 0);
  cassert(value == 268435455);

  uint32_t idx = 0;
  do {
    uint32_t number = rand() & 0x0fffffff; // NOLINT(cert-msc30-c,cert-msc50-cpp)

    buffer.used = 0;
    cassert(mqtt_var_byte_enc(number,&buffer) == 0);
    cassert(mqtt_var_byte_dec(buffer.data,buffer.data + buffer.used,&end,&value) == 0);
    cassert(value == number);

  } while(++idx < 1000);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

