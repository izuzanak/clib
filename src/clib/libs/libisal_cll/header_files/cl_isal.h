
#ifndef CL_LIB_ISAL_H
#define CL_LIB_ISAL_H

@begin
include "cl_struct.h"
@end

#include <isa-l.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libisal_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libisal_cll_EXPORTS
#define libisal_cll_EXPORT __declspec(dllexport)
#else
#define libisal_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_ISAL_DUMMY_ERROR 1

// === definition of structure isal_crc16_s ====================================

typedef usi isal_crc16_s;
@begin
define isal_crc16_s basic
@end

#if OPTION_TO_STRING == ENABLED
static inline void isal_crc16_s_to_string(const isal_crc16_s *this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void isal_crc16_s_to_json(const isal_crc16_s *this,bc_array_s *a_trg);
#endif
static inline void isal_crc16_s_update(isal_crc16_s *this,
    uint64_t a_size,const void *a_data);
static inline int isal_crc16_s_valid(const isal_crc16_s *this,
    isal_crc16_s a_init,uint64_t a_size,const void *a_data);

// === inline methods of structure isal_crc16_s ================================

#if OPTION_TO_STRING == ENABLED
static inline void isal_crc16_s_to_string(const isal_crc16_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"isal_crc16_s{0x%4.4hx}",*this);
}/*}}}*/
#endif

#if OPTION_TO_JSON == ENABLED
static inline void isal_crc16_s_to_json(const isal_crc16_s *this,bc_array_s *a_trg)
{/*{{{*/
  usi_to_json(this,a_trg);
}/*}}}*/
#endif

static inline int isal_crc16_s_valid(const isal_crc16_s *this,
    isal_crc16_s a_init,uint64_t a_size,const void *a_data)
{/*{{{*/
  isal_crc16_s crc = a_init;
  isal_crc16_s_update(&crc,a_size,a_data);

  return *this == crc;
}/*}}}*/

static inline void isal_crc16_s_update(isal_crc16_s *this,
    uint64_t a_size,const void *a_data)
{//{{{
  *this = crc16_t10dif(*this,a_data,a_size);
}//}}}

#endif

