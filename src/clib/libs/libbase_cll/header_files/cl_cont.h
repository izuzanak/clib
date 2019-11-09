
#ifndef __CL_CONT_H
#define __CL_CONT_H

@begin
include "cl_basic.h"
@end

#include <ctype.h>

#if OPTION_MEMCHECK == ENABLED
#define cmalloc(SIZE) check_malloc(SIZE)
#define crealloc(LOCATION,SIZE) check_realloc(LOCATION,SIZE)
#define cfree(LOCATION) check_free(LOCATION)
#else
#define cmalloc(SIZE) malloc(SIZE)
#define crealloc(LOCATION,SIZE) realloc(LOCATION,SIZE)
#define cfree(LOCATION) free(LOCATION)
#endif

// === definition of generated structures ======================================

// -- bc_array_s --
@begin
array<bc> bc_array_s;
@end

static inline void bc_array_s_append(bc_array_s *this,unsigned a_count,const char *a_data);
static inline void bc_array_s_append_ptr(bc_array_s *this,const char *a_data);
static inline  void bc_array_s_append_format(bc_array_s *this,const char *a_format,...);
libbase_cll_EXPORT void bc_array_s_append_format_ap(bc_array_s *this,const char *a_format,va_list a_ap);
static inline void bc_array_s_tail(bc_array_s *this,unsigned a_count);

static inline void bc_array_s_append_be_usi(bc_array_s *this,usi a_value);
static inline void bc_array_s_append_be_ui(bc_array_s *this,ui a_value);
static inline void bc_array_s_append_be_lli(bc_array_s *this,lli a_value);
static inline void bc_array_s_append_be_ulli(bc_array_s *this,ulli a_value);

// -- json_nice_s --
@begin
struct
<
bc_array_s:tabulator
bc_array_s:indent_buffer
ui:indent_size
pointer:user
>
json_nice_s;
@end

static inline void json_nice_s_create(json_nice_s *this,const char *a_tabulator,const char *a_indentation,void *a_user);
static inline void json_nice_s_push_indent(json_nice_s *this,bc_array_s *a_trg);
static inline void json_nice_s_pop_indent(json_nice_s *this,bc_array_s *a_trg);
static inline void json_nice_s_indent(const json_nice_s *this,bc_array_s *a_trg);

// -- bc_block_s --
@begin
struct
<
bc_pointer:data
ui:size
>
bc_block_s;
@end

// -- ui_array_s --
@begin
array<ui>
options ( to_json to_json_nice )
ui_array_s;
@end

#if OPTION_MEMCHECK == ENABLED
#define check_malloc(SIZE) malloc(SIZE)
#define check_realloc(LOCATION,SIZE) realloc(LOCATION,SIZE)
#define check_free(LOCATION) free(LOCATION)

// -- mc_block_s --
@begin
struct
<
pointer:location
ui:size
>
mc_block_s;
@end

// -- mc_block_tree_s --
@begin
safe_rb_tree<mc_block_s> mc_block_tree_s;
@end

// -- memcheck_s --
@begin
struct
<
mc_block_tree_s:mc_block_set
ulli:alloc_size
ulli:total_alloc_size
ulli:max_alloc_size
>
additions
{
libbase_cll_EXPORT void memcheck_init();
libbase_cll_EXPORT void memcheck_release();
libbase_cll_EXPORT void memcheck_release_assert();
static inline pointer memcheck_s_get_block(memcheck_s *this,unsigned a_size);
static inline pointer memcheck_s_reget_block(memcheck_s *this,pointer a_location,unsigned a_size);
static inline void memcheck_s_release_block(memcheck_s *this,pointer a_location);
void memcheck_s_print(memcheck_s *this);
}
memcheck_s;
@end

libbase_cll_EXPORT extern memcheck_s *g_memcheck;

#undef check_malloc
#undef check_realloc
#undef check_free

static inline pointer check_malloc(unsigned a_size);
static inline pointer check_realloc(pointer a_location,unsigned a_size);
static inline void check_free(pointer a_location);
#else
#define memcheck_init()
#define memcheck_release()
#define memcheck_release_assert()
#endif

// === definition of to_string methods for basic data types ====================

#define BASIC_TYPE_TO_STRING_DEFINE(TYPE) \
static inline void TYPE ## _to_string(const TYPE *this,bc_array_s *a_trg);

BASIC_TYPE_TO_STRING_DEFINE(bc);
BASIC_TYPE_TO_STRING_DEFINE(uc);
BASIC_TYPE_TO_STRING_DEFINE(si);
BASIC_TYPE_TO_STRING_DEFINE(usi);
BASIC_TYPE_TO_STRING_DEFINE(bi);
BASIC_TYPE_TO_STRING_DEFINE(ui);
BASIC_TYPE_TO_STRING_DEFINE(lli);
BASIC_TYPE_TO_STRING_DEFINE(ulli);
BASIC_TYPE_TO_STRING_DEFINE(bf);
BASIC_TYPE_TO_STRING_DEFINE(bd);
BASIC_TYPE_TO_STRING_DEFINE(pointer);
BASIC_TYPE_TO_STRING_DEFINE(bc_pointer);

