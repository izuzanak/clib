
#ifndef __CL_REGEX_H
#define __CL_REGEX_H

@begin
include "cl_struct.h"
@end

#include <regex.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libregex_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libregex_cll_EXPORTS
#define libregex_cll_EXPORT __declspec(dllexport)
#else
#define libregex_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_REGEX_COMPILE_ERROR 1
#define ERROR_REGEX_EMPTY_MATCH 2

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

// === definition of structure regex_s =========================================

typedef regex_t * regex_s;
@begin
define regex_s dynamic
@end

static inline void regex_s_init(regex_s *this);
static inline void regex_s_clear(regex_s *this);
static inline void regex_s_flush_all(regex_s *this);
static inline void regex_s_swap(regex_s *this,regex_s *a_second);
static inline void regex_s_copy(regex_s *this,const regex_s *a_src);
static inline int regex_s_compare(const regex_s *this,const regex_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void regex_s_to_string(const regex_s *this,bc_array_s *a_trg);
#endif

WUR static inline int regex_s_create(regex_s *this,const char *a_data);
static inline int regex_s_match(regex_s *this,const char *a_data,regmatch_s *a_match);
libregex_cll_EXPORT int regex_s_match_n(regex_s *this,const char *a_data,size_t a_match_cnt,regmatch_array_s *a_matches);
WUR libregex_cll_EXPORT int regex_s_split(regex_s *this,const string_s *a_src,string_array_s *a_trg);
libregex_cll_EXPORT int regex_s_replace(regex_s *this,const string_s *a_src,const string_s *a_new,bc_array_s *a_trg);

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

// === inline methods of structure regex_s =====================================

static inline void regex_s_init(regex_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void regex_s_clear(regex_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    regfree(*this);
    cfree(*this);
  }

  regex_s_init(this);
}/*}}}*/

static inline void regex_s_flush_all(regex_s *this)
{/*{{{*/
}/*}}}*/

static inline void regex_s_swap(regex_s *this,regex_s *a_second)
{/*{{{*/
  regex_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void regex_s_copy(regex_s *this,const regex_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int regex_s_compare(const regex_s *this,const regex_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void regex_s_to_string(const regex_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"regex_s{%p}",*this);
}/*}}}*/
#endif

static inline int regex_s_create(regex_s *this,const char *a_data)
{/*{{{*/
  regex_s_clear(this);

  // - allocate regular expression pointer -
  regex_t *re = (regex_t *)cmalloc(sizeof(regex_t));

  // - ERROR -
  if (regcomp(re,a_data,0) != 0)
  {
    cfree(re);

    throw_error(REGEX_COMPILE_ERROR);
  }

  *this = re;

  return 0;
}/*}}}*/

static inline int regex_s_match(regex_s *this,const char *a_data,regmatch_s *a_match)
{/*{{{*/
  debug_assert(*this != NULL);

  // - execute regular expression -
  return regexec(*this,a_data,1,a_match,0) == 0;
}/*}}}*/

#endif

