
#ifndef CL_LIB_XSD_BASIC_H
#define CL_LIB_XSD_BASIC_H

@begin
include "cl_time.h"
include "cl_xml.h"
include "cl_regex.h"
@end

// - error codes -
#define ERROR_XSD_INVALID_DATA_ERROR 1
#define ERROR_XSD_DATE_TIME_PARSE_ERROR 2
#define ERROR_XSD_DURATION_PARSE_ERROR 3

// === constants and definitions ===============================================

extern const char XML_HEADER[];

// === definition of generated structures ======================================

// -- lli_array_s --
@begin
array<lli> lli_array_s;
@end

// -- bd_array_s --
@begin
array<bd> bd_array_s;
@end

// -- xs_boolean_value_s --
@begin
abbreviation bi xs_boolean_value_s
@end

WUR int xs_boolean_value_s_write(const xs_boolean_value_s *this,bc_array_s *a_trg);
WUR int xs_boolean_value_s_read(xs_boolean_value_s *this,var_s a_var);

// -- xs_bool_s --
@begin
abbreviation bi xs_bool_s
@end

WUR int xs_bool_s_write(const xs_bool_s *this,bc_array_s *a_trg);
WUR int xs_bool_s_read(xs_bool_s *this,var_s a_var);

// -- xs_boolean_s --
@begin
abbreviation bi xs_boolean_s
@end

WUR int xs_boolean_s_write(const xs_boolean_s *this,bc_array_s *a_trg);
WUR int xs_boolean_s_read(xs_boolean_s *this,var_s a_var);

// -- xs_byte_s --
@begin
abbreviation bi xs_byte_s
@end

WUR int xs_byte_s_write(const xs_byte_s *this,bc_array_s *a_trg);
WUR int xs_byte_s_read(xs_byte_s *this,var_s a_var);

// -- xs_int_value_s --
@begin
abbreviation lli xs_int_value_s
@end

WUR int xs_int_value_s_write(const xs_int_value_s *this,bc_array_s *a_trg);
WUR int xs_int_value_s_read(xs_int_value_s *this,var_s a_var);

// -- xs_int_s --
@begin
abbreviation lli xs_int_s
@end

WUR int xs_int_s_write(const xs_int_s *this,bc_array_s *a_trg);
WUR int xs_int_s_read(xs_int_s *this,var_s a_var);

// -- xs_integer_value_s --
@begin
abbreviation lli xs_integer_value_s
@end

WUR int xs_integer_value_s_write(const xs_integer_value_s *this,bc_array_s *a_trg);
WUR int xs_integer_value_s_read(xs_integer_value_s *this,var_s a_var);

// -- xs_integer_s --
@begin
abbreviation lli xs_integer_s
@end

WUR int xs_integer_s_write(const xs_integer_s *this,bc_array_s *a_trg);
WUR int xs_integer_s_read(xs_integer_s *this,var_s a_var);

// -- xs_unsignedInt_s --
@begin
abbreviation lli xs_unsignedInt_s
@end

WUR int xs_unsignedInt_s_write(const xs_unsignedInt_s *this,bc_array_s *a_trg);
WUR int xs_unsignedInt_s_read(xs_unsignedInt_s *this,var_s a_var);

// -- xs_unsignedLong_s --
@begin
abbreviation ulli xs_unsignedLong_s
@end

WUR int xs_unsignedLong_s_write(const xs_unsignedLong_s *this,bc_array_s *a_trg);
WUR int xs_unsignedLong_s_read(xs_unsignedLong_s *this,var_s a_var);

// -- xs_nonNegativeInteger_s --
@begin
abbreviation lli xs_nonNegativeInteger_s
@end

WUR int xs_nonNegativeInteger_s_write(const xs_nonNegativeInteger_s *this,bc_array_s *a_trg);
WUR int xs_nonNegativeInteger_s_read(xs_nonNegativeInteger_s *this,var_s a_var);

// -- xs_float_value_s --
@begin
abbreviation bf xs_float_value_s
@end

WUR int xs_float_value_s_write(const xs_float_value_s *this,bc_array_s *a_trg);
WUR int xs_float_value_s_read(xs_float_value_s *this,var_s a_var);

// -- xs_float_s --
@begin
abbreviation bf xs_float_s
@end

WUR int xs_float_s_write(const xs_float_s *this,bc_array_s *a_trg);
WUR int xs_float_s_read(xs_float_s *this,var_s a_var);

// -- xs_double_value_s --
@begin
abbreviation bd xs_double_value_s
@end

WUR int xs_double_value_s_write(const xs_double_value_s *this,bc_array_s *a_trg);
WUR int xs_double_value_s_read(xs_double_value_s *this,var_s a_var);

// -- xs_double_s --
@begin
abbreviation bd xs_double_s
@end

WUR int xs_double_s_write(const xs_double_s *this,bc_array_s *a_trg);
WUR int xs_double_s_read(xs_double_s *this,var_s a_var);

// -- xs_decimal_s --
@begin
abbreviation bd xs_decimal_s
@end

WUR int xs_decimal_s_write(const xs_decimal_s *this,bc_array_s *a_trg);
WUR int xs_decimal_s_read(xs_decimal_s *this,var_s a_var);

// -- xs_token_s --
@begin
abbreviation string_s xs_token_s
@end

WUR int xs_token_s_write(const xs_token_s *this,bc_array_s *a_trg);
WUR int xs_token_s_read(xs_token_s *this,var_s a_var);

// -- xs_string_value_s --
@begin
abbreviation string_s xs_string_value_s
@end

