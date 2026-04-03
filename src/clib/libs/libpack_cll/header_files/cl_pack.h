
#ifndef CL_LIB_PACK_H
#define CL_LIB_PACK_H

@begin
include "cl_var.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libpack_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libpack_cll_EXPORTS
#define libpack_cll_EXPORT __declspec(dllexport)
#else
#define libpack_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_PACK_INVALID_FORMAT_STRING       1
#define ERROR_PACK_NOT_ENOUGH_ARGUMENTS        2
#define ERROR_PACK_WRONG_ARGUMENT_TYPE         3
#define ERROR_PACK_NOT_ALL_ARGUMENTS_CONVERTED 4
#define ERROR_PACK_STRING_EXCEEDS_TARGET       5
#define ERROR_PACK_STRING_NOT_TERMINATED       6
#define ERROR_PACK_NOT_ENOUGH_DATA             7

// - pack code/decode terminals -
enum {
  c_pack_term_NUMBER = 0,
  c_pack_term_NATIVE,
  c_pack_term_LITTLE_ENDIAN,
  c_pack_term_BIG_ENDIAN,
  c_pack_term_CHAR,
  c_pack_term_SIGNED_BYTE,
  c_pack_term_UNSIGNED_BYTE,
  c_pack_term_SHORT,
  c_pack_term_UNSIGNED_SHORT,
  c_pack_term_INT,
  c_pack_term_UNSIGNED,
  c_pack_term_LONG_INT,
  c_pack_term_LONG_UNSIGNED,
  c_pack_term_LONG_LONG_INT,
  c_pack_term_LONG_LONG_UNSIGNED,
  c_pack_term_FLOAT,
  c_pack_term_DOUBLE,
  c_pack_term_STRING,
  c_pack_term_ZERO_TERM_STRING,
  c_pack_term_END,
};

// === function declarations ===================================================

// - va_args encode -
libpack_cll_EXPORT WUR int pack_s_code(bc_array_s *a_trg,const char *a_format,...);
libpack_cll_EXPORT WUR int pack_s_code_ap(bc_array_s *a_trg,const char *a_format,va_list a_ap);

// - va_args decode -
libpack_cll_EXPORT WUR int pack_s_decode(const char *a_format,const char *a_data,unsigned a_size,unsigned *a_read,...);
libpack_cll_EXPORT WUR int pack_s_decode_ap(const char *a_format,const char *a_data,unsigned a_size,unsigned *a_read,va_list a_ap);

// - var_s encode/decode -
libpack_cll_EXPORT WUR int pack_s_code_var(bc_array_s *a_trg,const char *a_format,var_s a_values);
libpack_cll_EXPORT WUR int pack_s_decode_var(const char *a_format,const char *a_data,unsigned a_size,unsigned *a_read,var_s *a_result);

// - utility -
libpack_cll_EXPORT WUR int pack_s_calc_size(const char *a_format,unsigned *a_size);

#endif

