
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libpack_cll_test";

const char *test_names[] =
{/*{{{*/
  "pack_code",
  "pack_decode",
  "pack_roundtrip",
  "pack_endian",
  "pack_string",
  "pack_count",
  "pack_var",
  "pack_calc_size",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_pack_code,
  test_pack_decode,
  test_pack_roundtrip,
  test_pack_endian,
  test_pack_string,
  test_pack_count,
  test_pack_var,
  test_pack_calc_size,
};/*}}}*/

// === test execution functions ================================================

void test_pack_code()
{/*{{{*/

  // - test basic big-endian encode of QHH -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    ulli id = 0x0102030405060708ULL;
    usi type = 0x090a;
    usi req = 0x0b0c;
    cassert(pack_s_code(&buffer,">QHH",id,type,req) == 0);
    cassert(buffer.used == 12);

    // - verify big-endian byte order -
    cassert((unsigned char)buffer.data[0] == 0x01);
    cassert((unsigned char)buffer.data[1] == 0x02);
    cassert((unsigned char)buffer.data[2] == 0x03);
    cassert((unsigned char)buffer.data[3] == 0x04);
    cassert((unsigned char)buffer.data[4] == 0x05);
    cassert((unsigned char)buffer.data[5] == 0x06);
    cassert((unsigned char)buffer.data[6] == 0x07);
    cassert((unsigned char)buffer.data[7] == 0x08);
    cassert((unsigned char)buffer.data[8] == 0x09);
    cassert((unsigned char)buffer.data[9] == 0x0a);
    cassert((unsigned char)buffer.data[10] == 0x0b);
    cassert((unsigned char)buffer.data[11] == 0x0c);
  }

  // - test encoding different types -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    cassert(pack_s_code(&buffer,">BHI",(unsigned char)0xab,(unsigned short)0x1234,(unsigned)0x56789abc) == 0);
    cassert(buffer.used == 7);
    cassert((unsigned char)buffer.data[0] == 0xab);
    cassert((unsigned char)buffer.data[1] == 0x12);
    cassert((unsigned char)buffer.data[2] == 0x34);
    cassert((unsigned char)buffer.data[3] == 0x56);
    cassert((unsigned char)buffer.data[4] == 0x78);
    cassert((unsigned char)buffer.data[5] == 0x9a);
    cassert((unsigned char)buffer.data[6] == 0xbc);
  }

  // - test empty format string -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    cassert(pack_s_code(&buffer,"") == 0);
    cassert(buffer.used == 0);
  }
}/*}}}*/

void test_pack_decode()
{/*{{{*/

  // - test basic big-endian decode of QHH -
  {
    unsigned char data[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c};
    ulli id;
    usi type;
    usi req;
    unsigned bytes_read;
    cassert(pack_s_decode(">QHH",(const char *)data,sizeof(data),&bytes_read,&id,&type,&req) == 0);
    cassert(bytes_read == 12);
    cassert(id == 0x0102030405060708ULL);
    cassert(type == 0x090a);
    cassert(req == 0x0b0c);
  }

  // - test decoding different types -
  {
    unsigned char data[] = {0xab,0x12,0x34,0x56,0x78,0x9a,0xbc};
    unsigned char b_val;
    unsigned short h_val;
    unsigned i_val;
    unsigned bytes_read;
    cassert(pack_s_decode(">BHI",(const char *)data,sizeof(data),&bytes_read,&b_val,&h_val,&i_val) == 0);
    cassert(bytes_read == 7);
    cassert(b_val == 0xab);
    cassert(h_val == 0x1234);
    cassert(i_val == 0x56789abc);
  }
}/*}}}*/

