
#ifndef __CL_PCRE_H
#define __CL_PCRE_H

@begin
include "cl_struct.h"
@end

#include <pcreposix.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libpcre_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libpcre_cll_EXPORTS
#define libpcre_cll_EXPORT __declspec(dllexport)
#else
#define libpcre_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_PCRE_COMPILE_ERROR 1
#define ERROR_PCRE_EMPTY_MATCH 2

// === definition of structure regmatch_s ======================================

typedef regmatch_t regmatch_s;
@begin
define regmatch_s static
@end

static inline int regmatch_s_compare(const regmatch_s *this,const regmatch_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void regmatch_s_to_string(const regmatch_s *this,bc_array_s *a_trg);
#endif

// === definition of generated structures ======================================

// -- regmatch_array_s --
@begin
array<regmatch_s> regmatch_array_s;
@end

// === definition of structure pcre_s ==========================================

typedef regex_t * pcre_s;
@begin
define pcre_s dynamic
@end

static inline void pcre_s_init(pcre_s *this);
static inline void pcre_s_clear(pcre_s *this);
static inline void pcre_s_flush_all(pcre_s *this);
static inline void pcre_s_swap(pcre_s *this,pcre_s *a_second);
static inline void pcre_s_copy(pcre_s *this,const pcre_s *a_src);
static inline int pcre_s_compare(const pcre_s *this,const pcre_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void pcre_s_to_string(const pcre_s *this,bc_array_s *a_trg);
#endif

WUR static inline int pcre_s_create(pcre_s *this,const char *a_data);
static inline int pcre_s_match(pcre_s *this,const char *a_data,regmatch_s *a_match);
libpcre_cll_EXPORT int pcre_s_match_n(pcre_s *this,const char *a_data,size_t a_match_cnt,regmatch_array_s *a_matches);
WUR libpcre_cll_EXPORT int pcre_s_split(pcre_s *this,const string_s *a_src,string_array_s *a_trg);
libpcre_cll_EXPORT int pcre_s_replace(pcre_s *this,const string_s *a_src,const string_s *a_new,bc_array_s *a_trg);

// === inline methods of structure regmatch_s ==================================

static inline int regmatch_s_compare(const regmatch_s *this,const regmatch_s *a_second)
{/*{{{*/
  return this->rm_so == a_second->rm_so && this->rm_eo == a_second->rm_eo;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void regmatch_s_to_string(const regmatch_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"regmatch_s{%d,%d}",this->rm_so,this->rm_eo);
}/*}}}*/
#endif

// === inline methods of generated structures ==================================

// -- regmatch_array_s --
@begin
inlines regmatch_array_s
@end

// === inline methods of structure pcre_s ======================================

static inline void pcre_s_init(pcre_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void pcre_s_clear(pcre_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    regfree(*this);
    cfree(*this);
  }

  pcre_s_init(this);
}/*}}}*/

static inline void pcre_s_flush_all(pcre_s *this)
{/*{{{*/
}/*}}}*/

static inline void pcre_s_swap(pcre_s *this,pcre_s *a_second)
{/*{{{*/
  pcre_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void pcre_s_copy(pcre_s *this,const pcre_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int pcre_s_compare(const pcre_s *this,const pcre_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void pcre_s_to_string(const pcre_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"pcre_s{%p}",*this);
}/*}}}*/
#endif

static inline int pcre_s_create(pcre_s *this,const char *a_data)
{/*{{{*/
  pcre_s_clear(this);

  // - allocate regular expression pointer -
  regex_t *re = (regex_t *)cmalloc(sizeof(regex_t));

  // - ERROR -
  if (regcomp(re,a_data,0) != 0)
  {
    cfree(re);

    throw_error(PCRE_COMPILE_ERROR);
  }

  *this = re;

  return 0;
}/*}}}*/

static inline int pcre_s_match(pcre_s *this,const char *a_data,regmatch_s *a_match)
{/*{{{*/
  debug_assert(*this != NULL);

  // - execute regular expression -
  return regexec(*this,a_data,1,a_match,0) == 0;
}/*}}}*/

#endif

