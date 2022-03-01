
#ifndef SD_LIB_TRACE_H
#define SD_LIB_TRACE_H

@begin
include "sd_segment.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libtrace_sdl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libtrace_sdl_EXPORTS
#define libtrace_sdl_EXPORT __declspec(dllexport)
#else
#define libtrace_sdl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_SD_TRACE_RECORD_MSYNC_ERROR 1

#define ERROR_SD_TRACE_HEADER_QUEUE_SIZE_ERROR 1
#define ERROR_SD_TRACE_TRACE_QUEUE_SIZE_ERROR 2
#define ERROR_SD_TRACE_TIMESTAMP_TRACE_QUEUE_SIZE_ERROR 3
#define ERROR_SD_TRACE_READ_TIMESTAMP_STRUCTURES_ERROR 4
#define ERROR_SD_TRACE_WRITE_HEADER_ERROR 5
#define ERROR_SD_TRACE_WRITE_TIMESTAMP_QUEUE_ERROR 6
#define ERROR_SD_TRACE_INVALID_RECORD_ID 7
#define ERROR_SD_TRACE_INVALID_RECORD_CRC 8
#define ERROR_SD_TRACE_MMAP_CREATE_ERROR 9
#define ERROR_SD_TRACE_SEGMENT_INVALID_SIZE 10
#define ERROR_SD_TRACE_SEGMENT_CREATE_ERROR 11
#define ERROR_SD_TRACE_SEGMENT_WRITE_ERROR 12
#define ERROR_SD_TRACE_INVALID_TRACE_DATA_TYPE 13

#define ERROR_SD_TRACE_DESCR_FILE_OPEN_ERROR 1
#define ERROR_SD_TRACE_DESCR_MMAP_CREATE_ERROR 2
#define ERROR_SD_TRACE_DESCR_FILES_MMAP_ERROR 3
#define ERROR_SD_TRACE_DESCR_TRACE_CREATE_ERROR 4

// === constants and definitions ===============================================

// === definition of generated structures ======================================

// -- sd_record_timestamp_array_s --
@begin
array<sd_record_timestamp_s> sd_record_timestamp_array_s;
@end

// -- sd_record_timestamp_queue_s --
@begin
queue<sd_record_timestamp_s>
options ( fixed_buffer )
sd_record_timestamp_queue_s;
@end

// -- sd_record_timestamp_tree_s --
@begin
safe_rb_tree<sd_record_timestamp_s> sd_record_timestamp_tree_s;
@end

// === definition of structure sd_trace_queue_s ================================

typedef struct sd_trace_queue_s sd_trace_queue_s;
@begin
define sd_trace_queue_s dynamic
@end

struct sd_trace_queue_s
{
  unsigned size; //!< actual size of allocated space (element count)
  unsigned used; //!< count of stored elements
  unsigned begin; //!< position of first element
  unsigned rec_size; //!< size of one record (with header)
  char *data; //!< pointer to queue elements
};

static inline void sd_trace_queue_s_init(sd_trace_queue_s *this);
static inline void sd_trace_queue_s_init_buffer(sd_trace_queue_s *this,
    unsigned a_size,unsigned a_rec_size,sd_record_s *a_data);
static inline void sd_trace_queue_s_clear(sd_trace_queue_s *this);
static inline void sd_trace_queue_s_set_buffer(sd_trace_queue_s *this,
    unsigned a_size,unsigned a_rec_size,sd_record_s *a_data);
static inline void sd_trace_queue_s_flush(sd_trace_queue_s *this);
static inline void sd_trace_queue_s_flush_all(sd_trace_queue_s *this);
static inline void sd_trace_queue_s_swap(sd_trace_queue_s *this,sd_trace_queue_s *a_second);
static inline sd_record_s *sd_trace_queue_s_at(const sd_trace_queue_s *this,unsigned a_idx);
static inline void sd_trace_queue_s_insert(sd_trace_queue_s *this,const sd_record_s *a_value);
static inline void sd_trace_queue_s_insert_blank(sd_trace_queue_s *this);
static inline sd_record_s *sd_trace_queue_s_next(sd_trace_queue_s *this);
static inline sd_record_s *sd_trace_queue_s_first(const sd_trace_queue_s *this);
static inline sd_record_s *sd_trace_queue_s_last(const sd_trace_queue_s *this);
libtrace_sdl_EXPORT void sd_trace_queue_s_copy(sd_trace_queue_s *this,
    const sd_trace_queue_s *a_src);
