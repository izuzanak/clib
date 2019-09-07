
#ifndef __CL_TRACE_H
#define __CL_TRACE_H

@begin
include "cl_linux.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libtrace_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libtrace_cll_EXPORTS
#define libtrace_cll_EXPORT __declspec(dllexport)
#else
#define libtrace_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_TRACE_DUMMY_ERROR 1

// === constants and definitions ===============================================

// === definition of generated structures ======================================

// -- trace_record_header_s --
@begin
struct
<
ulli:id
ulli:time
usi:crc
>
trace_record_header_s;
@end

// -- trace_record_header_tree_s --
@begin
rb_tree<trace_record_header_s>
options ( safe fixed_buffer )
trace_record_header_tree_s;
@end

// === definition of structure trace_record_s ==================================

typedef struct trace_record_s trace_record_s;

struct trace_record_s
{
  trace_record_header_s header;
  char data[];
};

static inline void trace_record_s_init(trace_record_s *this);
static inline void trace_record_s_clear(trace_record_s *this);
static inline void trace_record_s_set(trace_record_s *this,const trace_record_header_s *a_header,
    unsigned a_size,const bc *a_data);
static inline void trace_record_s_flush_all(trace_record_s *this);
static inline void trace_record_s_swap(trace_record_s *this,trace_record_s *a_second);
static inline void trace_record_s_copy(trace_record_s *this,const trace_record_s *a_src,unsigned a_rec_size);
static inline int trace_record_s_compare(const trace_record_s *this,const trace_record_s *a_second,unsigned a_rec_size);
#if OPTION_TO_STRING == ENABLED
static inline void trace_record_s___to_string(const trace_record_s *this,bc_array_s *a_trg);
#define trace_record_s_to_string trace_record_s___to_string
static inline void trace_record_s_to_string_separator(const trace_record_s *this,bc_array_s *a_trg,unsigned a_count,const char *a_data);
#endif

// === definition of structure trace_record_queue_s ============================

typedef struct trace_record_queue_s trace_record_queue_s;
@begin
define trace_record_queue_s dynamic
@end

//typedef struct trace_record_queue_header_s
//{
//  unsigned used; //!< count of stored elements
//  unsigned begin; //!< position of first element
//  unsigned rec_size; //!< size of one record (with header)
//} trace_record_queue_header_s;

struct trace_record_queue_s
{
  unsigned size; //!< actual size of allocated space (element count)
  unsigned used; //!< count of stored elements
  unsigned begin; //!< position of first element
  unsigned rec_size; //!< size of one record (with header)
  char *data; //!< pointer to queue elements
};

static inline void trace_record_queue_s_init(trace_record_queue_s *this);
static inline void trace_record_queue_s_init_buffer(trace_record_queue_s *this,
    unsigned a_size,unsigned a_rec_size,trace_record_s *a_data);
static inline void trace_record_queue_s_clear(trace_record_queue_s *this);
static inline void trace_record_queue_s_set_buffer(trace_record_queue_s *this,
    unsigned a_size,unsigned a_rec_size,trace_record_s *a_data);
static inline void trace_record_queue_s_flush(trace_record_queue_s *this);
static inline void trace_record_queue_s_flush_all(trace_record_queue_s *this);
static inline void trace_record_queue_s_swap(trace_record_queue_s *this,trace_record_queue_s *a_second);
static inline trace_record_s *trace_record_queue_s_at(const trace_record_queue_s *this,unsigned a_idx);
static inline void trace_record_queue_s_insert(trace_record_queue_s *this,const trace_record_s *a_value);
static inline void trace_record_queue_s_insert_blank(trace_record_queue_s *this);
static inline trace_record_s *trace_record_queue_s_next(trace_record_queue_s *this);
static inline trace_record_s *trace_record_queue_s_first(const trace_record_queue_s *this);
static inline trace_record_s *trace_record_queue_s_last(const trace_record_queue_s *this);
EXPORT void trace_record_queue_s_copy(trace_record_queue_s *this,const trace_record_queue_s *a_src);
EXPORT int trace_record_queue_s_compare(const trace_record_queue_s *this,const trace_record_queue_s *a_second);
#if OPTION_TO_STRING == ENABLED
EXPORT void trace_record_queue_s___to_string(const trace_record_queue_s *this,bc_array_s *a_trg);
#define trace_record_queue_s_to_string trace_record_queue_s___to_string
EXPORT void trace_record_queue_s_to_string_separator(const trace_record_queue_s *this,bc_array_s *a_trg,unsigned a_count,const char *a_data);
#endif

// -- trace_s --
@begin
struct
<
trace_record_queue_s:header_queue
trace_record_queue_s:trace_queue
>
trace_s;
@end

WUR int trace_s_create(trace_s *this,
    void *header_data,ulli header_size,
    void *trace_data,ulli trace_size,unsigned a_rec_size);

// === inline methods of generated structures ==================================

// -- trace_record_header_s --
@begin
inlines trace_record_header_s
@end

// -- trace_record_header_tree_s --
@begin
inlines trace_record_header_tree_s
@end

static inline int trace_record_header_tree_s___compare_value(const trace_record_header_tree_s *this,const trace_record_header_s *a_first,const trace_record_header_s *a_second)
{/*{{{*/
  (void)this;

  return a_first->time < a_second->time ? -1 : a_first->time > a_second->time ? 1 : 0;
}/*}}}*/

// === inline methods of structure trace_record_s ==============================

static inline void trace_record_s_init(trace_record_s *this)
{/*{{{*/
}/*}}}*/

