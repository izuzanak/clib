
#ifndef CL_LIB_UTF8PROC_H
#define CL_LIB_UTF8PROC_H

@begin
include "cl_struct.h"
@end

#include "utf8proc.h"

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libutf8proc_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libutf8proc_cll_EXPORTS
#define libutf8proc_cll_EXPORT __declspec(dllexport)
#else
#define libutf8proc_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_UTF8PROC_UTF8_SEQUENCE_INVALID_CODE_POINT 1
#define ERROR_UTF8PROC_UTF8_SEQUENCE_MAP_ERROR 2

// === definition of structure utf8proc_s ======================================

WUR libutf8proc_cll_EXPORT int utf8proc_s_to_lower(unsigned a_size,const char *a_src,bc_array_s *a_trg);
WUR libutf8proc_cll_EXPORT int utf8proc_s_to_upper(unsigned a_size,const char *a_src,bc_array_s *a_trg);
WUR libutf8proc_cll_EXPORT int utf8proc_s_map(unsigned a_size,const char *a_src,
    utf8proc_option_t a_options,bc_array_s *a_trg);
static inline void utf8proc_s_version(bc_array_s *a_trg);

// === inline methods of structure utf8proc_s ==================================

static inline void utf8proc_s_version(bc_array_s *a_trg)
{/*{{{*/
  const char *version = utf8proc_version();
  bc_array_s_append_ptr(a_trg,version);
}/*}}}*/

#endif

