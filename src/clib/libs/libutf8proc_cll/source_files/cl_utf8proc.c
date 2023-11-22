
@begin
include "cl_utf8proc.h"
@end

// === methods of structure utf8proc_s =========================================

#define UTF8PROC_S_TRANSFORM_STRING(CODE_POINT_CODE) \
/*{{{*/\
\
  /* - create target buffer - */\
  bc_array_s_reserve(a_trg,a_size);\
  unsigned orig_used = a_trg->used;\
\
  if (a_size > 0)\
  {\
    const utf8proc_uint8_t *s_ptr = (const utf8proc_uint8_t *)a_src;\
    const utf8proc_uint8_t *s_ptr_end = s_ptr + a_size;\
\
    utf8proc_int32_t code_point;\
    do {\
\
      /* - retrieve next code point - */\
      s_ptr += utf8proc_iterate(s_ptr,s_ptr_end - s_ptr,&code_point);\
\
      /* - ERROR - */\
      if (code_point < 0)\
      {\
        /* - remove converted stub - */\
        a_trg->used = orig_used;\
\
        throw_error(UTF8PROC_UTF8_SEQUENCE_INVALID_CODE_POINT);\
      }\
\
      CODE_POINT_CODE;\
\
      /* - encode code point to target buffer - */\
      bc_array_s_reserve(a_trg,4);\
      a_trg->used += utf8proc_encode_char(code_point,\
          (utf8proc_uint8_t *)(a_trg->data + a_trg->used));\
\
    } while(s_ptr < s_ptr_end);\
  }\
/*}}}*/

int utf8proc_s_to_lower(unsigned a_size,const char *a_src,bc_array_s *a_trg)
{/*{{{*/
  UTF8PROC_S_TRANSFORM_STRING(

    // - code point to lower -
    code_point = utf8proc_tolower(code_point);
  );

  return 0;
}/*}}}*/

int utf8proc_s_to_upper(unsigned a_size,const char *a_src,bc_array_s *a_trg)
{/*{{{*/
  UTF8PROC_S_TRANSFORM_STRING(

    // - code point to lower -
    code_point = utf8proc_toupper(code_point);
  );

  return 0;
}/*}}}*/

int utf8proc_s_map(unsigned a_size,const char *a_src,utf8proc_option_t a_options,bc_array_s *a_trg)
{/*{{{*/
  utf8proc_uint8_t *output = NULL;
  utf8proc_ssize_t length = utf8proc_map(
      (const utf8proc_uint8_t *)a_src,a_size,&output,a_options);

  // - ERROR -
  if (length < 0)
  {
    throw_error(UTF8PROC_UTF8_SEQUENCE_MAP_ERROR);
  }

  bc_array_s_append(a_trg,length,(const char *)output);
  free(output);

  return 0;
}/*}}}*/