static inline void trace_record_s_clear(trace_record_s *this)
{/*{{{*/
}/*}}}*/

static inline void trace_record_s_set(trace_record_s *this,const trace_record_header_s *a_header,
  unsigned a_size,const bc *a_data)
{/*{{{*/
  this->header = *a_header;
  memcpy(this->data,a_data,a_size);
}/*}}}*/

static inline void trace_record_s_flush_all(trace_record_s *this)
{/*{{{*/
}/*}}}*/

static inline void trace_record_s_swap(trace_record_s *this,trace_record_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

static inline void trace_record_s_copy(trace_record_s *this,const trace_record_s *a_src,unsigned a_rec_size)
{/*{{{*/
  memcpy(this,a_src,a_rec_size);
}/*}}}*/

static inline int trace_record_s_compare(const trace_record_s *this,const trace_record_s *a_second,unsigned a_rec_size)
{/*{{{*/
  return memcmp(this,a_second,a_rec_size) == 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void trace_record_s___to_string(const trace_record_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"{header:");
  trace_record_header_s_to_string(&this->header,a_trg);
  bc_array_s_append_ptr(a_trg,",data:{...}");
  bc_array_s_push(a_trg,'}');
}/*}}}*/
#endif

#if OPTION_TO_STRING == ENABLED
static inline void trace_record_s_to_string_separator(const trace_record_s *this,bc_array_s *a_trg,unsigned a_count,const char *a_data)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"header:");
  trace_record_header_s_to_string(&this->header,a_trg);
  bc_array_s_append(a_trg,a_count,a_data);
  bc_array_s_append_ptr(a_trg,"data:{...}");
}/*}}}*/
#endif

// === inline methods of structure trace_record_queue_s ========================

static inline void trace_record_queue_s_init(trace_record_queue_s *this)
{/*{{{*/
  this->size = 0;
  this->used = 0;
  this->begin = 0;
  this->rec_size = 0;
  this->data = NULL;
}/*}}}*/

static inline void trace_record_queue_s_init_buffer(trace_record_queue_s *this,
    unsigned a_size,unsigned a_rec_size,trace_record_s *a_data)
{/*{{{*/
  trace_record_queue_s_init(this);
  trace_record_queue_s_set_buffer(this,a_size,a_rec_size,a_data);
}/*}}}*/

static inline void trace_record_queue_s_clear(trace_record_queue_s *this)
{/*{{{*/
  this->used = 0;
  this->begin = 0;
}/*}}}*/

static inline void trace_record_queue_s_set_buffer(trace_record_queue_s *this,
    unsigned a_size,unsigned a_rec_size,trace_record_s *a_data)
{/*{{{*/
  debug_assert(a_size != 0 && a_data != NULL);

  trace_record_queue_s_clear(this);

  this->size = a_size;
  this->rec_size = a_rec_size;
  this->data = (char *)a_data;
}/*}}}*/

static inline void trace_record_queue_s_flush(trace_record_queue_s *this)
{/*{{{*/
}/*}}}*/

static inline void trace_record_queue_s_flush_all(trace_record_queue_s *this)
{/*{{{*/
}/*}}}*/

static inline void trace_record_queue_s_swap(trace_record_queue_s *this,trace_record_queue_s *a_second)
{/*{{{*/
  trace_record_queue_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline trace_record_s *trace_record_queue_s_at(const trace_record_queue_s *this,unsigned a_idx)
{/*{{{*/
  debug_assert(a_idx < this->used);

  unsigned real_idx = this->begin + a_idx;
  if (real_idx >= this->size)
  {
    return (trace_record_s *)(this->data + (real_idx - this->size)*this->rec_size);
  }

  return (trace_record_s *)(this->data + real_idx*this->rec_size);
}/*}}}*/

static inline void trace_record_queue_s_insert(trace_record_queue_s *this,const trace_record_s *a_value)
{/*{{{*/
  debug_assert(this->used < this->size);
  unsigned inserted_idx = this->begin + this->used++;

  if (inserted_idx >= this->size)
  {
    inserted_idx -= this->size;
  }

  trace_record_s_copy((trace_record_s *)(this->data + inserted_idx*this->rec_size),a_value,this->rec_size);
}/*}}}*/

static inline void trace_record_queue_s_insert_blank(trace_record_queue_s *this)
{/*{{{*/
  debug_assert(this->used < this->size);
  ++this->used;
}/*}}}*/

static inline trace_record_s *trace_record_queue_s_next(trace_record_queue_s *this)
{/*{{{*/
  debug_assert(this->used > 0);

  unsigned ret_idx = this->begin;

  if (++this->begin >= this->size)
  {
    this->begin = 0;
  }

  this->used--;

  return (trace_record_s *)(this->data + ret_idx*this->rec_size);
}/*}}}*/

static inline trace_record_s *trace_record_queue_s_first(const trace_record_queue_s *this)
{/*{{{*/
  debug_assert(this->used > 0);

  return (trace_record_s *)(this->data + this->begin*this->rec_size);
}/*}}}*/

static inline trace_record_s *trace_record_queue_s_last(const trace_record_queue_s *this)
{/*{{{*/
  debug_assert(this->used > 0);

  unsigned last_idx = this->begin + (this->used - 1);
  if (last_idx >= this->size)
  {
    return (trace_record_s *)(this->data + (last_idx - this->size)*this->rec_size);
  }

  return (trace_record_s *)(this->data + last_idx*this->rec_size);
}/*}}}*/

// -- trace_s --
@begin
inlines trace_s
@end

#endif

