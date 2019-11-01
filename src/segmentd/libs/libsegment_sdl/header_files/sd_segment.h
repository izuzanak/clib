
#ifndef __SD_SEGMENT_H
#define __SD_SEGMENT_H

@begin
include "sd_segfile.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libsegment_sdl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libsegment_sdl_EXPORTS
#define libsegment_sdl_EXPORT __declspec(dllexport)
#else
#define libsegment_sdl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_SD_SEGMENT_DESCR_FILE_CREATE_ERROR 1
#define ERROR_SD_SEGMENT_DESCR_INVALID_SEGMENT_TYPE 2

// === constants and definitions ===============================================

// === definition of structure sd_segment_handle_s =============================

typedef int (*sd_segment_clear_cb_t)(void *a_object);
typedef int (*sd_segment_write_record_cb_t)(void *a_object,time_s a_time,unsigned a_size,const char *a_data);
typedef int (*sd_segment_get_record_cb_t)(void *a_object,time_s *a_time,bc_array_s *a_record);

typedef struct sd_segment_handle_s
{
  void *segment;
  sd_segment_clear_cb_t sd_segment_clear_cb;
  sd_segment_write_record_cb_t sd_segment_write_record_cb;
  sd_segment_get_record_cb_t sd_segment_get_record_cb;
}
sd_segment_handle_s;

@begin
define sd_segment_handle_s dynamic
@end

static inline void sd_segment_handle_s_init(sd_segment_handle_s *this);
static inline void sd_segment_handle_s_clear(sd_segment_handle_s *this);
static inline void sd_segment_handle_s_flush_all(sd_segment_handle_s *this);
static inline void sd_segment_handle_s_swap(sd_segment_handle_s *this,sd_segment_handle_s *a_second);
static inline void sd_segment_handle_s_copy(const sd_segment_handle_s *this,const sd_segment_handle_s *a_src);
static inline int sd_segment_handle_s_compare(const sd_segment_handle_s *this,const sd_segment_handle_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void sd_segment_handle_s_to_string(const sd_segment_handle_s *this,bc_array_s *a_trg);
#endif

static inline void sd_segment_handle_s_create(sd_segment_handle_s *this,
    void *a_segment,
    sd_segment_clear_cb_t a_sd_segment_clear_cb,
    sd_segment_write_record_cb_t a_sd_segment_write_record_cb,
    sd_segment_get_record_cb_t a_sd_segment_get_record_cb);
static inline int sd_segment_handle_s_write_record(sd_segment_handle_s *this,
    time_s a_time,unsigned a_size,const char *a_data);
static inline int sd_segment_handle_s_get_record(sd_segment_handle_s *this,
    time_s *a_time,bc_array_s *a_record);

// === definition of generated structures ======================================

// -- sd_segment_descr_s --
@begin
struct
<
sd_conf_segment_s:config
sd_segment_handle_s:handle
ui_array_s:channel_idxs
>
sd_segment_descr_s;
@end

WUR libsegment_sdl_EXPORT int sd_segment_descr_s_create(sd_segment_descr_s *this,
    sd_conf_segment_s *a_config);

// === inline methods of structure sd_segment_handle_s =========================

static inline void sd_segment_handle_s_init(sd_segment_handle_s *this)
{/*{{{*/
  this->segment = NULL;
  this->sd_segment_clear_cb = NULL;
  this->sd_segment_write_record_cb = NULL;
  this->sd_segment_get_record_cb = NULL;
}/*}}}*/

static inline void sd_segment_handle_s_clear(sd_segment_handle_s *this)
{/*{{{*/
  if (this->segment != NULL)
  {
    this->sd_segment_clear_cb(this->segment);
    cfree(this->segment);
  }

  sd_segment_handle_s_init(this);
}/*}}}*/

static inline void sd_segment_handle_s_flush_all(sd_segment_handle_s *this)
{/*{{{*/
}/*}}}*/

static inline void sd_segment_handle_s_swap(sd_segment_handle_s *this,sd_segment_handle_s *a_second)
{/*{{{*/
  sd_segment_handle_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void sd_segment_handle_s_copy(const sd_segment_handle_s *this,const sd_segment_handle_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int sd_segment_handle_s_compare(const sd_segment_handle_s *this,const sd_segment_handle_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void sd_segment_handle_s_to_string(const sd_segment_handle_s *this,bc_array_s *a_trg)
{/*{{{*/
  (void)this;

  bc_array_s_append_ptr(a_trg,"sd_segment_handle_s{}");
}/*}}}*/
#endif

static inline void sd_segment_handle_s_create(sd_segment_handle_s *this,
    void *a_segment,
    sd_segment_clear_cb_t a_sd_segment_clear_cb,
    sd_segment_write_record_cb_t a_sd_segment_write_record_cb,
    sd_segment_get_record_cb_t a_sd_segment_get_record_cb)
{/*{{{*/
  sd_segment_handle_s_clear(this);

  this->segment = a_segment;
  this->sd_segment_clear_cb = a_sd_segment_clear_cb;
  this->sd_segment_write_record_cb = a_sd_segment_write_record_cb;
  this->sd_segment_get_record_cb = a_sd_segment_get_record_cb;
}/*}}}*/

static inline int sd_segment_handle_s_write_record(sd_segment_handle_s *this,
    time_s a_time,unsigned a_size,const char *a_data)
{/*{{{*/
  return this->sd_segment_write_record_cb(this->segment,a_time,a_size,a_data);
}/*}}}*/

static inline int sd_segment_handle_s_get_record(sd_segment_handle_s *this,
    time_s *a_time,bc_array_s *a_record)
{/*{{{*/
  return this->sd_segment_get_record_cb(this->segment,a_time,a_record);
}/*}}}*/

// === inline methods of generated structures ==================================

// -- sd_segment_descr_s --
@begin
inlines sd_segment_descr_s
@end

#endif

