
#ifndef __CL_CONT_H
#define __CL_CONT_H

@begin
include "cl_basic.h"
@end

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
array<bc>
additions
{
static inline void bc_array_s_append(bc_array_s *this,unsigned a_count,const char *a_data);
static inline void bc_array_s_append_ptr(bc_array_s *this,const char *a_data);
libbase_cll_EXPORT void bc_array_s_append_format(bc_array_s *this,const char *a_format,...);
static inline void bc_array_s_tail(bc_array_s *this,unsigned a_count);
}
bc_array_s;
@end

// -- ui_array_s --
@begin
array<ui> ui_array_s;
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
ui:alloc_size
ui:total_alloc_size
ui:max_alloc_size
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
BASIC_TYPE_TO_STRING_DEFINE(bd);
BASIC_TYPE_TO_STRING_DEFINE(pointer);

// === definition of to_json methods for basic data types ======================

#define   si_to_json   si_to_string
#define  usi_to_json  usi_to_string
#define   bi_to_json   bi_to_string
#define   ui_to_json   ui_to_string
#define  lli_to_json  lli_to_string
#define ulli_to_json ulli_to_string
#define   bd_to_json   bd_to_string

#define   si_to_json_nice(VALUE,NICE,TARGET)   si_to_json(VALUE,TARGET)
#define  usi_to_json_nice(VALUE,NICE,TARGET)  usi_to_json(VALUE,TARGET)
#define   bi_to_json_nice(VALUE,NICE,TARGET)   bi_to_json(VALUE,TARGET)
#define   ui_to_json_nice(VALUE,NICE,TARGET)   ui_to_json(VALUE,TARGET)
#define  lli_to_json_nice(VALUE,NICE,TARGET)  lli_to_json(VALUE,TARGET)
#define ulli_to_json_nice(VALUE,NICE,TARGET) ulli_to_json(VALUE,TARGET)
#define   bd_to_json_nice(VALUE,NICE,TARGET)   bd_to_json(VALUE,TARGET)

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

#define BASIC_TYPE_TO_STRING_INLINE(TYPE,FORMAT) \
static inline void TYPE ## _to_string(const TYPE *this,bc_array_s *a_trg)\
{/*{{{*/\
  int space = a_trg->size - a_trg->used;\
  int length = snprintf(a_trg->data + a_trg->used,space,FORMAT,*this);\
\
  if (length >= space)\
  {\
    bc_array_s_reserve(a_trg,length + 1);\
\
    space = a_trg->size - a_trg->used;\
    length = snprintf(a_trg->data + a_trg->used,space,FORMAT,*this);\
  }\
\
  a_trg->used += length;\
}/*}}}*/

BASIC_TYPE_TO_STRING_INLINE(bc,"%c");
BASIC_TYPE_TO_STRING_INLINE(uc,"%hhu");
BASIC_TYPE_TO_STRING_INLINE(si,"%hd");
BASIC_TYPE_TO_STRING_INLINE(usi,"%hu");
BASIC_TYPE_TO_STRING_INLINE(bi,"%d");
BASIC_TYPE_TO_STRING_INLINE(ui,"%u");
BASIC_TYPE_TO_STRING_INLINE(lli,"%" HOST_LL_FORMAT "d");
BASIC_TYPE_TO_STRING_INLINE(ulli,"%" HOST_LL_FORMAT "u");
BASIC_TYPE_TO_STRING_INLINE(bd,"%f");
BASIC_TYPE_TO_STRING_INLINE(pointer,"%p");

#endif

