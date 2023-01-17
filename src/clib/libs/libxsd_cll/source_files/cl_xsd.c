
@begin
include "cl_xsd.h"
@end

// === constants and definitions ===============================================

const char XML_HEADER[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

// === methods of generated structures =========================================

// -- lli_array_s --
@begin
methods lli_array_s
@end

// -- bd_array_s --
@begin
methods bd_array_s
@end

// -- xs_boolean_value_s --
int xs_boolean_value_s_write(const xs_boolean_value_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,*this ? "true" : "false");

  return 0;
}/*}}}*/

int xs_boolean_value_s_read(xs_boolean_value_s *this,var_s a_var)
{/*{{{*/
  *this = strcmp(loc_s_string_value(a_var)->data,"true") == 0;
  return 0;
}/*}}}*/

// -- xs_bool_s --
int xs_bool_s_write(const xs_bool_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,*this ? "true" : "false");

  return 0;
}/*}}}*/

int xs_bool_s_read(xs_bool_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  *this = strcmp(loc_s_string_value(value_var)->data,"true") == 0;
  return 0;
}/*}}}*/

// -- xs_boolean_s --
int xs_boolean_s_write(const xs_boolean_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,*this ? "true" : "false");

  return 0;
}/*}}}*/

int xs_boolean_s_read(xs_boolean_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  *this = strcmp(loc_s_string_value(value_var)->data,"true") == 0;
  return 0;
}/*}}}*/

// -- xs_byte_s --
int xs_byte_s_write(const xs_byte_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%d",*this);

  return 0;
}/*}}}*/