libtrace_sdl_EXPORT int sd_trace_queue_s_compare(const sd_trace_queue_s *this,
    const sd_trace_queue_s *a_second);
#if OPTION_TO_STRING == ENABLED
libtrace_sdl_EXPORT void sd_trace_queue_s___to_string(const sd_trace_queue_s *this,
    bc_array_s *a_trg);
#define sd_trace_queue_s_to_string sd_trace_queue_s___to_string
libtrace_sdl_EXPORT void sd_trace_queue_s_to_string_separator(const sd_trace_queue_s *this,
    bc_array_s *a_trg,unsigned a_count,const char *a_data);
#endif

libtrace_sdl_EXPORT unsigned sd_trace_queue_s_scan_for_last(const sd_trace_queue_s *this);

// -- sd_trace_queue_header_s --
@begin
struct
<
ui:used
ui:begin
>
sd_trace_queue_header_s;
@end

// -- sd_trace_mmap_s --
@begin
struct
<
mmap_s:mmap
pointer:address
>
sd_trace_mmap_s;
@end

WUR static inline int sd_trace_mmap_s_create(sd_trace_mmap_s *this,
  void *a_addr,size_t a_length,int a_prot,int a_flags,int a_fd,off_t a_offset);

// -- sd_trace_s --
@begin
struct
<
ui:data_size

ui:header_type
lli:header_last_id
sd_trace_queue_s:header_queue
sd_segment_descr_s:header_segment

lli:trace_last_id
sd_trace_queue_s:trace_queue

ui:ts_type
sd_trace_queue_s:ts_trace_queue
sd_segment_descr_s:ts_segment

lli:timestamp_div
bi:timestamp_nomem

sd_record_timestamp_array_s:timestamp_buffer
sd_record_timestamp_queue_s:timestamp_queue

ui:first_timestamp_tree_idx
sd_record_timestamp_tree_s:timestamp_tree
>
sd_trace_s;
@end

WUR int sd_trace_s_create(sd_trace_s *this,
    unsigned a_header_type,
    void *header_data,ulli header_size,
    sd_conf_segment_s *a_header_segment,
    void *trace_data,ulli sd_trace_size,
    unsigned a_ts_type,
    void *ts_trace_data,ulli ts_trace_size,
    sd_conf_segment_s *a_ts_segment,
    unsigned a_data_size,
    lli a_timestamp_div,
    int a_timestamp_nomem);
WUR libtrace_sdl_EXPORT int sd_trace_s_read_timestamp_structures(sd_trace_s *this);
libtrace_sdl_EXPORT void sd_trace_s_clear_timestamp_structures(sd_trace_s *this);
libtrace_sdl_EXPORT void sd_trace_s_update_timestamp_structures(sd_trace_s *this,sd_record_timestamp_s *a_timestamp);
WUR libtrace_sdl_EXPORT int sd_trace_s_write_header(sd_trace_s *this,time_s a_time);
WUR libtrace_sdl_EXPORT int sd_trace_s_write_timestamp_queue(sd_trace_s *this,time_s a_time);

static inline lli sd_trace_s_head(sd_trace_s *this);
static inline lli sd_trace_s_tail(sd_trace_s *this);

WUR static inline int sd_trace_s_init_timestamp(sd_trace_s *this);
static inline void sd_trace_s_clear_timestamp(sd_trace_s *this);
libtrace_sdl_EXPORT lli sd_trace_s_lee_time(sd_trace_s *this,time_s a_time);
libtrace_sdl_EXPORT lli sd_trace_s_gre_time(sd_trace_s *this,time_s a_time);

WUR libtrace_sdl_EXPORT int sd_trace_s_write_record(sd_trace_s *this,
    time_s a_time,unsigned a_size,const char *a_data);
WUR libtrace_sdl_EXPORT int sd_trace_s_read_record(sd_trace_s *this,
    lli a_id,time_s *a_time,bc_array_s *a_trg);

