
#ifndef __CL_UUID_H // NOLINT
#define __CL_UUID_H

@begin
include "cl_struct.h"
@end

#include <uuid/uuid.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libuuid_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libuuid_cll_EXPORTS
#define libuuid_cll_EXPORT __declspec(dllexport)
#else
#define libuuid_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_UUID_PARSE_ERROR 1

// === definition of structure uuid_s ==========================================

typedef struct uuid_s
{
  uuid_t value;
} uuid_s;

@begin
define uuid_s static
@end

static inline int uuid_s_compare(const uuid_s *this,const uuid_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void uuid_s_to_string(uuid_s *this,bc_array_s *a_trg);
#endif

static inline void uuid_s_generate(uuid_s *this);
WUR static inline int uuid_s_parse(uuid_s *this,char *a_data);
static inline void uuid_s_unparse(uuid_s *this,bc_array_s *a_trg);

// === inline methods of structure uuid_s ======================================

static inline int uuid_s_compare(const uuid_s *this,const uuid_s *a_second)
{/*{{{*/
  return uuid_compare(this->value,a_second->value) == 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void uuid_s_to_string(uuid_s *this,bc_array_s *a_trg)
{/*{{{*/
  uuid_s_unparse(this,a_trg);
}/*}}}*/
#endif

static inline void uuid_s_generate(uuid_s *this)
{/*{{{*/
  uuid_generate(this->value);
}/*}}}*/

static inline int uuid_s_parse(uuid_s *this,char *a_data)
{/*{{{*/
  if (uuid_parse(a_data,this->value) != 0)
  {
    throw_error(UUID_PARSE_ERROR);
  }

  return 0;
}/*}}}*/

static inline void uuid_s_unparse(uuid_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_reserve(a_trg,37);
  uuid_unparse(this->value,a_trg->data + a_trg->used);
  a_trg->used += 36;
}/*}}}*/

#endif

