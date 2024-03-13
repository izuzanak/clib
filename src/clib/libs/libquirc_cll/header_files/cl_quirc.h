
#ifndef CL_LIB_QUIRC_H
#define CL_LIB_QUIRC_H

@begin
include "cl_struct.h"
@end

#include <quirc.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libquirc_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libquirc_cll_EXPORTS
#define libquirc_cll_EXPORT __declspec(dllexport)
#else
#define libquirc_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_QUIRC_INVALID_IMAGE_SIZE_PROPERTIES 1
#define ERROR_QUIRC_CREATE_INSTANCE_ERROR 2
#define ERROR_QUIRC_PTROCESS_INVALID_DATA_SIZE 3
#define ERROR_QUIRC_INDEX_DOES_NOT_REFER_TO_VALID_CODE 4

#define ERROR_QUIRC_CODE_DECODE_DATA_ERROR 1

typedef struct quirc * quirc_s;
typedef struct quirc_code quirc_code_s;
typedef struct quirc_data quirc_data_s;

// === definition of structure quirc_s =========================================

@begin
define quirc_s dynamic
@end

static inline void quirc_s_init(quirc_s *this);
static inline void quirc_s_clear(quirc_s *this);
static inline void quirc_s_flush_all(quirc_s *this);
static inline void quirc_s_swap(quirc_s *this,quirc_s *a_second);
static inline void quirc_s_copy(const quirc_s *this,const quirc_s *a_src);
static inline int quirc_s_compare(const quirc_s *this,const quirc_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void quirc_s_to_string(const quirc_s *this,bc_array_s *a_trg);
#endif

WUR libquirc_cll_EXPORT int quirc_s_create(quirc_s *this,int a_width,int a_height);
WUR static inline int quirc_s_process(quirc_s *this,const char *a_data,size_t a_size);
static inline int quirc_s_count(const quirc_s *this);
static inline int quirc_s_extract(const quirc_s *this,int a_index,quirc_code_s *a_trg);

// === definition of structure quirc_code_s ====================================

@begin
define quirc_code_s static
@end

static inline int quirc_code_s_compare(const quirc_code_s *this,const quirc_code_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void quirc_code_s_to_string(const quirc_code_s *this,bc_array_s *a_trg);
#endif

static inline int quirc_code_s_decode(const quirc_code_s *this,quirc_data_s *a_trg);

// === definition of structure quirc_data_s ====================================

@begin
define quirc_data_s dynamic
@end

static inline int quirc_data_s_compare(const quirc_data_s *this,const quirc_data_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void quirc_data_s_to_string(const quirc_data_s *this,bc_array_s *a_trg);
#endif

static inline void quirc_data_s_payload(const quirc_data_s *this,bc_array_s *a_trg);

// === inline methods of structure quirc_s =====================================

static inline void quirc_s_init(quirc_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void quirc_s_clear(quirc_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    quirc_destroy(*this);
  }

  quirc_s_init(this);
}/*}}}*/

static inline void quirc_s_flush_all(quirc_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

static inline void quirc_s_swap(quirc_s *this,quirc_s *a_second)
{/*{{{*/
  quirc_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void quirc_s_copy(const quirc_s *this,const quirc_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int quirc_s_compare(const quirc_s *this,const quirc_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void quirc_s_to_string(const quirc_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"quirc_s{%p}",*this);
}/*}}}*/
#endif

static inline int quirc_s_process(quirc_s *this,const char *a_data,size_t a_size)
{/*{{{*/
  int quirc_width;
  int quirc_height;
  uint8_t *quirc_buffer = quirc_begin(*this,&quirc_width,&quirc_height);

  // - ERROR -
  if ((unsigned)(quirc_width*quirc_height) != a_size)
  {
    throw_error(QUIRC_PTROCESS_INVALID_DATA_SIZE);
  }

  // - copy data to quirc buffer -
  memcpy(quirc_buffer,a_data,a_size);

  quirc_end(*this);

  return 0;
}/*}}}*/

static inline int quirc_s_count(const quirc_s *this)
{/*{{{*/
  return quirc_count(*this);
}/*}}}*/

static inline int quirc_s_extract(const quirc_s *this,int a_index,quirc_code_s *a_trg)
{/*{{{*/
  if (a_index < 0 || a_index >= quirc_count(*this))
  {
    throw_error(QUIRC_INDEX_DOES_NOT_REFER_TO_VALID_CODE);
  }

  quirc_extract(*this,a_index,a_trg);

  return 0;
}/*}}}*/

// === inline methods of structure quirc_code_s ================================

static inline int quirc_code_s_compare(const quirc_code_s *this,const quirc_code_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void quirc_code_s_to_string(const quirc_code_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"quirc_code_s{%p}",*this);
}/*}}}*/
#endif

static inline int quirc_code_s_decode(const quirc_code_s *this,quirc_data_s *a_trg)
{/*{{{*/

  // - ERROR -
  if (quirc_decode(this,a_trg) != 0)
  {
    throw_error(QUIRC_CODE_DECODE_DATA_ERROR);
  }

  return 0;
}/*}}}*/

// === inline methods of structure quirc_data_s ================================

static inline int quirc_data_s_compare(const quirc_data_s *this,const quirc_data_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void quirc_data_s_to_string(const quirc_data_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"quirc_data_s{%p}",*this);
}/*}}}*/
#endif

static inline void quirc_data_s_payload(const quirc_data_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append(a_trg,this->payload_len,(const char *)this->payload);
}/*}}}*/

#endif