// -- sd_trace_descr_s --
@begin
struct
<
sd_conf_trace_s:config
sd_trace_mmap_s:mmap_header
sd_trace_mmap_s:mmap_trace
sd_trace_mmap_s:mmap_timestamp
sd_trace_s:trace
ui_array_s:channel_idxs
>
sd_trace_descr_s;
@end

WUR libtrace_sdl_EXPORT int sd_trace_descr_s_create(sd_trace_descr_s *this,
    sd_conf_trace_s *a_config);
WUR static inline int sd_trace_descr_s_mmap_file(sd_trace_mmap_s *a_trace_mmap,sd_conf_mmap_s *a_trace_data);
void sd_trace_descr_s_read_to_message(sd_trace_descr_s *this,lli a_record_id,bc_array_s *a_trg);
void sd_trace_descr_s_read_size_to_message(sd_trace_descr_s *this,lli a_record_id,bc_array_s *a_trg);

// === inline methods of generated structures ==================================

// -- sd_record_timestamp_array_s --
@begin
inlines sd_record_timestamp_array_s
@end

// -- sd_record_timestamp_queue_s --
@begin
inlines sd_record_timestamp_queue_s
@end

// -- sd_record_timestamp_tree_s --
@begin
inlines sd_record_timestamp_tree_s
@end

static inline int sd_record_timestamp_tree_s___compare_value(const sd_record_timestamp_tree_s *this,const sd_record_timestamp_s *a_first,const sd_record_timestamp_s *a_second)
{/*{{{*/
  (void)this;

  return a_first->time < a_second->time ? -1 : a_first->time > a_second->time ? 1 : 0;
}/*}}}*/

// === inline methods of structure sd_trace_queue_s ============================

static inline void sd_trace_queue_s_init(sd_trace_queue_s *this)
{/*{{{*/
  this->size = 0;
  this->used = 0;
  this->begin = 0;
  this->rec_size = 0;
  this->data = NULL;
}/*}}}*/

static inline void sd_trace_queue_s_init_buffer(sd_trace_queue_s *this,
    unsigned a_size,unsigned a_rec_size,sd_record_s *a_data)
{/*{{{*/
  sd_trace_queue_s_init(this);
  sd_trace_queue_s_set_buffer(this,a_size,a_rec_size,a_data);
}/*}}}*/

static inline void sd_trace_queue_s_clear(sd_trace_queue_s *this)
{/*{{{*/
  this->used = 0;
  this->begin = 0;
}/*}}}*/

static inline void sd_trace_queue_s_set_buffer(sd_trace_queue_s *this,
    unsigned a_size,unsigned a_rec_size,sd_record_s *a_data)
{/*{{{*/
  debug_assert(a_size != 0 && a_data != NULL);

  sd_trace_queue_s_clear(this);

  this->size = a_size;
  this->rec_size = a_rec_size;
  this->data = (char *)a_data;
}/*}}}*/

static inline void sd_trace_queue_s_flush(sd_trace_queue_s *this)
{/*{{{*/
}/*}}}*/

static inline void sd_trace_queue_s_flush_all(sd_trace_queue_s *this)
{/*{{{*/
}/*}}}*/