WUR int xs_string_value_s_write(const xs_string_value_s *this,bc_array_s *a_trg);
WUR int xs_string_value_s_read(xs_string_value_s *this,var_s a_var);

// -- xs_string_s --
@begin
abbreviation string_s xs_string_s
@end

WUR int xs_string_s_write(const xs_string_s *this,bc_array_s *a_trg);
WUR int xs_string_s_read(xs_string_s *this,var_s a_var);

// -- xs_language_s --
@begin
abbreviation string_s xs_language_s
@end

WUR int xs_language_s_write(const xs_language_s *this,bc_array_s *a_trg);
WUR int xs_language_s_read(xs_language_s *this,var_s a_var);

// -- xs_NMTOKEN_s --
@begin
abbreviation string_s xs_NMTOKEN_s
@end

WUR int xs_NMTOKEN_s_write(const xs_NMTOKEN_s *this,bc_array_s *a_trg);
WUR int xs_NMTOKEN_s_read(xs_NMTOKEN_s *this,var_s a_var);

// -- xs_anyURI_value_s --
@begin
abbreviation string_s xs_anyURI_value_s
@end

WUR int xs_anyURI_value_s_write(const xs_anyURI_value_s *this,bc_array_s *a_trg);
WUR int xs_anyURI_value_s_read(xs_anyURI_value_s *this,var_s a_var);

// -- xs_anyURI_s --
@begin
abbreviation string_s xs_anyURI_s
@end

WUR int xs_anyURI_s_write(const xs_anyURI_s *this,bc_array_s *a_trg);
WUR int xs_anyURI_s_read(xs_anyURI_s *this,var_s a_var);

// -- xs_anySimpleType_value_s --
@begin
abbreviation string_s xs_anySimpleType_value_s
@end

WUR int xs_anySimpleType_value_s_write(const xs_anySimpleType_value_s *this,bc_array_s *a_trg);
WUR int xs_anySimpleType_value_s_read(xs_anySimpleType_value_s *this,var_s a_var);

// -- xs_anyType_s --
@begin
abbreviation var_s xs_anyType_s
@end

WUR int xs_anyType_s_write(const xs_anyType_s *this,bc_array_s *a_trg);
WUR int xs_anyType_s_read(xs_anyType_s *this,var_s a_var);

// -- xs_QName_value_s --
@begin
abbreviation string_s xs_QName_value_s
@end

WUR int xs_QName_value_s_write(const xs_QName_value_s *this,bc_array_s *a_trg);
WUR int xs_QName_value_s_read(xs_QName_value_s *this,var_s a_var);

// -- xs_QName_s --
@begin
abbreviation string_s xs_QName_s
@end

WUR int xs_QName_s_write(const xs_QName_s *this,bc_array_s *a_trg);
WUR int xs_QName_s_read(xs_QName_s *this,var_s a_var);

// -- xs_base64Binary_value_s --
@begin
abbreviation string_s xs_base64Binary_value_s
@end

WUR int xs_base64Binary_value_s_write(const xs_base64Binary_value_s *this,bc_array_s *a_trg);
WUR int xs_base64Binary_value_s_read(xs_base64Binary_value_s *this,var_s a_var);

// -- xs_base64Binary_s --
@begin
abbreviation string_s xs_base64Binary_s
@end

WUR int xs_base64Binary_s_write(const xs_base64Binary_s *this,bc_array_s *a_trg);
WUR int xs_base64Binary_s_read(xs_base64Binary_s *this,var_s a_var);

// -- xs_dateTime_value_s --
@begin
abbreviation string_s xs_dateTime_value_s
@end

WUR int xs_dateTime_value_s_write(const xs_dateTime_value_s *this,bc_array_s *a_trg);
WUR int xs_dateTime_value_s_read(xs_dateTime_value_s *this,var_s a_var);

// -- xs_date_s --
@begin
abbreviation string_s xs_date_s
@end

WUR int xs_date_s_write(const xs_date_s *this,bc_array_s *a_trg);
WUR int xs_date_s_read(xs_date_s *this,var_s a_var);

void xs_date_s_create(xs_date_s *this,time_s a_time);
WUR int xs_date_s_parse(xs_date_s *this,time_s *a_time);

// -- xs_time_s --
@begin
abbreviation string_s xs_time_s
@end

WUR int xs_time_s_write(const xs_time_s *this,bc_array_s *a_trg);
WUR int xs_time_s_read(xs_time_s *this,var_s a_var);

void xs_time_s_create(xs_time_s *this,time_s a_time);
WUR int xs_time_s_parse(xs_time_s *this,time_s *a_time);

// -- xs_dateTime_s --
@begin
abbreviation string_s xs_dateTime_s
@end

WUR int xs_dateTime_s_write(const xs_dateTime_s *this,bc_array_s *a_trg);
WUR int xs_dateTime_s_read(xs_dateTime_s *this,var_s a_var);

void xs_dateTime_s_create(xs_dateTime_s *this,time_s a_time);
WUR int xs_dateTime_s_parse(xs_dateTime_s *this,time_s *a_time);

// -- xs_duration_s --
@begin
abbreviation string_s xs_duration_s
@end

WUR int xs_duration_s_write(const xs_duration_s *this,bc_array_s *a_trg);
WUR int xs_duration_s_read(xs_duration_s *this,var_s a_var);

void xs_duration_s_create(xs_duration_s *this,lli a_period);
WUR int xs_duration_s_parse(xs_duration_s *this,lli *a_period);

// === inline methods of generated structures ==================================

// -- lli_array_s --
@begin
inlines lli_array_s
@end

// -- bd_array_s --
@begin
inlines bd_array_s
@end

#endif