int xs_byte_s_read(xs_byte_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);

  char *end_ptr;
  *this = strtol(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_int_value_s --
int xs_int_value_s_write(const xs_int_value_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%lld",*this);

  return 0;
}/*}}}*/

int xs_int_value_s_read(xs_int_value_s *this,var_s a_var)
{/*{{{*/
  const string_s *value_str = loc_s_string_value(a_var);

  char *end_ptr;
  *this = strtoll(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_int_s --
int xs_int_s_write(const xs_int_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%lld",*this);

  return 0;
}/*}}}*/

int xs_int_s_read(xs_int_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);

  char *end_ptr;
  *this = strtoll(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_integer_value_s --
int xs_integer_value_s_write(const xs_integer_value_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%lld",*this);

  return 0;
}/*}}}*/

int xs_integer_value_s_read(xs_integer_value_s *this,var_s a_var)
{/*{{{*/
  const string_s *value_str = loc_s_string_value(a_var);

  char *end_ptr;
  *this = strtoll(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_integer_s --
int xs_integer_s_write(const xs_integer_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%lld",*this);

  return 0;
}/*}}}*/

int xs_integer_s_read(xs_integer_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);

  char *end_ptr;
  *this = strtoll(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_unsignedInt_s --
int xs_unsignedInt_s_write(const xs_unsignedInt_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (*this < 0)
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  bc_array_s_append_format(a_trg,"%lld",*this);

  return 0;
}/*}}}*/

int xs_unsignedInt_s_read(xs_unsignedInt_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);

  char *end_ptr;
  *this = strtoll(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1) ||
      *this < 0)
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_unsignedLong_s --
int xs_unsignedLong_s_write(const xs_unsignedLong_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%llu",*this);

  return 0;
}/*}}}*/

int xs_unsignedLong_s_read(xs_unsignedLong_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);

  char *end_ptr;
  *this = strtoll(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_nonNegativeInteger_s --
int xs_nonNegativeInteger_s_write(const xs_nonNegativeInteger_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (*this < 0)
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  bc_array_s_append_format(a_trg,"%lld",*this);

  return 0;
}/*}}}*/

int xs_nonNegativeInteger_s_read(xs_nonNegativeInteger_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);

  char *end_ptr;
  *this = strtoll(value_str->data,&end_ptr,10);

  if (end_ptr != (value_str->data + value_str->size - 1) ||
      *this < 0)
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_float_value_s --
int xs_float_value_s_write(const xs_float_value_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%f",*this);

  return 0;
}/*}}}*/

int xs_float_value_s_read(xs_float_value_s *this,var_s a_var)
{/*{{{*/
  const string_s *value_str = loc_s_string_value(a_var);
  char *end_ptr;
  *this = strtof(value_str->data,&end_ptr);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_float_s --
int xs_float_s_write(const xs_float_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%f",*this);

  return 0;
}/*}}}*/

int xs_float_s_read(xs_float_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);
  char *end_ptr;
  *this = strtof(value_str->data,&end_ptr);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_double_value_s --
int xs_double_value_s_write(const xs_double_value_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%f",*this);

  return 0;
}/*}}}*/

int xs_double_value_s_read(xs_double_value_s *this,var_s a_var)
{/*{{{*/
  const string_s *value_str = loc_s_string_value(a_var);
  char *end_ptr;
  *this = strtod(value_str->data,&end_ptr);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_double_s --
int xs_double_s_write(const xs_double_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%f",*this);

  return 0;
}/*}}}*/

int xs_double_s_read(xs_double_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);
  char *end_ptr;
  *this = strtod(value_str->data,&end_ptr);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_decimal_s --
int xs_decimal_s_write(const xs_decimal_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"%f",*this);

  return 0;
}/*}}}*/

int xs_decimal_s_read(xs_decimal_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  const string_s *value_str = loc_s_string_value(value_var);
  char *end_ptr;
  *this = strtod(value_str->data,&end_ptr);

  if (end_ptr != (value_str->data + value_str->size - 1))
  {
    throw_error(XSD_INVALID_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// -- xs_token_s --
int xs_token_s_write(const xs_token_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_token_s_read(xs_token_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

// -- xs_string_value_s --
int xs_string_value_s_write(const xs_string_value_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_string_value_s_read(xs_string_value_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(this,loc_s_string_value(a_var));

  return 0;
}/*}}}*/

// -- xs_string_s --
int xs_string_s_write(const xs_string_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_string_s_read(xs_string_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

// -- xs_language_s --
int xs_language_s_write(const xs_language_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_language_s_read(xs_language_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

// -- xs_NMTOKEN_s --
int xs_NMTOKEN_s_write(const xs_NMTOKEN_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_NMTOKEN_s_read(xs_NMTOKEN_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

// -- xs_anyURI_value_s --
int xs_anyURI_value_s_write(const xs_anyURI_value_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_anyURI_value_s_read(xs_anyURI_value_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(this,loc_s_string_value(a_var));

  return 0;
}/*}}}*/

// -- xs_anyURI_s --
int xs_anyURI_s_write(const xs_anyURI_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_anyURI_s_read(xs_anyURI_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

// -- xs_anySimpleType_value_s --
int xs_anySimpleType_value_s_write(const xs_anySimpleType_value_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_anySimpleType_value_s_read(xs_anySimpleType_value_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(this,loc_s_string_value(a_var));

  return 0;
}/*}}}*/

// -- xs_anyType_s --
int xs_anyType_s_write(const xs_anyType_s *this,bc_array_s *a_trg)
{/*{{{*/

  // FIXME TODO continue ...
  cassert(0);

  return 0;
}/*}}}*/

int xs_anyType_s_read(xs_anyType_s *this,var_s a_var)
{/*{{{*/
  var_s_copy(this,&a_var);

  return 0;
}/*}}}*/

// -- xs_QName_value_s --
int xs_QName_value_s_write(const xs_QName_value_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_QName_value_s_read(xs_QName_value_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(this,loc_s_string_value(a_var));

  return 0;
}/*}}}*/

// -- xs_QName_s --
int xs_QName_s_write(const xs_QName_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_QName_s_read(xs_QName_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

// -- xs_base64Binary_value_s --
int xs_base64Binary_value_s_write(const xs_base64Binary_value_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_base64Binary_value_s_read(xs_base64Binary_value_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(this,loc_s_string_value(a_var));

  return 0;
}/*}}}*/

// -- xs_base64Binary_s --
int xs_base64Binary_s_write(const xs_base64Binary_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_base64Binary_s_read(xs_base64Binary_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

// -- xs_dateTime_value_s --
int xs_dateTime_value_s_write(const xs_dateTime_value_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_dateTime_value_s_read(xs_dateTime_value_s *this,var_s a_var)
{/*{{{*/
  string_s_copy(this,loc_s_string_value(a_var));

  return 0;
}/*}}}*/

// -- xs_date_s --
int xs_date_s_write(const xs_date_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_date_s_read(xs_date_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

void xs_date_s_create(xs_date_s *this,time_s a_time)
{/*{{{*/
  datetime_s dt;
  datetime_s_from_nanosec(&dt,a_time);
  string_s_set_format(this,
      "%4.4hu-%2.2hhu-%2.2hhu",dt.year,dt.month,dt.day);
}/*}}}*/

int xs_date_s_parse(xs_date_s *this,time_s *a_time)
{/*{{{*/
  char *ptr = this->data;
  char *end_ptr;

  do {
    datetime_s dt = {};

    dt.year = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 4 || *end_ptr != '-') { break; }

    ptr = end_ptr + 1;
    dt.month = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 2 || *end_ptr != '-') { break; }

    ptr = end_ptr + 1;
    dt.day = strtoll(ptr,&end_ptr,10);

    if (datetime_s_to_nanosec(&dt,a_time))
    {
      break;
    }

    char mod = *end_ptr;
    switch (mod)
    {
    case '\0':
    case 'Z':
      return 0;
    case '-':
    case '+':
      do {
        ptr = end_ptr + 1;
        dt.hour = strtoll(ptr,&end_ptr,10);
        if (end_ptr - ptr != 2 || *end_ptr != ':') { break; }

        ptr = end_ptr + 1;
        dt.min = strtoll(ptr,&end_ptr,10);
        if (end_ptr - ptr != 2 || *end_ptr != '\0') { break; }

        ulli mod_value = (dt.hour*60ULL + dt.min)*60000000000ULL;

        if (mod == '-')
        {
          *a_time -= mod_value;
        }
        else
        {
          *a_time += mod_value;
        }

        return 0;
      } while(0);
      break;
    }
  } while(0);

  throw_error(XSD_DATE_TIME_PARSE_ERROR);
}/*}}}*/

// -- xs_time_s --
int xs_time_s_write(const xs_time_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_time_s_read(xs_time_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

void xs_time_s_create(xs_time_s *this,time_s a_time)
{/*{{{*/
  datetime_s dt;
  datetime_s_from_nanosec(&dt,a_time);
  string_s_set_format(this,
      "%2.2hhu:%2.2hhu:%2.2hhu.%3.3hu",dt.hour,dt.min,dt.sec,dt.msec);
}/*}}}*/

int xs_time_s_parse(xs_time_s *this,time_s *a_time)
{/*{{{*/
  char *ptr = this->data;
  char *end_ptr;

  do {
    datetime_s dt = { .year = 1970, .month = 1, .day = 1, };

    dt.hour = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 2 || *end_ptr != ':') { break; }

    ptr = end_ptr + 1;
    dt.min = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 2 || *end_ptr != ':') { break; }

    ptr = end_ptr + 1;
    double sec = strtod(ptr,&end_ptr) + 0.0005;
    dt.sec = sec;
    dt.msec = (sec - dt.sec)*1000;

    if (datetime_s_to_nanosec(&dt,a_time))
    {
      break;
    }

    char mod = *end_ptr;
    switch (mod)
    {
    case '\0':
    case 'Z':
      return 0;
    case '-':
    case '+':
      do {
        ptr = end_ptr + 1;
        dt.hour = strtoll(ptr,&end_ptr,10);
        if (end_ptr - ptr != 2 || *end_ptr != ':') { break; }

        ptr = end_ptr + 1;
        dt.min = strtoll(ptr,&end_ptr,10);
        if (end_ptr - ptr != 2 || *end_ptr != '\0') { break; }

        ulli mod_value = (dt.hour*60ULL + dt.min)*60000000000ULL;

        if (mod == '-')
        {
          *a_time -= mod_value;
        }
        else
        {
          *a_time += mod_value;
        }

        return 0;
      } while(0);
      break;
    }
  } while(0);

  throw_error(XSD_DATE_TIME_PARSE_ERROR);
}/*}}}*/

// -- xs_dateTime_s --
int xs_dateTime_s_write(const xs_dateTime_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_dateTime_s_read(xs_dateTime_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

void xs_dateTime_s_create(xs_dateTime_s *this,time_s a_time)
{/*{{{*/
  datetime_s dt;
  datetime_s_from_nanosec(&dt,a_time);
  string_s_set_format(this,
      "%4.4hu-%2.2hhu-%2.2hhuT%2.2hhu:%2.2hhu:%2.2hhu.%3.3hu",
      dt.year,dt.month,dt.day,dt.hour,dt.min,dt.sec,dt.msec);
}/*}}}*/

int xs_dateTime_s_parse(xs_dateTime_s *this,time_s *a_time)
{/*{{{*/
  char *ptr = this->data;
  char *end_ptr;

  do {
    datetime_s dt = {};

    dt.year = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 4 || *end_ptr != '-') { break; }

    ptr = end_ptr + 1;
    dt.month = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 2 || *end_ptr != '-') { break; }

    ptr = end_ptr + 1;
    dt.day = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 2 || *end_ptr != 'T') { break; }

    ptr = end_ptr + 1;
    dt.hour = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 2 || *end_ptr != ':') { break; }

    ptr = end_ptr + 1;
    dt.min = strtoll(ptr,&end_ptr,10);
    if (end_ptr - ptr != 2 || *end_ptr != ':') { break; }

    ptr = end_ptr + 1;
    double sec = strtod(ptr,&end_ptr) + 0.0005;
    dt.sec = sec;
    dt.msec = (sec - dt.sec)*1000;

    if (datetime_s_to_nanosec(&dt,a_time))
    {
      break;
    }

    char mod = *end_ptr;
    switch (mod)
    {
    case '\0':
    case 'Z':
      return 0;
    case '-':
    case '+':
      do {
        ptr = end_ptr + 1;
        dt.hour = strtoll(ptr,&end_ptr,10);
        if (end_ptr - ptr != 2 || *end_ptr != ':') { break; }

        ptr = end_ptr + 1;
        dt.min = strtoll(ptr,&end_ptr,10);
        if (end_ptr - ptr != 2 || *end_ptr != '\0') { break; }

        ulli mod_value = (dt.hour*60ULL + dt.min)*60000000000ULL;

        if (mod == '-')
        {
          *a_time -= mod_value;
        }
        else
        {
          *a_time += mod_value;
        }

        return 0;
      } while(0);
      break;
    }
  } while(0);

  throw_error(XSD_DATE_TIME_PARSE_ERROR);
}/*}}}*/

// -- xs_duration_s --
int xs_duration_s_write(const xs_duration_s *this,bc_array_s *a_trg)
{/*{{{*/
  xml_create_append_string(this->size - 1,this->data,a_trg);

  return 0;
}/*}}}*/

int xs_duration_s_read(xs_duration_s *this,var_s a_var)
{/*{{{*/
  var_s value_var = loc_s_dict_get(a_var,g_str_text_var);
  string_s_copy(this,loc_s_string_value(value_var));

  return 0;
}/*}}}*/

void xs_duration_s_create(xs_duration_s *this,lli a_period)
{/*{{{*/
  CONT_INIT_CLEAR(bc_array_s,buffer);

  if (a_period < 0)
  {
    bc_array_s_push(&buffer,'-');
    a_period = -a_period;
  }

  bc_array_s_push(&buffer,'P');

  if (a_period >= CL_TIME_NANOSEC_IN_DAY)
  {
    lli days = a_period / CL_TIME_NANOSEC_IN_DAY;
    a_period %= CL_TIME_NANOSEC_IN_DAY;
    bc_array_s_append_format(&buffer,"%lldD",days);
  }

  int time_section = 0;

  if (a_period >= CL_TIME_NANOSEC_IN_HOUR)
  {
    lli hours = a_period / CL_TIME_NANOSEC_IN_HOUR;
    a_period %= CL_TIME_NANOSEC_IN_HOUR;
    bc_array_s_append_format(&buffer,"T%lldH",hours);
    time_section = 1;
  }

  if (a_period >= CL_TIME_NANOSEC_IN_MINUTE)
  {
    lli minutes = a_period / CL_TIME_NANOSEC_IN_MINUTE;
    a_period %= CL_TIME_NANOSEC_IN_MINUTE;
    bc_array_s_append_format(&buffer,"%s%lldM",time_section ? "" : "T",minutes);
    time_section = 1;
  }

  // - truncate to milliseconds -
  a_period -= a_period % 1000000ULL;

  if (a_period != 0 || buffer.data[buffer.used - 1] == 'P')
  {
    if (!time_section)
    {
      bc_array_s_push(&buffer,'T');
    }

    if (a_period % CL_TIME_NANOSEC_IN_SECOND != 0)
    {
      double seconds = (double)a_period / CL_TIME_NANOSEC_IN_SECOND;
      bc_array_s_append_format(&buffer,"%.3fS",seconds);
    }
    else
    {
      lli seconds = a_period / CL_TIME_NANOSEC_IN_SECOND;
      bc_array_s_append_format(&buffer,"%lldS",seconds);
    }
  }

  string_s_set(this,buffer.used,buffer.data);
}/*}}}*/

int xs_duration_s_parse(xs_duration_s *this,lli *a_period)
{/*{{{*/
  int negative = 0;
  lli period = 0;
  lli value;

  char *ptr = this->data;
  char *end_ptr;

  if (*ptr == '-')
  {
    negative = 1;
    ++ptr;
  }

  if (*ptr++ != 'P')
  {
    throw_error(XSD_DURATION_PARSE_ERROR);
  }

  do
  {
    value = strtoll(ptr,&end_ptr,10);

    // - termination -
    if (*end_ptr == '\0')
    {
      if (ptr != end_ptr)
      {
        throw_error(XSD_DURATION_PARSE_ERROR);
      }

      break;
    }

    switch (*end_ptr)
    {
    case 'D':
      period += value*CL_TIME_NANOSEC_IN_DAY;
      break;
    case 'T':
      break;
    case 'H':
      period += value*CL_TIME_NANOSEC_IN_HOUR;
      break;
    case 'M':
      period += value*CL_TIME_NANOSEC_IN_MINUTE;
      break;
    case 'S':
      period += value*CL_TIME_NANOSEC_IN_SECOND;
      break;
    case '.':
      {
        double secs = strtod(ptr,&end_ptr);

        if (*end_ptr != 'S')
        {
          throw_error(XSD_DURATION_PARSE_ERROR);
        }

        period += secs*CL_TIME_NANOSEC_IN_SECOND;
      }
      break;
    default:
      throw_error(XSD_DURATION_PARSE_ERROR);
    }

    ptr = end_ptr + 1;

  } while(1);

  *a_period = negative ? -period : period;

  return 0;
}/*}}}*/