static inline void sd_trace_queue_s_swap(sd_trace_queue_s *this,sd_trace_queue_s *a_second)
{/*{{{*/
  sd_trace_queue_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline sd_record_s *sd_trace_queue_s_at(const sd_trace_queue_s *this,unsigned a_idx)
{/*{{{*/
  debug_assert(a_idx < this->used);

  unsigned real_idx = this->begin + a_idx;
  if (real_idx >= this->size)
  {
    return (sd_record_s *)(this->data + (real_idx - this->size)*this->rec_size);
  }

  return (sd_record_s *)(this->data + real_idx*this->rec_size);
}/*}}}*/

static inline void sd_trace_queue_s_insert(sd_trace_queue_s *this,const sd_record_s *a_value)
{/*{{{*/
  debug_assert(this->used < this->size);
  unsigned inserted_idx = this->begin + this->used++;

  if (inserted_idx >= this->size)
  {
    inserted_idx -= this->size;
  }

  sd_record_s_copy((sd_record_s *)(this->data + inserted_idx*this->rec_size),a_value,this->rec_size);
}/*}}}*/

static inline void sd_trace_queue_s_insert_blank(sd_trace_queue_s *this)
{/*{{{*/
  debug_assert(this->used < this->size);
  ++this->used;
}/*}}}*/

static inline sd_record_s *sd_trace_queue_s_next(sd_trace_queue_s *this)
{/*{{{*/
  debug_assert(this->used > 0);

  unsigned ret_idx = this->begin;

  if (++this->begin >= this->size)
  {
    this->begin = 0;
  }

  this->used--;

  return (sd_record_s *)(this->data + ret_idx*this->rec_size);
}/*}}}*/

static inline sd_record_s *sd_trace_queue_s_first(const sd_trace_queue_s *this)
{/*{{{*/
  debug_assert(this->used > 0);

  return (sd_record_s *)(this->data + this->begin*this->rec_size);
}/*}}}*/

static inline sd_record_s *sd_trace_queue_s_last(const sd_trace_queue_s *this)
{/*{{{*/
  debug_assert(this->used > 0);

  unsigned last_idx = this->begin + (this->used - 1);
  if (last_idx >= this->size)
  {
    return (sd_record_s *)(this->data + (last_idx - this->size)*this->rec_size);
  }

  return (sd_record_s *)(this->data + last_idx*this->rec_size);
}/*}}}*/

// -- sd_trace_queue_header_s --
@begin
inlines sd_trace_queue_header_s
@end

// -- sd_trace_mmap_s --
@begin
inlines sd_trace_mmap_s
@end

WUR static inline int sd_trace_mmap_s_create(sd_trace_mmap_s *this,
  void *a_addr,size_t a_length,int a_prot,int a_flags,int a_fd,off_t a_offset)
{/*{{{*/
  off_t off_diff = a_offset % g_sd_page_size;
  off_t off_aligned = a_offset - off_diff;

  if (mmap_s_create(&this->mmap,a_addr,a_length + off_diff,
        a_prot,a_flags,a_fd,off_aligned))
  {
    throw_error(SD_TRACE_MMAP_CREATE_ERROR);
  }

  this->address = this->mmap.address + off_diff;

  return 0;
}/*}}}*/

// -- sd_trace_s --
@begin
inlines sd_trace_s
@end

static inline lli sd_trace_s_head(sd_trace_s *this)
{/*{{{*/
  if (this->trace_queue.used != 0)
  {
    return this->trace_last_id;
  }

  return -1;
}/*}}}*/

static inline lli sd_trace_s_tail(sd_trace_s *this)
{/*{{{*/
  if (this->trace_queue.used != 0)
  {
    return this->trace_last_id - this->trace_queue.used + 1;
  }

  return -1;
}/*}}}*/

static inline int sd_trace_s_init_timestamp(sd_trace_s *this)
{/*{{{*/
  if (this->timestamp_nomem)
  {
    if (sd_trace_s_read_timestamp_structures(this))
    {
      throw_error(SD_TRACE_READ_TIMESTAMP_STRUCTURES_ERROR);
    }
  }

  return 0;
}/*}}}*/

static inline void sd_trace_s_clear_timestamp(sd_trace_s *this)
{/*{{{*/
  if (this->timestamp_nomem)
  {
    sd_trace_s_clear_timestamp_structures(this);
  }
}/*}}}*/

// -- sd_trace_descr_s --
@begin
inlines sd_trace_descr_s
@end

static inline int sd_trace_descr_s_mmap_file(sd_trace_mmap_s *a_trace_mmap,sd_conf_mmap_s *a_trace_data)
{/*{{{*/
  CONT_INIT_CLEAR(fd_s,fd);
  fd = open(a_trace_data->path.data,O_RDWR,0);

  if (fd == -1)
  {
    throw_error(SD_TRACE_DESCR_FILE_OPEN_ERROR);
  }

  if (sd_trace_mmap_s_create(a_trace_mmap,NULL,a_trace_data->size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,a_trace_data->offset))
  {
    throw_error(SD_TRACE_DESCR_MMAP_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

#endif

