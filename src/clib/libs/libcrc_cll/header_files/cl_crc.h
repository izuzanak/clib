
#ifndef CL_LIB_CRC_H
#define CL_LIB_CRC_H

@begin
include "cl_struct.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libcrc_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libcrc_cll_EXPORTS
#define libcrc_cll_EXPORT __declspec(dllexport)
#else
#define libcrc_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_CRC_DUMMY_ERROR 1

// === definition of structure crc16_s =========================================

typedef usi crc16_s;
@begin
define crc16_s basic
@end

#if OPTION_TO_STRING == ENABLED
static inline void crc16_s_to_string(const crc16_s *this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void crc16_s_to_json(const crc16_s *this,bc_array_s *a_trg);
#endif
libcrc_cll_EXPORT void crc16_s_update(crc16_s *this,unsigned a_size,const void *a_data);
static inline int crc16_s_valid(const crc16_s *this,crc16_s a_init,unsigned a_size,const void *a_data);

// === definition of structure crc32_s =========================================

typedef ui crc32_s;
@begin
define crc32_s basic
@end

#if OPTION_TO_STRING == ENABLED
static inline void crc32_s_to_string(const crc32_s *this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void crc32_s_to_json(const crc32_s *this,bc_array_s *a_trg);
#endif
libcrc_cll_EXPORT void crc32_s_update(crc32_s *this,unsigned a_size,const void *a_data);
static inline int crc32_s_valid(const crc32_s *this,crc32_s a_init,unsigned a_size,const void *a_data);

// === inline methods of structure crc16_s =====================================

#if OPTION_TO_STRING == ENABLED
static inline void crc16_s_to_string(const crc16_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"crc16_s{0x%4.4hx}",*this);
}/*}}}*/
#endif

#if OPTION_TO_JSON == ENABLED
static inline void crc16_s_to_json(const crc16_s *this,bc_array_s *a_trg)
{/*{{{*/
  usi_to_json(this,a_trg);
}/*}}}*/
#endif

static inline int crc16_s_valid(const crc16_s *this,crc16_s a_init,unsigned a_size,const void *a_data)
{/*{{{*/
  crc16_s crc = a_init;
  crc16_s_update(&crc,a_size,a_data);

  return *this == crc;
}/*}}}*/

// === inline methods of structure crc32_s =====================================

#if OPTION_TO_STRING == ENABLED
static inline void crc32_s_to_string(const crc32_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"crc32_s{0x%8.8x}",*this);
}/*}}}*/
#endif

#if OPTION_TO_JSON == ENABLED
static inline void crc32_s_to_json(const crc32_s *this,bc_array_s *a_trg)
{/*{{{*/
  ui_to_json(this,a_trg);
}/*}}}*/
#endif

static inline int crc32_s_valid(const crc32_s *this,crc32_s a_init,unsigned a_size,const void *a_data)
{/*{{{*/
  crc32_s crc = a_init;
  crc32_s_update(&crc,a_size,a_data);

  return *this == crc;
}/*}}}*/

#endif

