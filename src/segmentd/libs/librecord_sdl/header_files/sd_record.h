
#ifndef __SD_RECORD_H // NOLINT
#define __SD_RECORD_H

@begin
include "cl_time.h"
include "cl_crc.h"
include "cl_linux.h"
include "cl_logger.h"
include "sd_conf.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define librecord_sdl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef librecord_sdl_EXPORTS
#define librecord_sdl_EXPORT __declspec(dllexport)
#else
#define librecord_sdl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_SYSTEM_MSYNC_ERROR 1

// === constants and definitions ===============================================

#define RECORD_CRC_INIT_VALUE 0x5555

extern unsigned g_sd_page_size;

// === definition of generated structures ======================================

// -- bc_array_s --

static inline void bc_array_s_append_be_usi(bc_array_s *this,usi a_value);
static inline void bc_array_s_append_be_ui(bc_array_s *this,ui a_value);
static inline void bc_array_s_append_be_lli(bc_array_s *this,lli a_value);
static inline void bc_array_s_append_be_ulli(bc_array_s *this,ulli a_value);
static inline void bc_array_s_append_sd_segmentd_msg_header(bc_array_s *this,
    ulli a_id,usi a_type,usi a_req_id,const string_s *a_segment_id);

// -- sd_record_header_s --
@begin
struct
<
crc16_s:crc
lli:id
time_s:time
ui:data_size
>
sd_record_header_s;
@end

// -- sd_record_timestamp_s --
@begin
struct
<
lli:id
time_s:time
>
sd_record_timestamp_s;
@end

// === definition of structure sd_record_s ==================================

typedef struct sd_record_s sd_record_s;

struct sd_record_s
{
  sd_record_header_s header;
  char data[];
};

static inline void sd_record_s_init(sd_record_s *this);
static inline void sd_record_s_clear(sd_record_s *this);
static inline void sd_record_s_set(sd_record_s *this,const sd_record_header_s *a_header,
    unsigned a_size,const bc *a_data);
static inline void sd_record_s_flush_all(sd_record_s *this);
static inline void sd_record_s_swap(sd_record_s *this,sd_record_s *a_second);
static inline void sd_record_s_copy(sd_record_s *this,const sd_record_s *a_src,unsigned a_rec_size);
static inline int sd_record_s_compare(const sd_record_s *this,const sd_record_s *a_second,unsigned a_rec_size);
#if OPTION_TO_STRING == ENABLED
static inline void sd_record_s___to_string(const sd_record_s *this,bc_array_s *a_trg);
#define sd_record_s_to_string sd_record_s___to_string
static inline void sd_record_s_to_string_separator(const sd_record_s *this,bc_array_s *a_trg,unsigned a_count,const char *a_data);
#endif

static inline crc16_s sd_record_s_compute_crc(const sd_record_s *this,unsigned a_rec_size);
WUR static inline int sd_record_s_msync(const sd_record_s *this,unsigned a_rec_size);

// === definition of global functions ==========================================

void librecord_sdl_init();
void librecord_sdl_clear();

// === inline methods of generated structures ==================================

// -- bc_array_s --

static inline void bc_array_s_append_be_usi(bc_array_s *this,usi a_value)
{/*{{{*/
  a_value = htobe16(a_value);
  bc_array_s_append(this,sizeof(usi),(char *)&a_value);
}/*}}}*/

static inline void bc_array_s_append_be_ui(bc_array_s *this,ui a_value)
{/*{{{*/
  a_value = htobe32(a_value);
  bc_array_s_append(this,sizeof(ui),(char *)&a_value);
}/*}}}*/

static inline void bc_array_s_append_be_lli(bc_array_s *this,lli a_value)
{/*{{{*/
  a_value = htobe64(a_value);
  bc_array_s_append(this,sizeof(lli),(char *)&a_value);
}/*}}}*/

static inline void bc_array_s_append_be_ulli(bc_array_s *this,ulli a_value)
{/*{{{*/
  a_value = htobe64(a_value);
  bc_array_s_append(this,sizeof(ulli),(char *)&a_value);
}/*}}}*/

static inline void bc_array_s_append_sd_segmentd_msg_header(bc_array_s *this,
    ulli a_id,usi a_type,usi a_req_id,const string_s *a_segment_id)
{/*{{{*/
  bc_array_s_append_be_ulli(this,a_id);
  bc_array_s_append_be_usi(this,a_type);
  bc_array_s_append_be_usi(this,a_req_id);
  bc_array_s_append(this,a_segment_id->size,a_segment_id->data);
}/*}}}*/

// -- sd_record_timestamp_s --
@begin
inlines sd_record_timestamp_s
@end

// -- sd_record_header_s --
@begin
inlines sd_record_header_s
@end

// === inline methods of structure sd_record_s ==============================

static inline void sd_record_s_init(sd_record_s *this)
{/*{{{*/
}/*}}}*/

static inline void sd_record_s_clear(sd_record_s *this)
{/*{{{*/
}/*}}}*/

static inline void sd_record_s_set(sd_record_s *this,const sd_record_header_s *a_header,
  unsigned a_size,const bc *a_data)
{/*{{{*/
  this->header = *a_header;
  memcpy(this->data,a_data,a_size);
}/*}}}*/

static inline void sd_record_s_flush_all(sd_record_s *this)
{/*{{{*/
}/*}}}*/

static inline void sd_record_s_swap(sd_record_s *this,sd_record_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

static inline void sd_record_s_copy(sd_record_s *this,const sd_record_s *a_src,unsigned a_rec_size)
{/*{{{*/
  memcpy(this,a_src,a_rec_size);
}/*}}}*/

static inline int sd_record_s_compare(const sd_record_s *this,const sd_record_s *a_second,unsigned a_rec_size)
{/*{{{*/
  return memcmp(this,a_second,a_rec_size) == 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void sd_record_s___to_string(const sd_record_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"{header:");
  sd_record_header_s_to_string(&this->header,a_trg);
  bc_array_s_append_ptr(a_trg,",data:{...}");
  bc_array_s_push(a_trg,'}');
}/*}}}*/
#endif

#if OPTION_TO_STRING == ENABLED
static inline void sd_record_s_to_string_separator(const sd_record_s *this,bc_array_s *a_trg,unsigned a_count,const char *a_data)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"header:");
  sd_record_header_s_to_string(&this->header,a_trg);
  bc_array_s_append(a_trg,a_count,a_data);
  bc_array_s_append_ptr(a_trg,"data:{...}");
}/*}}}*/
#endif

static inline crc16_s sd_record_s_compute_crc(const sd_record_s *this,unsigned a_rec_size)
{/*{{{*/
  const char *crc_begin = (const char *)&this->header.id;
  const char *crc_end = (const char *)this + a_rec_size;

  crc16_s crc = RECORD_CRC_INIT_VALUE;
  crc16_s_update(&crc,crc_end - crc_begin,crc_begin);

  return crc;
}/*}}}*/

static inline int sd_record_s_msync(const sd_record_s *this,unsigned a_rec_size)
{/*{{{*/

  // - page alligned pointer -
  void *alligned = (void *)this - ((uintptr_t)this % g_sd_page_size);

  // - ERROR -
  if (msync(alligned,(void *)this + a_rec_size - alligned,MS_SYNC))
  {
    throw_error(SYSTEM_MSYNC_ERROR);
  }

  return 0;
}/*}}}*/

#endif