// === definition of to_json methods for basic data types ======================

#if OPTION_TO_JSON == ENABLED

#define   si_to_json   si_to_string
#define  usi_to_json  usi_to_string
#define   bi_to_json   bi_to_string
#define   ui_to_json   ui_to_string
#define  lli_to_json  lli_to_string
#define ulli_to_json ulli_to_string
#define   bf_to_json   bf_to_string
#define   bd_to_json   bd_to_string

#define   si_to_json_nice(VALUE,NICE,TARGET)   si_to_json(VALUE,TARGET)
#define  usi_to_json_nice(VALUE,NICE,TARGET)  usi_to_json(VALUE,TARGET)
#define   bi_to_json_nice(VALUE,NICE,TARGET)   bi_to_json(VALUE,TARGET)
#define   ui_to_json_nice(VALUE,NICE,TARGET)   ui_to_json(VALUE,TARGET)
#define  lli_to_json_nice(VALUE,NICE,TARGET)  lli_to_json(VALUE,TARGET)
#define ulli_to_json_nice(VALUE,NICE,TARGET) ulli_to_json(VALUE,TARGET)
#define   bf_to_json_nice(VALUE,NICE,TARGET)   bf_to_json(VALUE,TARGET)
#define   bd_to_json_nice(VALUE,NICE,TARGET)   bd_to_json(VALUE,TARGET)

#endif

// === inline methods of generated structures ==================================

// -- bc_array_s --
@begin
inlines bc_array_s
@end

static inline void bc_array_s_append(bc_array_s *this,unsigned a_count,const char *a_data)
{/*{{{*/
  if (a_count != 0)
  {
    unsigned old_used = this->used;
    bc_array_s_push_blanks(this,a_count);
    memcpy(this->data + old_used,a_data,a_count);
  }
}/*}}}*/

static inline void bc_array_s_append_ptr(bc_array_s *this,const char *a_data)
{/*{{{*/
  bc_array_s_append(this,strlen(a_data),a_data);
}/*}}}*/

static inline void bc_array_s_append_format(bc_array_s *this,const char *a_format,...)
{/*{{{*/
  va_list ap;
  va_start(ap,a_format);
  bc_array_s_append_format_ap(this,a_format,ap);
  va_end(ap);
}/*}}}*/

static inline void bc_array_s_tail(bc_array_s *this,unsigned a_count)
{/*{{{*/
  debug_assert(a_count <= this->used);

  if (a_count != 0)
  {
    unsigned begin = this->used - a_count;

    if (begin < a_count)
    {
      memmove(this->data,this->data + begin,a_count);
    }
    else
    {
      memcpy(this->data,this->data + begin,a_count);
    }
  }

  this->used = a_count;
}/*}}}*/

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

// -- json_nice_s --
@begin
inlines json_nice_s
@end

static inline void json_nice_s_create(json_nice_s *this,const char *a_tabulator,const char *a_indentation,void *a_user)
{/*{{{*/
  json_nice_s_clear(this);

  bc_array_s_set(&this->tabulator,strlen(a_tabulator),a_tabulator);
  bc_array_s_push(&this->indent_buffer,'\n');
  bc_array_s_append_ptr(&this->indent_buffer,a_indentation);
  this->indent_size = this->indent_buffer.used;
  this->user = a_user;
}/*}}}*/

static inline void json_nice_s_push_indent(json_nice_s *this,bc_array_s *a_trg)
{/*{{{*/
  if ((this->indent_size += this->tabulator.used) > this->indent_buffer.used)
  {
    bc_array_s_append(&this->indent_buffer,this->tabulator.used,this->tabulator.data);
  }

  bc_array_s_append(a_trg,this->indent_size,this->indent_buffer.data);
}/*}}}*/

static inline void json_nice_s_pop_indent(json_nice_s *this,bc_array_s *a_trg)
{/*{{{*/
  this->indent_size -= this->tabulator.used;
  bc_array_s_append(a_trg,this->indent_size,this->indent_buffer.data);
}/*}}}*/

static inline void json_nice_s_indent(const json_nice_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append(a_trg,this->indent_size,this->indent_buffer.data);
}/*}}}*/

// -- bc_block_s --
@begin
inlines bc_block_s
@end

// -- ui_array_s --
@begin
inlines ui_array_s
@end

#if OPTION_MEMCHECK == ENABLED
#define check_malloc(SIZE) malloc(SIZE)
#define check_realloc(LOCATION,SIZE) realloc(LOCATION,SIZE)
#define check_free(LOCATION) free(LOCATION)

