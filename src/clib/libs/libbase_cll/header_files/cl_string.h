
#ifndef __CL_STRING_H
#define __CL_STRING_H

@begin
include "cl_cont.h"
@end

// === constants and definitions ===============================================

// - string terminating character -
libbase_cll_EXPORT extern const char c_string_terminating_char;

// === definition of structure string_s ========================================

@begin
define string_s dynamic
@end

typedef struct string_s
{
  unsigned size;
  char *data;
} string_s;

#define INIT_STRING \
.size = 1,\
.data = (char *)&c_string_terminating_char\

#define STRING_S_FORMAT(NAME,FORMAT,...) \
  CONT_INIT(string_s,NAME);\
  string_s_set_format(&NAME,FORMAT,__VA_ARGS__)\

static inline void string_s_init(string_s *this);
static inline void string_s_clear(string_s *this);
static inline void string_s_create(string_s *this,unsigned a_length);
static inline void string_s_set(string_s *this,unsigned a_length,const char *a_data);
static inline void string_s_set_ptr(string_s *this,const char *a_data);
libbase_cll_EXPORT void string_s_set_format(string_s *this,const char *a_format,...);
static inline void string_s_flush_all(string_s *this);
static inline void string_s_swap(string_s *this,string_s *a_second);
static inline void string_s_copy(string_s *this,const string_s *a_src);
static inline int string_s_compare(const string_s *this,const string_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void string_s_to_string(const string_s *this,bc_array_s *a_trg);
#endif
static inline void string_s_to_json(const string_s *this,bc_array_s *a_trg);
#if OPTION_TO_JSON == ENABLED
static inline void string_s_to_json_nice(const string_s *this,void *a_json_nice,bc_array_s *a_trg);
#endif

libbase_cll_EXPORT void string_s_buffer_to_json(const char *a_ptr,const char *a_ptr_end,bc_array_s *a_trg);
libbase_cll_EXPORT unsigned string_s_get_idx(string_s *this,unsigned a_idx,unsigned a_length,const char *a_data);

// === definition of generated structures ======================================

// -- string_array_s --
@begin
array<string_s> string_array_s;
@end

static inline void string_array_s_push_ptr(string_array_s *this,const char *a_data);

// -- string_tree_s --
@begin
rb_tree<string_s> string_tree_s;
@end

static inline unsigned string_tree_s_insert_ptr(string_tree_s *this,const char *a_data);

static inline unsigned string_tree_s_get_idx_ptr(const string_tree_s *this,const char *a_data);

// === inline methods of structure string_s ====================================

static inline void string_s_init(string_s *this)
{/*{{{*/
  this->size = 1;
  this->data = (char *)&c_string_terminating_char;
}/*}}}*/

static inline void string_s_clear(string_s *this)
{/*{{{*/
  if (this->data != &c_string_terminating_char)
  {
    cfree(this->data);
  }

  string_s_init(this);
}/*}}}*/

static inline void string_s_create(string_s *this,unsigned a_length)
{/*{{{*/
  string_s_clear(this);

  if (a_length == 0)
  {
    return;
  }

  this->data = (char *)cmalloc((a_length + 1)*sizeof(char));

  this->data[a_length] = '\0';
  this->size = a_length + 1;
}/*}}}*/

static inline void string_s_set(string_s *this,unsigned a_length,const char *a_data)
{/*{{{*/
  string_s_clear(this);

  if (a_length == 0)
  {
    return;
  }

  this->data = (char *)cmalloc((a_length + 1)*sizeof(char));
  memcpy(this->data,a_data,a_length*sizeof(char));

  this->data[a_length] = '\0';
  this->size = a_length + 1;
}/*}}}*/

static inline void string_s_set_ptr(string_s *this,const char *a_data)
{/*{{{*/
  string_s_set(this,strlen(a_data),a_data);
}/*}}}*/

static inline void string_s_flush_all(string_s *this)
{/*{{{*/
}/*}}}*/

static inline void string_s_swap(string_s *this,string_s *a_second)
{/*{{{*/
  string_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void string_s_copy(string_s *this,const string_s *a_src)
{/*{{{*/
  string_s_clear(this);

  if (a_src->data == &c_string_terminating_char)
  {
    return;
  }

  debug_assert(a_src->size > 0);
  this->data = (char *)cmalloc(a_src->size*sizeof(char));
  memcpy(this->data,a_src->data,(a_src->size - 1)*sizeof(char));

  this->data[a_src->size - 1] = '\0';
  this->size = a_src->size;
}/*}}}*/

static inline int string_s_compare(const string_s *this,const string_s *a_second)
{/*{{{*/
  if (this->size != a_second->size) { return 0; }
  if (this->data == &c_string_terminating_char) { return 1; }
  return (memcmp(this->data,a_second->data,(this->size - 1)*sizeof(char)) == 0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void string_s_to_string(const string_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append(a_trg,this->size - 1,this->data);
}/*}}}*/
#endif

static inline void string_s_to_json(const string_s *this,bc_array_s *a_trg)
{/*{{{*/
  string_s_buffer_to_json(this->data,this->data + this->size - 1,a_trg);
}/*}}}*/

#if OPTION_TO_JSON == ENABLED
static inline void string_s_to_json_nice(const string_s *this,void *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  (void)a_json_nice;

  string_s_buffer_to_json(this->data,this->data + this->size - 1,a_trg);
}/*}}}*/
#endif

// === inline methods of generated structures ==================================

// -- string_array_s --
@begin
inlines string_array_s
@end

static inline void string_array_s_push_ptr(string_array_s *this,const char *a_data)
{/*{{{*/
  string_s string = {strlen(a_data) + 1,(char *)a_data};
  return string_array_s_push(this,&string);
}/*}}}*/

// -- string_tree_s --
@begin
inlines string_tree_s
@end

static inline int string_tree_s___compare_value(const string_tree_s *this,const string_s *a_first,const string_s *a_second)
{/*{{{*/
  (void)this;

  if (a_first->size < a_second->size) { return -1; }
  if (a_first->size > a_second->size) { return 1; }
  return memcmp(a_first->data,a_second->data,a_first->size - 1);
}/*}}}*/

static inline unsigned string_tree_s_insert_ptr(string_tree_s *this,const char *a_data)
{/*{{{*/
  string_s string = {strlen(a_data) + 1,(char *)a_data};
  return string_tree_s_insert(this,&string);
}/*}}}*/

static inline unsigned string_tree_s_get_idx_ptr(const string_tree_s *this,const char *a_data)
{/*{{{*/
  string_s string = {strlen(a_data) + 1,(char *)a_data};
  return string_tree_s_get_idx(this,&string);
}/*}}}*/

#endif

