
#ifndef __SD_TRACE_H
#define __SD_TRACE_H

@begin
include "sd_record.h"
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
#define ERROR_SD_TRACE_WRITE_HEADER_ERROR 4
#define ERROR_SD_TRACE_WRITE_TIMESTAMP_QUEUE_ERROR 5
#define ERROR_SD_TRACE_INVALID_RECORD_ID 6
#define ERROR_SD_TRACE_INVALID_RECORD_CRC 7
#define ERROR_SD_TRACE_MMAP_CREATE_ERROR 8

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
lli:timestamp_div

lli:header_last_id
lli:trace_last_id

sd_trace_queue_s:header_queue
sd_trace_queue_s:trace_queue
sd_trace_queue_s:ts_trace_queue

sd_record_timestamp_array_s:timestamp_buffer
sd_record_timestamp_queue_s:timestamp_queue

ui:first_timestamp_tree_idx
sd_record_timestamp_tree_s:timestamp_tree
>
sd_trace_s;
@end

WUR int sd_trace_s_create(sd_trace_s *this,
    void *header_data,ulli header_size,
    void *trace_data,ulli sd_trace_size,
    void *ts_trace_data,ulli ts_trace_size,
    unsigned a_data_size,
    lli a_timestamp_div);
void sd_trace_s_update_timestamp_structures(sd_trace_s *this,sd_record_timestamp_s *a_timestamp);
WUR libtrace_sdl_EXPORT int sd_trace_s_write_header(sd_trace_s *this,time_s a_time);
WUR libtrace_sdl_EXPORT int sd_trace_s_write_timestamp_queue(sd_trace_s *this,time_s a_time);

static inline lli sd_trace_s_head(sd_trace_s *this);
static inline lli sd_trace_s_tail(sd_trace_s *this);
static inline lli sd_trace_s_lee_time(sd_trace_s *this,time_s a_time);
static inline lli sd_trace_s_gre_time(sd_trace_s *this,time_s a_time);
WUR libtrace_sdl_EXPORT int sd_trace_s_write_record(sd_trace_s *this,
    time_s a_time,unsigned a_size,const char *a_data);
WUR libtrace_sdl_EXPORT int sd_trace_s_read_record(sd_trace_s *this,
    lli a_id,time_s *a_time,bc_array_s *a_trg);

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

static inline lli sd_trace_s_lee_time(sd_trace_s *this,time_s a_time)
{/*{{{*/
  sd_record_timestamp_s search_timestamp = {-1,a_time};

  unsigned timestamp_idx = sd_record_timestamp_tree_s_get_lee_idx(&this->timestamp_tree,&search_timestamp);
  if (timestamp_idx == c_idx_not_exist)
  {
    return -1;
  }

  return sd_record_timestamp_tree_s_at(&this->timestamp_tree,timestamp_idx)->id;
}/*}}}*/

static inline lli sd_trace_s_gre_time(sd_trace_s *this,time_s a_time)
{/*{{{*/
  sd_record_timestamp_s search_timestamp = {-1,a_time};

  unsigned timestamp_idx = sd_record_timestamp_tree_s_get_gre_idx(&this->timestamp_tree,&search_timestamp);
  if (timestamp_idx == c_idx_not_exist)
  {
    return -1;
  }

  return sd_record_timestamp_tree_s_at(&this->timestamp_tree,timestamp_idx)->id;
}/*}}}*/

#endif

