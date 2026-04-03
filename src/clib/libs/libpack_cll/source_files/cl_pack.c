
@begin
include "cl_pack.h"
@end

// === constants and definitions ===============================================

static const unsigned short c_pack_two_bytes = 0xff00;
#define c_pack_big_endian    (((const unsigned char *)&c_pack_two_bytes)[0] == 0xff)
#define c_pack_little_endian (((const unsigned char *)&c_pack_two_bytes)[1] == 0xff)

// === internal functions ======================================================

unsigned pack_s_terminal_recognize(const char **a_ptr)
{/*{{{*/
#define PACK_GET_NEXT_CHAR() in_char = (unsigned char)**a_ptr;
#define PACK_CLOSE_CHAR(RET_TERM_IDX) ++(*a_ptr);

  unsigned short in_char;

// - STATE 0 -
  PACK_GET_NEXT_CHAR();

  if (in_char == 0) {
    goto state_1_label;
  }
  if (in_char >= 48 && in_char < 58) {
    goto state_2_label;
  }
  if (in_char == 60) {
    goto state_3_label;
  }
  if (in_char == 61) {
    goto state_4_label;
  }
  if (in_char == 62) {
    goto state_5_label;
  }
  if (in_char == 66) {
    goto state_6_label;
  }
  if (in_char == 72) {
    goto state_7_label;
  }
  if (in_char == 73) {
    goto state_8_label;
  }
  if (in_char == 76) {
    goto state_9_label;
  }
  if (in_char == 81) {
    goto state_10_label;
  }
  if (in_char == 98) {
    goto state_11_label;
  }
  if (in_char == 99) {
    goto state_12_label;
  }
  if (in_char == 100) {
    goto state_13_label;
  }
  if (in_char == 102) {
    goto state_14_label;
  }
  if (in_char == 104) {
    goto state_15_label;
  }
  if (in_char == 105) {
    goto state_16_label;
  }
  if (in_char == 108) {
    goto state_17_label;
  }
  if (in_char == 113) {
    goto state_18_label;
  }
  if (in_char == 115) {
    goto state_19_label;
  }
  if (in_char == 122) {
    goto state_20_label;
  }
  return c_idx_not_exist;

// - STATE 1 -
state_1_label:
  PACK_CLOSE_CHAR(19);
  return 19;

// - STATE 2 -
state_2_label:
  PACK_CLOSE_CHAR(0);
  PACK_GET_NEXT_CHAR();

  if (in_char >= 48 && in_char < 58) {
    goto state_2_label;
  }
  return 0;

// - STATE 3 -
state_3_label:
  PACK_CLOSE_CHAR(2);
  return 2;

// - STATE 4 -
state_4_label:
  PACK_CLOSE_CHAR(1);
  return 1;

// - STATE 5 -
state_5_label:
  PACK_CLOSE_CHAR(3);
  return 3;

// - STATE 6 -
state_6_label:
  PACK_CLOSE_CHAR(6);
  return 6;

// - STATE 7 -
state_7_label:
  PACK_CLOSE_CHAR(8);
  return 8;

// - STATE 8 -
state_8_label:
  PACK_CLOSE_CHAR(10);
  return 10;

// - STATE 9 -
state_9_label:
  PACK_CLOSE_CHAR(12);
  return 12;

// - STATE 10 -
state_10_label:
  PACK_CLOSE_CHAR(14);
  return 14;

// - STATE 11 -
state_11_label:
  PACK_CLOSE_CHAR(5);
  return 5;

// - STATE 12 -
state_12_label:
  PACK_CLOSE_CHAR(4);
  return 4;

// - STATE 13 -
state_13_label:
  PACK_CLOSE_CHAR(16);
  return 16;

// - STATE 14 -
state_14_label:
  PACK_CLOSE_CHAR(15);
  return 15;

// - STATE 15 -
state_15_label:
  PACK_CLOSE_CHAR(7);
  return 7;

// - STATE 16 -
state_16_label:
  PACK_CLOSE_CHAR(9);
  return 9;

// - STATE 17 -
state_17_label:
  PACK_CLOSE_CHAR(11);
  return 11;

// - STATE 18 -
state_18_label:
  PACK_CLOSE_CHAR(13);
  return 13;

// - STATE 19 -
state_19_label:
  PACK_CLOSE_CHAR(17);
  return 17;

// - STATE 20 -
state_20_label:
  PACK_CLOSE_CHAR(18);
  return 18;

}/*}}}*/

// === va_args encode ==========================================================