void test_pack_roundtrip()
{/*{{{*/

  // - test encode then decode roundtrip -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    ulli orig_id = 123456789012345ULL;
    usi orig_type = 42;
    usi orig_req = 7;

    cassert(pack_s_code(&buffer,">QHH",orig_id,orig_type,orig_req) == 0);

    ulli dec_id;
    usi dec_type;
    usi dec_req;
    unsigned bytes_read;
    cassert(pack_s_decode(">QHH",buffer.data,buffer.used,&bytes_read,&dec_id,&dec_type,&dec_req) == 0);

    cassert(dec_id == orig_id);
    cassert(dec_type == orig_type);
    cassert(dec_req == orig_req);
    cassert(bytes_read == buffer.used);
  }

  // - test float/double roundtrip -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    float orig_f = 3.14f;
    double orig_d = 2.718281828;

    cassert(pack_s_code(&buffer,">fd",orig_f,orig_d) == 0);
    cassert(buffer.used == sizeof(float) + sizeof(double));

    float dec_f;
    double dec_d;
    unsigned bytes_read;
    cassert(pack_s_decode(">fd",buffer.data,buffer.used,&bytes_read,&dec_f,&dec_d) == 0);

    cassert(dec_f == orig_f);
    cassert(dec_d == orig_d);
  }

  // - test signed types roundtrip -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    long long int orig_q = -123456789012345LL;
    int orig_i = -42;
    short orig_h = -1000;

    cassert(pack_s_code(&buffer,">qih",orig_q,orig_i,orig_h) == 0);

    long long int dec_q;
    int dec_i;
    short dec_h;
    unsigned bytes_read;
    cassert(pack_s_decode(">qih",buffer.data,buffer.used,&bytes_read,&dec_q,&dec_i,&dec_h) == 0);

    cassert(dec_q == orig_q);
    cassert(dec_i == orig_i);
    cassert(dec_h == orig_h);
  }
}/*}}}*/

void test_pack_endian()
{/*{{{*/

  // - test big endian -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    cassert(pack_s_code(&buffer,">H",(unsigned short)0x0102) == 0);
    cassert(buffer.used == 2);
    cassert((unsigned char)buffer.data[0] == 0x01);
    cassert((unsigned char)buffer.data[1] == 0x02);
  }

  // - test little endian -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    cassert(pack_s_code(&buffer,"<H",(unsigned short)0x0102) == 0);
    cassert(buffer.used == 2);
    cassert((unsigned char)buffer.data[0] == 0x02);
    cassert((unsigned char)buffer.data[1] == 0x01);
  }

  // - test native endian roundtrip -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    unsigned orig = 0x12345678;
    cassert(pack_s_code(&buffer,"=I",orig) == 0);

    unsigned dec;
    unsigned bytes_read;
    cassert(pack_s_decode("=I",buffer.data,buffer.used,&bytes_read,&dec) == 0);
    cassert(dec == orig);
  }

  // - test little endian decode -
  {
    unsigned char data[] = {0x78,0x56,0x34,0x12};
    unsigned val;
    unsigned bytes_read;
    cassert(pack_s_decode("<I",(const char *)data,sizeof(data),&bytes_read,&val) == 0);
    cassert(val == 0x12345678);
  }
}/*}}}*/

void test_pack_string()
{/*{{{*/

  // - test zero-terminated string encode/decode -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    CONT_INIT_CLEAR(string_s,str);
    string_s_set_ptr(&str,"hello");

    cassert(pack_s_code(&buffer,">Hz",(unsigned short)0x1234,&str) == 0);

    // - decode -
    unsigned short dec_val;
    CONT_INIT_CLEAR(string_s,dec_str);
    unsigned bytes_read;
    cassert(pack_s_decode(">Hz",buffer.data,buffer.used,&bytes_read,&dec_val,&dec_str) == 0);
    cassert(dec_val == 0x1234);
    cassert(dec_str.size - 1 == 5);
    cassert(memcmp(dec_str.data,"hello",5) == 0);
  }

  // - test zero-terminated string roundtrip -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    CONT_INIT_CLEAR(string_s,str);
    string_s_set_ptr(&str,"world");

    cassert(pack_s_code(&buffer,">z",&str) == 0);
    cassert(buffer.used == 6);

    CONT_INIT_CLEAR(string_s,dec_str);
    unsigned bytes_read;
    cassert(pack_s_decode(">z",buffer.data,buffer.used,&bytes_read,&dec_str) == 0);
    cassert(dec_str.size - 1 == 5);
    cassert(memcmp(dec_str.data,"world",5) == 0);
    cassert(bytes_read == 6);
  }

  // - test length-prefixed string roundtrip -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    CONT_INIT_CLEAR(string_s,str);
    string_s_set_ptr(&str,"test");

    cassert(pack_s_code(&buffer,"=s",&str) == 0);
    cassert(buffer.used == sizeof(unsigned) + 4);

    CONT_INIT_CLEAR(string_s,dec_str);
    unsigned bytes_read;
    cassert(pack_s_decode("=s",buffer.data,buffer.used,&bytes_read,&dec_str) == 0);
    cassert(dec_str.size - 1 == 4);
    cassert(memcmp(dec_str.data,"test",4) == 0);
  }
}/*}}}*/