// -- mc_block_s --
@begin
inlines mc_block_s
@end

// -- mc_block_tree_s --
@begin
inlines mc_block_tree_s
@end

static inline int mc_block_tree_s___compare_value(const mc_block_tree_s *this,const mc_block_s *a_first,const mc_block_s *a_second)
{/*{{{*/
  (void)this;

  pointer f_location = a_first->location;
  pointer s_location = a_second->location;

  return f_location < s_location ? -1 : f_location > s_location ? 1 : 0;
}/*}}}*/

// -- memcheck_s --
@begin
inlines memcheck_s
@end

static inline pointer memcheck_s_get_block(memcheck_s *this,unsigned a_size)
{/*{{{*/
  pointer location = malloc(a_size);

  mc_block_s mc_block = {location,a_size};
  mc_block_tree_s_insert(&this->mc_block_set,&mc_block);

  this->alloc_size += a_size;
  this->total_alloc_size += a_size;

  if (this->alloc_size > this->max_alloc_size)
  {
    this->max_alloc_size = this->alloc_size;
  }

  return location;
}/*}}}*/

static inline pointer memcheck_s_reget_block(memcheck_s *this,pointer a_location,unsigned a_size)
{/*{{{*/

  if (a_location != NULL)
  {
    mc_block_s mc_block = {a_location,0};
    unsigned mcb_idx = mc_block_tree_s_get_idx(&this->mc_block_set,&mc_block);
    cassert(mcb_idx != c_idx_not_exist);

    this->alloc_size -= mc_block_tree_s_at(&this->mc_block_set,mcb_idx)->size;
    mc_block_tree_s_remove(&this->mc_block_set,mcb_idx);
  }

  pointer location = realloc(a_location,a_size);

  {
    mc_block_s mc_block = {location,a_size};
    mc_block_tree_s_insert(&this->mc_block_set,&mc_block);

    this->alloc_size += a_size;
    this->total_alloc_size += a_size;

    if (this->alloc_size > this->max_alloc_size)
    {
      this->max_alloc_size = this->alloc_size;
    }
  }

  return location;
}/*}}}*/

static inline void memcheck_s_release_block(memcheck_s *this,pointer a_location)
{/*{{{*/
  mc_block_s mc_block = {a_location,0};
  unsigned mcb_idx = mc_block_tree_s_get_idx(&this->mc_block_set,&mc_block);
  cassert(mcb_idx != c_idx_not_exist);

  this->alloc_size -= mc_block_tree_s_at(&this->mc_block_set,mcb_idx)->size;
  mc_block_tree_s_remove(&this->mc_block_set,mcb_idx);

  free (a_location);
}/*}}}*/

#undef check_malloc
#undef check_realloc
#undef check_free

static inline pointer check_malloc(unsigned a_size)
{/*{{{*/
  return memcheck_s_get_block(g_memcheck,a_size);
}/*}}}*/

static inline pointer check_realloc(pointer a_location,unsigned a_size)
{/*{{{*/
  return memcheck_s_reget_block(g_memcheck,a_location,a_size);
}/*}}}*/

static inline void check_free(pointer a_location)
{/*{{{*/
  memcheck_s_release_block(g_memcheck,a_location);
}/*}}}*/
#endif

// === inline basic data types to_string methods ===============================

static inline void bc_to_string(const bc *this,bc_array_s *a_trg)
{/*{{{*/
  if (isprint(*this))
  {
    bc_array_s_push(a_trg,*this);
  }
  else
  {
    bc_array_s_append_format(a_trg,"0x%hhx",*this);
  }
}/*}}}*/

#define BASIC_TYPE_TO_STRING_INLINE(TYPE,FORMAT) \
static inline void TYPE ## _to_string(const TYPE *this,bc_array_s *a_trg)\
{/*{{{*/\
  bc_array_s_append_format(a_trg,FORMAT,*this);\
}/*}}}*/

BASIC_TYPE_TO_STRING_INLINE(uc,"%hhu");
BASIC_TYPE_TO_STRING_INLINE(si,"%hd");
BASIC_TYPE_TO_STRING_INLINE(usi,"%hu");
BASIC_TYPE_TO_STRING_INLINE(bi,"%d");
BASIC_TYPE_TO_STRING_INLINE(ui,"%u");
BASIC_TYPE_TO_STRING_INLINE(lli,"%" HOST_LL_FORMAT "d");
BASIC_TYPE_TO_STRING_INLINE(ulli,"%" HOST_LL_FORMAT "u");
BASIC_TYPE_TO_STRING_INLINE(bf,"%f");
BASIC_TYPE_TO_STRING_INLINE(bd,"%f");
BASIC_TYPE_TO_STRING_INLINE(pointer,"%p");
BASIC_TYPE_TO_STRING_INLINE(bc_pointer,"%p");

#endif