int pack_s_code_ap(bc_array_s *a_trg,const char *a_format,va_list a_ap)
{/*{{{*/
  const char *f_ptr = a_format;
  const char *old_f_ptr;

  int change_byte_order = 0;
  unsigned count = 0;

  const unsigned c_temp_size = 512;
  char temp_buffer[c_temp_size];

  char *data_buffer = temp_buffer;
  unsigned data_size = 0;

#define PC_CLEAR_VA() \
  if (data_buffer != temp_buffer) { cfree(data_buffer); }

#define PC_ELEMENT_VA(TYPE,PROMOTED_TYPE) \
  {/*{{{*/\
    if (count == 0) { count = 1; }\
    data_size = count * sizeof(TYPE);\
\
    if (data_size > c_temp_size)\
    {\
      data_buffer = (char *)cmalloc(data_size);\
    }\
\
    if (data_size > 0)\
    {\
      TYPE *d_ptr;\
      TYPE *d_ptr_end;\
      int d_step;\
\
      if (change_byte_order)\
      {\
        d_ptr = (TYPE *)data_buffer + count - 1;\
        d_ptr_end = (TYPE *)data_buffer;\
        d_step = -1;\
      }\
      else\
      {\
        d_ptr = (TYPE *)data_buffer;\
        d_ptr_end = (TYPE *)data_buffer + count - 1;\
        d_step = 1;\
      }\
\
      do {\
        *d_ptr = (TYPE)va_arg(a_ap,PROMOTED_TYPE);\
\
        if (d_ptr == d_ptr_end)\
        {\
          break;\
        }\
\
        d_ptr += d_step;\
\
      } while(1);\
    }\
  }/*}}}*/

#define PC_WRITE_DATA_VA() \
  {/*{{{*/\
    if (data_size > 0)\
    {\
      unsigned old_used = a_trg->used;\
      bc_array_s_push_blanks(a_trg,data_size);\
\
      if (change_byte_order)\
      {\
        char *s_ptr = data_buffer + data_size - 1;\
        char *s_ptr_end = data_buffer;\
        char *ptr = a_trg->data + old_used;\
        do\
        {\
          *ptr = *s_ptr;\
        }\
        while(++ptr,--s_ptr >= s_ptr_end);\
      }\
      else\
      {\
        memcpy(a_trg->data + old_used,data_buffer,data_size);\
      }\
    }\
\
    if (data_buffer != temp_buffer)\
    {\
      cfree(data_buffer);\
      data_buffer = temp_buffer;\
    }\
\
    count = 0;\
  }/*}}}*/

  unsigned ret_term;
  do
  {
    old_f_ptr = f_ptr;
    ret_term = pack_s_terminal_recognize(&f_ptr);

    // - ERROR -
    if (ret_term == c_idx_not_exist)
    {
      PC_CLEAR_VA();
      throw_error(PACK_INVALID_FORMAT_STRING);
    }

    // - test end of format string -
    if (ret_term != c_pack_term_END)
    {
      switch (ret_term)
      {
      case c_pack_term_NUMBER:
      {/*{{{*/
        count = strtoul(old_f_ptr,NULL,10);
      }/*}}}*/
      break;
      case c_pack_term_NATIVE:
      {/*{{{*/
        change_byte_order = 0;
      }/*}}}*/
      break;
      case c_pack_term_LITTLE_ENDIAN:
      {/*{{{*/
        change_byte_order = !c_pack_little_endian;
      }/*}}}*/
      break;
      case c_pack_term_BIG_ENDIAN:
      {/*{{{*/
        change_byte_order = !c_pack_big_endian;
      }/*}}}*/
      break;
      case c_pack_term_CHAR:
      {/*{{{*/
        PC_ELEMENT_VA(char,int);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_SIGNED_BYTE:
      {/*{{{*/
        PC_ELEMENT_VA(signed char,int);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_UNSIGNED_BYTE:
      {/*{{{*/
        PC_ELEMENT_VA(unsigned char,int);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_SHORT:
      {/*{{{*/
        PC_ELEMENT_VA(short,int);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_UNSIGNED_SHORT:
      {/*{{{*/
        PC_ELEMENT_VA(unsigned short,int);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_INT:
      {/*{{{*/
        PC_ELEMENT_VA(int,int);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_UNSIGNED:
      {/*{{{*/
        PC_ELEMENT_VA(unsigned,unsigned);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_LONG_INT:
      {/*{{{*/
        PC_ELEMENT_VA(long int,long int);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_LONG_UNSIGNED:
      {/*{{{*/
        PC_ELEMENT_VA(long unsigned,long unsigned);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_LONG_LONG_INT:
      {/*{{{*/
        PC_ELEMENT_VA(long long int,long long int);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_LONG_LONG_UNSIGNED:
      {/*{{{*/
        PC_ELEMENT_VA(long long unsigned,long long unsigned);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_FLOAT:
      {/*{{{*/
        PC_ELEMENT_VA(float,double);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_DOUBLE:
      {/*{{{*/
        PC_ELEMENT_VA(double,double);
      }/*}}}*/
      goto pc_write_data_va;
      case c_pack_term_STRING:
      {/*{{{*/
        const string_s *string_ptr = va_arg(a_ap,const string_s *);
        unsigned string_length = string_ptr->size - 1;

        if (count == 0)
        {
          unsigned old_used = a_trg->used;
          bc_array_s_push_blanks(a_trg,sizeof(unsigned));

          if (change_byte_order)
          {
            const char *ptr = (const char *)&string_length;
            const char *ptr_end = ptr + sizeof(unsigned);
            char *t_ptr = a_trg->data + old_used + sizeof(unsigned);
            do
            {
              *(--t_ptr) = *ptr;
            }
            while(++ptr < ptr_end);
          }
          else
          {
            memcpy(a_trg->data + old_used,&string_length,sizeof(unsigned));
          }

          if (string_length > 0)
          {
            old_used = a_trg->used;
            bc_array_s_push_blanks(a_trg,string_length);
            memcpy(a_trg->data + old_used,string_ptr->data,string_length);
          }
        }
        else
        {
          if (string_length > count)
          {
            PC_CLEAR_VA();
            throw_error(PACK_STRING_EXCEEDS_TARGET);
          }

          unsigned old_used = a_trg->used;
          bc_array_s_push_blanks(a_trg,count);

          if (string_length > 0)
          {
            memcpy(a_trg->data + old_used,string_ptr->data,string_length);
          }

          if (string_length < count)
          {
            memset(a_trg->data + old_used + string_length,0,count - string_length);
          }
        }

        count = 0;
      }/*}}}*/
      break;
      case c_pack_term_ZERO_TERM_STRING:
      {/*{{{*/
        const string_s *string_ptr = va_arg(a_ap,const string_s *);
        unsigned string_length = string_ptr->size - 1;

        if (count == 0)
        {
          unsigned old_used = a_trg->used;
          bc_array_s_push_blanks(a_trg,string_length + 1);
          memcpy(a_trg->data + old_used,string_ptr->data,string_length + 1);
        }
        else
        {
          if (string_length > count)
          {
            PC_CLEAR_VA();
            throw_error(PACK_STRING_EXCEEDS_TARGET);
          }

          unsigned old_used = a_trg->used;
          bc_array_s_push_blanks(a_trg,count);

          if (string_length > 0)
          {
            memcpy(a_trg->data + old_used,string_ptr->data,string_length);
          }

          if (string_length < count)
          {
            memset(a_trg->data + old_used + string_length,0,count - string_length);
          }
        }

        count = 0;
      }/*}}}*/
      break;
pc_write_data_va:
      {/*{{{*/
        PC_WRITE_DATA_VA();
      }/*}}}*/
      break;
      default:
        cassert(0);
      }
    }
  }
  while(ret_term != c_pack_term_END);

  return 0;
}/*}}}*/

int pack_s_code(bc_array_s *a_trg,const char *a_format,...)
{/*{{{*/
  va_list ap;
  va_start(ap,a_format);
  int result = pack_s_code_ap(a_trg,a_format,ap);
  va_end(ap);

  return result;
}/*}}}*/

// === va_args decode ==========================================================

int pack_s_decode_ap(const char *a_format,const char *a_data,unsigned a_size,unsigned *a_read,va_list a_ap)
{/*{{{*/
  const char *ds_ptr = a_data;
  const char *ds_ptr_end = ds_ptr + a_size;

  const char *f_ptr = a_format;
  const char *old_f_ptr;

  int change_byte_order = 0;
  unsigned count = 0;

  const unsigned c_temp_size = 512;
  char temp_buffer[c_temp_size];

  char *data_buffer = temp_buffer;
  unsigned data_size = 0;

#define PD_CLEAR_VA() \
  if (data_buffer != temp_buffer) { cfree(data_buffer); }

#define PD_ELEMENT_VA(TYPE) \
  {/*{{{*/\
    if (count == 0) { count = 1; }\
    data_size = count * sizeof(TYPE);\
\
    if (data_size > 0)\
    {\
      if (data_size > c_temp_size)\
      {\
        data_buffer = (char *)cmalloc(data_size);\
      }\
\
      if ((unsigned)(ds_ptr_end - ds_ptr) < data_size)\
      {\
        PD_CLEAR_VA();\
        throw_error(PACK_NOT_ENOUGH_DATA);\
      }\
\
      TYPE *d_ptr;\
      TYPE *d_ptr_end;\
      int d_step;\
\
      if (change_byte_order)\
      {\
        char *ptr = data_buffer + data_size - 1;\
        const char *ptr_end = data_buffer;\
        do\
        {\
          *ptr = *ds_ptr;\
        }\
        while(++ds_ptr,--ptr >= ptr_end);\
\
        d_ptr = (TYPE *)data_buffer + count - 1;\
        d_ptr_end = (TYPE *)data_buffer;\
        d_step = -1;\
      }\
      else\
      {\
        memcpy(data_buffer,ds_ptr,data_size);\
        ds_ptr += data_size;\
\
        d_ptr = (TYPE *)data_buffer;\
        d_ptr_end = (TYPE *)data_buffer + count - 1;\
        d_step = 1;\
      }\
\
      do {\
        TYPE *out_ptr = va_arg(a_ap,TYPE *);\
        *out_ptr = *d_ptr;\
\
        if (d_ptr == d_ptr_end)\
        {\
          break;\
        }\
\
        d_ptr += d_step;\
\
      } while(1);\
\
      if (data_buffer != temp_buffer)\
      {\
        cfree(data_buffer);\
        data_buffer = temp_buffer;\
      }\
    }\
\
    count = 0;\
  }/*}}}*/

  unsigned ret_term;
  do
  {
    old_f_ptr = f_ptr;
    ret_term = pack_s_terminal_recognize(&f_ptr);

    // - ERROR -
    if (ret_term == c_idx_not_exist)
    {
      PD_CLEAR_VA();
      throw_error(PACK_INVALID_FORMAT_STRING);
    }

    // - test end of format string -
    if (ret_term != c_pack_term_END)
    {
      switch (ret_term)
      {
      case c_pack_term_NUMBER:
      {/*{{{*/
        count = strtoul(old_f_ptr,NULL,10);
      }/*}}}*/
      break;
      case c_pack_term_NATIVE:
      {/*{{{*/
        change_byte_order = 0;
      }/*}}}*/
      break;
      case c_pack_term_LITTLE_ENDIAN:
      {/*{{{*/
        change_byte_order = !c_pack_little_endian;
      }/*}}}*/
      break;
      case c_pack_term_BIG_ENDIAN:
      {/*{{{*/
        change_byte_order = !c_pack_big_endian;
      }/*}}}*/
      break;
      case c_pack_term_CHAR:
      {/*{{{*/
        PD_ELEMENT_VA(char);
      }/*}}}*/
      break;
      case c_pack_term_SIGNED_BYTE:
      {/*{{{*/
        PD_ELEMENT_VA(signed char);
      }/*}}}*/
      break;
      case c_pack_term_UNSIGNED_BYTE:
      {/*{{{*/
        PD_ELEMENT_VA(unsigned char);
      }/*}}}*/
      break;
      case c_pack_term_SHORT:
      {/*{{{*/
        PD_ELEMENT_VA(short);
      }/*}}}*/
      break;
      case c_pack_term_UNSIGNED_SHORT:
      {/*{{{*/
        PD_ELEMENT_VA(unsigned short);
      }/*}}}*/
      break;
      case c_pack_term_INT:
      {/*{{{*/
        PD_ELEMENT_VA(int);
      }/*}}}*/
      break;
      case c_pack_term_UNSIGNED:
      {/*{{{*/
        PD_ELEMENT_VA(unsigned);
      }/*}}}*/
      break;
      case c_pack_term_LONG_INT:
      {/*{{{*/
        PD_ELEMENT_VA(long int);
      }/*}}}*/
      break;
      case c_pack_term_LONG_UNSIGNED:
      {/*{{{*/
        PD_ELEMENT_VA(long unsigned);
      }/*}}}*/
      break;
      case c_pack_term_LONG_LONG_INT:
      {/*{{{*/
        PD_ELEMENT_VA(long long int);
      }/*}}}*/
      break;
      case c_pack_term_LONG_LONG_UNSIGNED:
      {/*{{{*/
        PD_ELEMENT_VA(long long unsigned);
      }/*}}}*/
      break;
      case c_pack_term_FLOAT:
      {/*{{{*/
        PD_ELEMENT_VA(float);
      }/*}}}*/
      break;
      case c_pack_term_DOUBLE:
      {/*{{{*/
        PD_ELEMENT_VA(double);
      }/*}}}*/
      break;
      case c_pack_term_STRING:
      {/*{{{*/
        unsigned string_length;

        if (count == 0)
        {
          if ((unsigned)(ds_ptr_end - ds_ptr) < sizeof(unsigned))
          {
            throw_error(PACK_NOT_ENOUGH_DATA);
          }

          if (change_byte_order)
          {
            const char *sl_ptr_end = ds_ptr + sizeof(unsigned);
            char *ptr = (char *)&string_length + sizeof(unsigned);
            do
            {
              *(--ptr) = *ds_ptr;
            }
            while(++ds_ptr < sl_ptr_end);
          }
          else
          {
            memcpy(&string_length,ds_ptr,sizeof(unsigned));
            ds_ptr += sizeof(unsigned);
          }
        }
        else
        {
          string_length = count;
        }

        if ((unsigned)(ds_ptr_end - ds_ptr) < string_length)
        {
          throw_error(PACK_NOT_ENOUGH_DATA);
        }

        string_s *out_str = va_arg(a_ap,string_s *);
        string_s_set(out_str,string_length,ds_ptr);
        ds_ptr += string_length;

        count = 0;
      }/*}}}*/
      break;
      case c_pack_term_ZERO_TERM_STRING:
      {/*{{{*/
        unsigned string_length;

        if (count == 0)
        {
          const char *ptr = ds_ptr;
          while (ptr < ds_ptr_end && *ptr != '\0') { ++ptr; }

          if (ptr >= ds_ptr_end)
          {
            throw_error(PACK_STRING_NOT_TERMINATED);
          }

          string_length = ptr - ds_ptr;
          count = string_length + 1;
        }
        else
        {
          const char *ptr = ds_ptr;
          const char *ptr_end = ptr + count;
          if (ptr_end > ds_ptr_end) { ptr_end = ds_ptr_end; }
          while (ptr < ptr_end && *ptr != '\0') { ++ptr; }

          string_length = ptr - ds_ptr;
        }

        if ((unsigned)(ds_ptr_end - ds_ptr) < count)
        {
          throw_error(PACK_NOT_ENOUGH_DATA);
        }

        string_s *out_str = va_arg(a_ap,string_s *);
        string_s_set(out_str,string_length,ds_ptr);
        ds_ptr += count;

        count = 0;
      }/*}}}*/
      break;
      default:
        cassert(0);
      }
    }
  }
  while(ret_term != c_pack_term_END);

  *a_read = ds_ptr - a_data;

  return 0;
}/*}}}*/

int pack_s_decode(const char *a_format,const char *a_data,unsigned a_size,unsigned *a_read,...)
{/*{{{*/
  va_list ap;
  va_start(ap,a_read);
  int result = pack_s_decode_ap(a_format,a_data,a_size,a_read,ap);
  va_end(ap);

  return result;
}/*}}}*/

// === var_s encode ============================================================

int pack_s_code_var(bc_array_s *a_trg,const char *a_format,var_s a_values)
{/*{{{*/
  var_array_s *array = loc_s_array_value(a_values);
  unsigned element_idx = 0;

  const char *f_ptr = a_format;
  const char *old_f_ptr;

  int change_byte_order = 0;
  unsigned count = 0;

  const unsigned c_temp_size = 512;
  char temp_buffer[c_temp_size];

  char *data_buffer = temp_buffer;
  unsigned data_size = 0;

#define PC_CLEAR_VAR() \
  if (data_buffer != temp_buffer) { cfree(data_buffer); }

#define PC_ELEMENT_VAR(TYPE) \
  {/*{{{*/\
    if (count == 0) { count = 1; }\
    data_size = count * sizeof(TYPE);\
\
    if (data_size > c_temp_size)\
    {\
      data_buffer = (char *)cmalloc(data_size);\
    }\
\
    if (data_size > 0)\
    {\
      TYPE *d_ptr;\
      TYPE *d_ptr_end;\
      int d_step;\
\
      if (change_byte_order)\
      {\
        d_ptr = (TYPE *)data_buffer + count - 1;\
        d_ptr_end = (TYPE *)data_buffer;\
        d_step = -1;\
      }\
      else\
      {\
        d_ptr = (TYPE *)data_buffer;\
        d_ptr_end = (TYPE *)data_buffer + count - 1;\
        d_step = 1;\
      }\
\
      do {\
        if (element_idx >= array->used)\
        {\
          PC_CLEAR_VAR();\
          throw_error(PACK_NOT_ENOUGH_ARGUMENTS);\
        }\
\
        var_s elem = *var_array_s_at(array,element_idx++);\
\
        if (elem->v_type != c_bi_type_integer && elem->v_type != c_bi_type_float)\
        {\
          PC_CLEAR_VAR();\
          throw_error(PACK_WRONG_ARGUMENT_TYPE);\
        }\
\
        if (elem->v_type == c_bi_type_integer)\
        {\
          *d_ptr = (TYPE)loc_s_int_value(elem);\
        }\
        else\
        {\
          *d_ptr = (TYPE)loc_s_float_value(elem);\
        }\
\
        if (d_ptr == d_ptr_end)\
        {\
          break;\
        }\
\
        d_ptr += d_step;\
\
      } while(1);\
    }\
  }/*}}}*/

#define PC_WRITE_DATA_VAR() \
  {/*{{{*/\
    if (data_size > 0)\
    {\
      unsigned old_used = a_trg->used;\
      bc_array_s_push_blanks(a_trg,data_size);\
\
      if (change_byte_order)\
      {\
        char *s_ptr = data_buffer + data_size - 1;\
        char *s_ptr_end = data_buffer;\
        char *ptr = a_trg->data + old_used;\
        do\
        {\
          *ptr = *s_ptr;\
        }\
        while(++ptr,--s_ptr >= s_ptr_end);\
      }\
      else\
      {\
        memcpy(a_trg->data + old_used,data_buffer,data_size);\
      }\
    }\
\
    if (data_buffer != temp_buffer)\
    {\
      cfree(data_buffer);\
      data_buffer = temp_buffer;\
    }\
\
    count = 0;\
  }/*}}}*/

  unsigned ret_term;
  do
  {
    old_f_ptr = f_ptr;
    ret_term = pack_s_terminal_recognize(&f_ptr);

    // - ERROR -
    if (ret_term == c_idx_not_exist)
    {
      PC_CLEAR_VAR();
      throw_error(PACK_INVALID_FORMAT_STRING);
    }

    // - test end of format string -
    if (ret_term != c_pack_term_END)
    {
      switch (ret_term)
      {
      case c_pack_term_NUMBER:
      {/*{{{*/
        count = strtoul(old_f_ptr,NULL,10);
      }/*}}}*/
      break;
      case c_pack_term_NATIVE:
      {/*{{{*/
        change_byte_order = 0;
      }/*}}}*/
      break;
      case c_pack_term_LITTLE_ENDIAN:
      {/*{{{*/
        change_byte_order = !c_pack_little_endian;
      }/*}}}*/
      break;
      case c_pack_term_BIG_ENDIAN:
      {/*{{{*/
        change_byte_order = !c_pack_big_endian;
      }/*}}}*/
      break;
      case c_pack_term_CHAR:
      {/*{{{*/
        PC_ELEMENT_VAR(char);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_SIGNED_BYTE:
      {/*{{{*/
        PC_ELEMENT_VAR(signed char);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_UNSIGNED_BYTE:
      {/*{{{*/
        PC_ELEMENT_VAR(unsigned char);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_SHORT:
      {/*{{{*/
        PC_ELEMENT_VAR(short);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_UNSIGNED_SHORT:
      {/*{{{*/
        PC_ELEMENT_VAR(unsigned short);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_INT:
      {/*{{{*/
        PC_ELEMENT_VAR(int);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_UNSIGNED:
      {/*{{{*/
        PC_ELEMENT_VAR(unsigned);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_LONG_INT:
      {/*{{{*/
        PC_ELEMENT_VAR(long int);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_LONG_UNSIGNED:
      {/*{{{*/
        PC_ELEMENT_VAR(long unsigned);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_LONG_LONG_INT:
      {/*{{{*/
        PC_ELEMENT_VAR(long long int);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_LONG_LONG_UNSIGNED:
      {/*{{{*/
        PC_ELEMENT_VAR(long long unsigned);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_FLOAT:
      {/*{{{*/
        PC_ELEMENT_VAR(float);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_DOUBLE:
      {/*{{{*/
        PC_ELEMENT_VAR(double);
      }/*}}}*/
      goto pc_write_data_var;
      case c_pack_term_STRING:
      {/*{{{*/
        if (element_idx >= array->used)
        {
          PC_CLEAR_VAR();
          throw_error(PACK_NOT_ENOUGH_ARGUMENTS);
        }

        var_s elem = *var_array_s_at(array,element_idx++);

        if (elem->v_type != c_bi_type_string)
        {
          PC_CLEAR_VAR();
          throw_error(PACK_WRONG_ARGUMENT_TYPE);
        }

        const string_s *string_ptr = loc_s_string_value(elem);
        unsigned string_length = string_ptr->size - 1;

        if (count == 0)
        {
          unsigned old_used = a_trg->used;
          bc_array_s_push_blanks(a_trg,sizeof(unsigned));

          if (change_byte_order)
          {
            const char *ptr = (const char *)&string_length;
            const char *ptr_end = ptr + sizeof(unsigned);
            char *t_ptr = a_trg->data + old_used + sizeof(unsigned);
            do
            {
              *(--t_ptr) = *ptr;
            }
            while(++ptr < ptr_end);
          }
          else
          {
            memcpy(a_trg->data + old_used,&string_length,sizeof(unsigned));
          }

          if (string_length > 0)
          {
            old_used = a_trg->used;
            bc_array_s_push_blanks(a_trg,string_length);
            memcpy(a_trg->data + old_used,string_ptr->data,string_length);
          }
        }
        else
        {
          if (string_length > count)
          {
            PC_CLEAR_VAR();
            throw_error(PACK_STRING_EXCEEDS_TARGET);
          }

          unsigned old_used = a_trg->used;
          bc_array_s_push_blanks(a_trg,count);

          if (string_length > 0)
          {
            memcpy(a_trg->data + old_used,string_ptr->data,string_length);
          }

          if (string_length < count)
          {
            memset(a_trg->data + old_used + string_length,0,count - string_length);
          }
        }

        count = 0;
      }/*}}}*/
      break;
      case c_pack_term_ZERO_TERM_STRING:
      {/*{{{*/
        if (element_idx >= array->used)
        {
          PC_CLEAR_VAR();
          throw_error(PACK_NOT_ENOUGH_ARGUMENTS);
        }

        var_s elem = *var_array_s_at(array,element_idx++);

        if (elem->v_type != c_bi_type_string)
        {
          PC_CLEAR_VAR();
          throw_error(PACK_WRONG_ARGUMENT_TYPE);
        }

        const string_s *string_ptr = loc_s_string_value(elem);
        unsigned string_length = string_ptr->size - 1;

        if (count == 0)
        {
          unsigned old_used = a_trg->used;
          bc_array_s_push_blanks(a_trg,string_length + 1);
          memcpy(a_trg->data + old_used,string_ptr->data,string_length + 1);
        }
        else
        {
          if (string_length > count)
          {
            PC_CLEAR_VAR();
            throw_error(PACK_STRING_EXCEEDS_TARGET);
          }

          unsigned old_used = a_trg->used;
          bc_array_s_push_blanks(a_trg,count);

          if (string_length > 0)
          {
            memcpy(a_trg->data + old_used,string_ptr->data,string_length);
          }

          if (string_length < count)
          {
            memset(a_trg->data + old_used + string_length,0,count - string_length);
          }
        }

        count = 0;
      }/*}}}*/
      break;
pc_write_data_var:
      {/*{{{*/
        PC_WRITE_DATA_VAR();
      }/*}}}*/
      break;
      default:
        cassert(0);
      }
    }
  }
  while(ret_term != c_pack_term_END);

  if (element_idx < array->used)
  {
    PC_CLEAR_VAR();
    throw_error(PACK_NOT_ALL_ARGUMENTS_CONVERTED);
  }

  return 0;
}/*}}}*/

// === var_s decode ============================================================

int pack_s_decode_var(const char *a_format,const char *a_data,unsigned a_size,unsigned *a_read,var_s *a_result)
{/*{{{*/
  VAR_CLEAR(result,loc_s_array());

  const char *ds_ptr = a_data;
  const char *ds_ptr_end = ds_ptr + a_size;

  const char *f_ptr = a_format;
  const char *old_f_ptr;

  int change_byte_order = 0;
  unsigned count = 0;

  const unsigned c_temp_size = 512;
  char temp_buffer[c_temp_size];

  char *data_buffer = temp_buffer;
  unsigned data_size = 0;

#define PD_CLEAR_VAR() \
  if (data_buffer != temp_buffer) { cfree(data_buffer); }

#define PD_ELEMENT_VAR(TYPE,CREATE_CODE) \
  {/*{{{*/\
    if (count == 0) { count = 1; }\
    data_size = count * sizeof(TYPE);\
\
    if (data_size > 0)\
    {\
      if (data_size > c_temp_size)\
      {\
        data_buffer = (char *)cmalloc(data_size);\
      }\
\
      if ((unsigned)(ds_ptr_end - ds_ptr) < data_size)\
      {\
        PD_CLEAR_VAR();\
        throw_error(PACK_NOT_ENOUGH_DATA);\
      }\
\
      TYPE *d_ptr;\
      TYPE *d_ptr_end;\
      int d_step;\
\
      if (change_byte_order)\
      {\
        char *ptr = data_buffer + data_size - 1;\
        const char *ptr_end = data_buffer;\
        do\
        {\
          *ptr = *ds_ptr;\
        }\
        while(++ds_ptr,--ptr >= ptr_end);\
\
        d_ptr = (TYPE *)data_buffer + count - 1;\
        d_ptr_end = (TYPE *)data_buffer;\
        d_step = -1;\
      }\
      else\
      {\
        memcpy(data_buffer,ds_ptr,data_size);\
        ds_ptr += data_size;\
\
        d_ptr = (TYPE *)data_buffer;\
        d_ptr_end = (TYPE *)data_buffer + count - 1;\
        d_step = 1;\
      }\
\
      do {\
        loc_s_array_push(result,CREATE_CODE);\
\
        if (d_ptr == d_ptr_end)\
        {\
          break;\
        }\
\
        d_ptr += d_step;\
\
      } while(1);\
\
      if (data_buffer != temp_buffer)\
      {\
        cfree(data_buffer);\
        data_buffer = temp_buffer;\
      }\
    }\
\
    count = 0;\
  }/*}}}*/

  unsigned ret_term;
  do
  {
    old_f_ptr = f_ptr;
    ret_term = pack_s_terminal_recognize(&f_ptr);

    // - ERROR -
    if (ret_term == c_idx_not_exist)
    {
      PD_CLEAR_VAR();
      throw_error(PACK_INVALID_FORMAT_STRING);
    }

    // - test end of format string -
    if (ret_term != c_pack_term_END)
    {
      switch (ret_term)
      {
      case c_pack_term_NUMBER:
      {/*{{{*/
        count = strtoul(old_f_ptr,NULL,10);
      }/*}}}*/
      break;
      case c_pack_term_NATIVE:
      {/*{{{*/
        change_byte_order = 0;
      }/*}}}*/
      break;
      case c_pack_term_LITTLE_ENDIAN:
      {/*{{{*/
        change_byte_order = !c_pack_little_endian;
      }/*}}}*/
      break;
      case c_pack_term_BIG_ENDIAN:
      {/*{{{*/
        change_byte_order = !c_pack_big_endian;
      }/*}}}*/
      break;
      case c_pack_term_CHAR:
      {/*{{{*/
        PD_ELEMENT_VAR(char,loc_s_int((long long int)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_SIGNED_BYTE:
      {/*{{{*/
        PD_ELEMENT_VAR(signed char,loc_s_int((long long int)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_UNSIGNED_BYTE:
      {/*{{{*/
        PD_ELEMENT_VAR(unsigned char,loc_s_int((long long int)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_SHORT:
      {/*{{{*/
        PD_ELEMENT_VAR(short,loc_s_int((long long int)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_UNSIGNED_SHORT:
      {/*{{{*/
        PD_ELEMENT_VAR(unsigned short,loc_s_int((long long int)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_INT:
      {/*{{{*/
        PD_ELEMENT_VAR(int,loc_s_int((long long int)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_UNSIGNED:
      {/*{{{*/
        PD_ELEMENT_VAR(unsigned,loc_s_int((long long int)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_LONG_INT:
      {/*{{{*/
        PD_ELEMENT_VAR(long int,loc_s_int((long long int)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_LONG_UNSIGNED:
      {/*{{{*/
        PD_ELEMENT_VAR(long unsigned,loc_s_int((long long int)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_LONG_LONG_INT:
      {/*{{{*/
        PD_ELEMENT_VAR(long long int,loc_s_int((long long int)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_LONG_LONG_UNSIGNED:
      {/*{{{*/
        PD_ELEMENT_VAR(long long unsigned,loc_s_int((long long int)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_FLOAT:
      {/*{{{*/
        PD_ELEMENT_VAR(float,loc_s_float((double)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_DOUBLE:
      {/*{{{*/
        PD_ELEMENT_VAR(double,loc_s_float((double)*d_ptr));
      }/*}}}*/
      break;
      case c_pack_term_STRING:
      {/*{{{*/
        unsigned string_length;

        if (count == 0)
        {
          if ((unsigned)(ds_ptr_end - ds_ptr) < sizeof(unsigned))
          {
            PD_CLEAR_VAR();
            throw_error(PACK_NOT_ENOUGH_DATA);
          }

          if (change_byte_order)
          {
            const char *sl_ptr_end = ds_ptr + sizeof(unsigned);
            char *ptr = (char *)&string_length + sizeof(unsigned);
            do
            {
              *(--ptr) = *ds_ptr;
            }
            while(++ds_ptr < sl_ptr_end);
          }
          else
          {
            memcpy(&string_length,ds_ptr,sizeof(unsigned));
            ds_ptr += sizeof(unsigned);
          }
        }
        else
        {
          string_length = count;
        }

        if ((unsigned)(ds_ptr_end - ds_ptr) < string_length)
        {
          PD_CLEAR_VAR();
          throw_error(PACK_NOT_ENOUGH_DATA);
        }

        loc_s_array_push(result,loc_s_string(string_length,ds_ptr));
        ds_ptr += string_length;

        count = 0;
      }/*}}}*/
      break;
      case c_pack_term_ZERO_TERM_STRING:
      {/*{{{*/
        unsigned string_length;

        if (count == 0)
        {
          const char *ptr = ds_ptr;
          while (ptr < ds_ptr_end && *ptr != '\0') { ++ptr; }

          if (ptr >= ds_ptr_end)
          {
            PD_CLEAR_VAR();
            throw_error(PACK_STRING_NOT_TERMINATED);
          }

          string_length = ptr - ds_ptr;
          count = string_length + 1;
        }
        else
        {
          const char *ptr = ds_ptr;
          const char *ptr_end = ptr + count;
          if (ptr_end > ds_ptr_end) { ptr_end = ds_ptr_end; }
          while (ptr < ptr_end && *ptr != '\0') { ++ptr; }

          string_length = ptr - ds_ptr;
        }

        if ((unsigned)(ds_ptr_end - ds_ptr) < count)
        {
          PD_CLEAR_VAR();
          throw_error(PACK_NOT_ENOUGH_DATA);
        }

        loc_s_array_push(result,loc_s_string(string_length,ds_ptr));
        ds_ptr += count;

        count = 0;
      }/*}}}*/
      break;
      default:
        cassert(0);
      }
    }
  }
  while(ret_term != c_pack_term_END);

  *a_read = ds_ptr - a_data;

  // - transfer result to caller -
  var_s_copy(a_result,&result);

  return 0;
}/*}}}*/

// === utility =================================================================

int pack_s_calc_size(const char *a_format,unsigned *a_size)
{/*{{{*/
  const char *f_ptr = a_format;
  const char *old_f_ptr;

  unsigned count = 0;
  unsigned total_size = 0;

  unsigned ret_term;
  do
  {
    old_f_ptr = f_ptr;
    ret_term = pack_s_terminal_recognize(&f_ptr);

    if (ret_term == c_idx_not_exist)
    {
      throw_error(PACK_INVALID_FORMAT_STRING);
    }

    if (ret_term != c_pack_term_END)
    {
      switch (ret_term)
      {
      case c_pack_term_NUMBER:
        count = strtoul(old_f_ptr,NULL,10);
        break;
      case c_pack_term_NATIVE:
      case c_pack_term_LITTLE_ENDIAN:
      case c_pack_term_BIG_ENDIAN:
        break;
      case c_pack_term_CHAR:
      case c_pack_term_SIGNED_BYTE:
      case c_pack_term_UNSIGNED_BYTE:
        if (count == 0) { count = 1; }
        total_size += count * sizeof(char);
        count = 0;
        break;
      case c_pack_term_SHORT:
      case c_pack_term_UNSIGNED_SHORT:
        if (count == 0) { count = 1; }
        total_size += count * sizeof(short);
        count = 0;
        break;
      case c_pack_term_INT:
      case c_pack_term_UNSIGNED:
        if (count == 0) { count = 1; }
        total_size += count * sizeof(int);
        count = 0;
        break;
      case c_pack_term_LONG_INT:
      case c_pack_term_LONG_UNSIGNED:
        if (count == 0) { count = 1; }
        total_size += count * sizeof(long);
        count = 0;
        break;
      case c_pack_term_LONG_LONG_INT:
      case c_pack_term_LONG_LONG_UNSIGNED:
        if (count == 0) { count = 1; }
        total_size += count * sizeof(long long);
        count = 0;
        break;
      case c_pack_term_FLOAT:
        if (count == 0) { count = 1; }
        total_size += count * sizeof(float);
        count = 0;
        break;
      case c_pack_term_DOUBLE:
        if (count == 0) { count = 1; }
        total_size += count * sizeof(double);
        count = 0;
        break;
      case c_pack_term_STRING:
        if (count == 0)
        {
          // - variable length: cannot compute -
          throw_error(PACK_INVALID_FORMAT_STRING);
        }
        total_size += count;
        count = 0;
        break;
      case c_pack_term_ZERO_TERM_STRING:
        if (count == 0)
        {
          // - variable length: cannot compute -
          throw_error(PACK_INVALID_FORMAT_STRING);
        }
        total_size += count;
        count = 0;
        break;
      default:
        cassert(0);
      }
    }
  }
  while(ret_term != c_pack_term_END);

  *a_size = total_size;

  return 0;
}/*}}}*/