void test_pack_count()
{/*{{{*/

  // - test count prefix -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    cassert(pack_s_code(&buffer,">3H",(unsigned short)1,(unsigned short)2,(unsigned short)3) == 0);
    cassert(buffer.used == 6);

    unsigned short v1,v2,v3;
    unsigned bytes_read;
    cassert(pack_s_decode(">3H",buffer.data,buffer.used,&bytes_read,&v1,&v2,&v3) == 0);
    cassert(v1 == 1);
    cassert(v2 == 2);
    cassert(v3 == 3);
  }

  // - test count with fixed-size string -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    CONT_INIT_CLEAR(string_s,str);
    string_s_set_ptr(&str,"hi");

    cassert(pack_s_code(&buffer,">8z",&str) == 0);
    cassert(buffer.used == 8);

    CONT_INIT_CLEAR(string_s,dec_str);
    unsigned bytes_read;
    cassert(pack_s_decode(">8z",buffer.data,buffer.used,&bytes_read,&dec_str) == 0);
    cassert(dec_str.size - 1 == 2);
    cassert(memcmp(dec_str.data,"hi",2) == 0);
    cassert(bytes_read == 8);
  }
}/*}}}*/

void test_pack_var()
{/*{{{*/

  // - test var_s encode -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    VAR_CLEAR(values,loc_s_array___locs(3,
      loc_s_int(0x0102030405060708LL),
      loc_s_int(0x090a),
      loc_s_int(0x0b0c)
    ));

    cassert(pack_s_code_var(&buffer,">QHH",values) == 0);
    cassert(buffer.used == 12);
    cassert((unsigned char)buffer.data[0] == 0x01);
    cassert((unsigned char)buffer.data[7] == 0x08);
    cassert((unsigned char)buffer.data[8] == 0x09);
    cassert((unsigned char)buffer.data[9] == 0x0a);
    cassert((unsigned char)buffer.data[10] == 0x0b);
    cassert((unsigned char)buffer.data[11] == 0x0c);
  }

  // - test var_s decode -
  {
    unsigned char data[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c};
    CONT_INIT_CLEAR(var_s,result);
    unsigned bytes_read;
    cassert(pack_s_decode_var(">QHH",(const char *)data,sizeof(data),&bytes_read,&result) == 0);
    cassert(bytes_read == 12);
    cassert(loc_s_array_length(result) == 3);
    cassert(loc_s_int_value(*loc_s_array_at(result,0)) == 0x0102030405060708LL);
    cassert(loc_s_int_value(*loc_s_array_at(result,1)) == 0x090a);
    cassert(loc_s_int_value(*loc_s_array_at(result,2)) == 0x0b0c);
  }

  // - test var_s roundtrip with string -
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);
    VAR_CLEAR(values,loc_s_array___locs(2,
      loc_s_int(42),
      loc_s_string_ptr("segment_a")
    ));

    cassert(pack_s_code_var(&buffer,">Hz",values) == 0);

    CONT_INIT_CLEAR(var_s,result);
    unsigned bytes_read;
    cassert(pack_s_decode_var(">Hz",buffer.data,buffer.used,&bytes_read,&result) == 0);
    cassert(loc_s_array_length(result) == 2);
    cassert(loc_s_int_value(*loc_s_array_at(result,0)) == 42);

    const string_s *dec_str = loc_s_string_value(*loc_s_array_at(result,1));
    cassert(dec_str->size - 1 == 9);
    cassert(memcmp(dec_str->data,"segment_a",9) == 0);
  }
}/*}}}*/

void test_pack_calc_size()
{/*{{{*/

  // - test fixed size format -
  {
    unsigned size;
    cassert(pack_s_calc_size(">QHH",&size) == 0);
    cassert(size == 12);
  }

  // - test mixed types -
  {
    unsigned size;
    cassert(pack_s_calc_size(">BHId",&size) == 0);
    cassert(size == 1 + 2 + 4 + 8);
  }

  // - test count prefix -
  {
    unsigned size;
    cassert(pack_s_calc_size(">3I",&size) == 0);
    cassert(size == 12);
  }

  // - test fixed string with count -
  {
    unsigned size;
    cassert(pack_s_calc_size(">H8z",&size) == 0);
    cassert(size == 10);
  }

  // - test empty format -
  {
    unsigned size;
    cassert(pack_s_calc_size("",&size) == 0);
    cassert(size == 0);
  }
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

