
#ifndef CL_LIB_ICONV_H
#define CL_LIB_ICONV_H

@begin
include "cl_struct.h"
@end

#include <iconv.h>
#include <errno.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libiconv_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libiconv_cll_EXPORTS
#define libiconv_cll_EXPORT __declspec(dllexport)
#else
#define libiconv_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_ICONV_OPEN_ERROR 1
#define ERROR_ICONV_CONVERT_ERROR 2

// === definition of structure iconv_s =========================================

typedef iconv_t iconv_s;
@begin
define iconv_s dynamic
@end

static inline void iconv_s_init(iconv_s *this);
static inline void iconv_s_clear(iconv_s *this);
static inline void iconv_s_flush_all(iconv_s *this);
static inline void iconv_s_swap(iconv_s *this,iconv_s *a_second);
static inline void iconv_s_copy(iconv_s *this,const iconv_s *a_src);
static inline int iconv_s_compare(const iconv_s *this,const iconv_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void iconv_s_to_string(const iconv_s *this,bc_array_s *a_trg);
#endif

WUR static inline int iconv_s_create(iconv_s *this,const char *a_tocode,const char *a_fromcode);
WUR libiconv_cll_EXPORT int iconv_s_convert(iconv_s *this,unsigned a_size,const char *a_src,bc_array_s *a_trg);

// === inline methods of structure iconv_s =====================================

static inline void iconv_s_init(iconv_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void iconv_s_clear(iconv_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    iconv_close(*this);
  }

  iconv_s_init(this);
}/*}}}*/

static inline void iconv_s_flush_all(iconv_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

static inline void iconv_s_swap(iconv_s *this,iconv_s *a_second)
{/*{{{*/
  iconv_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void iconv_s_copy(iconv_s *this,const iconv_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int iconv_s_compare(const iconv_s *this,const iconv_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void iconv_s_to_string(const iconv_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"iconv_s{%p}",*this);
}/*}}}*/
#endif

static inline int iconv_s_create(iconv_s *this,const char *a_tocode,const char *a_fromcode)
{/*{{{*/
  iconv_s_clear(this);

  // - open iconv descriptor -
  iconv_t cd = iconv_open(a_tocode,a_fromcode);

  // - ERROR -
  if (cd == (iconv_t)-1)
  {
    throw_error(ICONV_OPEN_ERROR);
  }

  *this = cd;

  return 0;
}/*}}}*/

#endif
