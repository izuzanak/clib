
#ifndef __CL_VAR_H
#define __CL_VAR_H

@begin
include "cl_atomic.h"
include "cl_struct.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libvar_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libvar_cll_EXPORTS
#define libvar_cll_EXPORT __declspec(dllexport)
#else
#define libvar_cll_EXPORT __declspec(dllimport)
#endif
#endif

// === constants and definitions ===============================================

// - variable types -
enum {
  c_bi_type_blank = 0,
  c_bi_type_integer,
  c_bi_type_float,
  c_bi_type_string,
  c_bi_type_array,
  c_bi_type_dict,
  c_bi_type_count,
};

// === definition of structure loc_s ===========================================

// - v_data_type typedef -
typedef union v_data_type
{
  long long int lli;
  double bd;
  pointer ptr;
} v_data_type;

@begin
define v_data_type basic
@end

typedef struct loc_s
{
  unsigned v_type;
  atomic_s v_ref_cnt;
  v_data_type v_data;
} loc_s;

typedef struct loc_s * var_s;

static inline var_s loc_s___new();
static inline void loc_s___release(var_s this);
static inline int loc_s_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_to_string(var_s this,bc_array_s *a_trg);
#endif

// === definition of structure loc_s function maps =============================

#define LOC_S_MAX_TYPES 32
libvar_cll_EXPORT extern unsigned g_loc_s_type_cnt;

typedef void (*loc_s___clear_t)(var_s this);
libvar_cll_EXPORT extern loc_s___clear_t g_loc_s___clear[LOC_S_MAX_TYPES];

typedef int (*loc_s___order_t)(var_s a_first,var_s a_second);
libvar_cll_EXPORT extern loc_s___order_t g_loc_s___order[LOC_S_MAX_TYPES];

#if OPTION_TO_STRING == ENABLED
typedef void (*loc_s___to_string_t)(var_s this,bc_array_s *a_trg);
libvar_cll_EXPORT extern loc_s___to_string_t g_loc_s___to_string[LOC_S_MAX_TYPES];
#endif

libvar_cll_EXPORT unsigned loc_s_register_type(
  loc_s___clear_t a_clear,
  loc_s___order_t a_order,
#if OPTION_TO_STRING == ENABLED
  loc_s___to_string_t a_to_string
#else
  void *a_to_string
#endif
  );

// === definition of var_s =====================================================

@begin
define var_s dynamic
@end

static inline void var_s_init(var_s *this);
static inline void var_s_clear(var_s *this);
static inline void var_s_flush_all(var_s *this);
static inline void var_s_swap(var_s *this,var_s *a_second);
static inline void var_s_copy(var_s *this,const var_s *a_src);
static inline void var_s_copy_loc(var_s *this,var_s a_src);
static inline int var_s_compare(const var_s *this,const var_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void var_s_to_string(const var_s *this,bc_array_s *a_trg);
#endif

// === definition of generated structures ======================================

// -- var_array_s --
@begin
array<var_s> var_array_s;
@end

static inline void var_array_s_push_loc(var_array_s *this,var_s a_value);

// -- var_arrays_s --
@begin
array<var_array_s> var_arrays_s;
@end

// -- var_map_s --
@begin
struct
<
var_s:key
var_s:value
>
var_map_s;
@end

static inline int var_map_s_order(const var_map_s *a_first,const var_map_s *a_second);
#if OPTION_TO_STRING == ENABLED
#undef var_map_s_to_string
static inline void var_map_s_to_string(const var_map_s *this,bc_array_s *a_trg);
#endif

// -- var_map_tree_s --
@begin
safe_rb_tree<var_map_s> var_map_tree_s;
@end

// === definition of loc_s types ===============================================

// - type BLANK -
static inline var_s loc_s_blank();
static inline void loc_s_blank_clear(var_s this);
static inline int loc_s_blank_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_blank_to_string(var_s this,bc_array_s *a_trg);
#endif

// - type INTEGER -
static inline var_s loc_s_int(long long int a_value);
static inline void loc_s_int_clear(var_s this);
static inline int loc_s_int_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_int_to_string(var_s this,bc_array_s *a_trg);
#endif
static inline long long int loc_s_int_value(var_s this);

// - type FLOAT -
static inline var_s loc_s_float(double a_value);
static inline void loc_s_float_clear(var_s this);
static inline int loc_s_float_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_float_to_string(var_s this,bc_array_s *a_trg);
#endif
static inline double loc_s_float_value(var_s this);

// - type STRING -
static inline var_s loc_s_string(unsigned a_length,const char *a_data);
static inline var_s loc_s_string_ptr(const char *a_data);
static inline var_s loc_s_string_str(string_s *a_src);
static inline var_s loc_s_string_swap(string_s *a_src);
static inline void loc_s_string_clear(var_s this);
static inline int loc_s_string_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_string_to_string(var_s this,bc_array_s *a_trg);
#endif
static inline const string_s *loc_s_string_value(var_s this);

// - type ARRAY -
static inline var_s loc_s_array();
static inline void loc_s_array_clear(var_s this);
static inline int loc_s_array_order(var_s a_first,var_s a_second);
libvar_cll_EXPORT int loc_s_array___order(const var_array_s *a_first,const var_array_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_array_to_string(var_s this,bc_array_s *a_trg);
#endif
static inline var_array_s *loc_s_array_value(var_s this);
static inline unsigned loc_s_array_length(var_s this);
static inline void loc_s_array_push(var_s this,var_s a_value);
static inline var_s loc_s_array_pop(var_s this);
static inline var_s *loc_s_array_at(var_s this,unsigned a_idx);

// - type DICT -
static inline var_s loc_s_dict();
static inline void loc_s_dict_clear(var_s this);
static inline int loc_s_dict_order(var_s a_first,var_s a_second);
libvar_cll_EXPORT int loc_s_dict___order(const var_map_tree_s *a_first,const var_map_tree_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_dict_to_string(var_s this,bc_array_s *a_trg);
#endif
static inline var_map_tree_s *loc_s_dict_value(var_s this);
static inline unsigned loc_s_dict_length(var_s this);
static inline int loc_s_dict_has_key(var_s this,var_s a_key);
static inline void loc_s_dict_remove_key(var_s this,var_s a_key);
static inline void loc_s_dict_set(var_s this,var_s a_key,var_s a_value);
static inline var_s loc_s_dict_get(var_s this,var_s a_key);

// === inline methods of structure loc_s =======================================

static inline var_s loc_s___new()
{/*{{{*/
  return (var_s)cmalloc(sizeof(loc_s));
}/*}}}*/

static inline void loc_s___release(var_s this)
{/*{{{*/

// - do not analyze: clang assumes that condition is always true for some reason -
#ifndef __clang_analyzer__
  if (atomic_s_dec_and_test(&this->v_ref_cnt))
  {
    g_loc_s___clear[this->v_type](this);
    cfree(this);
  }
#endif
}/*}}}*/

static inline int loc_s_order(var_s a_first,var_s a_second)
{/*{{{*/
  if (a_first->v_type != a_second->v_type)
  {
    return a_first->v_type < a_second->v_type ? -1 : 1;
  }

  return g_loc_s___order[a_first->v_type](a_first,a_second);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
void loc_s_to_string(var_s this,bc_array_s *a_trg)
{/*{{{*/
  g_loc_s___to_string[this->v_type](this,a_trg);
}/*}}}*/
#endif

// === inline methods of var_s =================================================

static inline void var_s_init(var_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void var_s_clear(var_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    loc_s___release(*this);
    *this = NULL;
  }
}/*}}}*/

static inline void var_s_flush_all(var_s *this)
{/*{{{*/
}/*}}}*/

static inline void var_s_swap(var_s *this,var_s *a_second)
{/*{{{*/
  var_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void var_s_copy(var_s *this,const var_s *a_src)
{/*{{{*/
  if (*this != NULL)
  {
    loc_s___release(*this);
  }

  if (*a_src != NULL)
  {
    atomic_s_inc(&(*a_src)->v_ref_cnt);
  }

  *this = *a_src;
}/*}}}*/

static inline void var_s_copy_loc(var_s *this,var_s a_src)
{/*{{{*/
  var_s_copy(this,&a_src);
}/*}}}*/

static inline int var_s_compare(const var_s *this,const var_s *a_second)
{/*{{{*/
  return *this ?
    (*a_second ? !loc_s_order(*this,*a_second) : 0) :
    (*a_second ? 0 : 1);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void var_s_to_string(const var_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (*this != NULL)
  {
    loc_s_to_string(*this,a_trg);
  }
  else
  {
    bc_array_s_append_ptr(a_trg,"<null>");
  }
}/*}}}*/
#endif

#define VAR(NAME,VALUE) \
  CONT_INIT(var_s,NAME);\
  var_s_copy_loc(&NAME,VALUE)

#define VAR_CLEAR(NAME,VALUE) \
  CONT_INIT_CLEAR(var_s,NAME);\
  var_s_copy_loc(&NAME,VALUE)

// === inline methods of generated structures ==================================

// -- var_array_s --
@begin
inlines var_array_s
@end

static inline void var_array_s_push_loc(var_array_s *this,var_s a_value)
{/*{{{*/
  var_array_s_push(this,&a_value);
}/*}}}*/

// -- var_arrays_s --
@begin
inlines var_arrays_s
@end

// -- var_map_s --
@begin
inlines var_map_s
@end

static inline int var_map_s_order(const var_map_s *a_first,const var_map_s *a_second)
{/*{{{*/
  int result;

  result = loc_s_order(a_first->key,a_second->key);
  if (result != 0)
  {
    return result;
  }

  result = loc_s_order(a_first->value,a_second->value);
  if (result != 0)
  {
    return result;
  }

  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void var_map_s_to_string(const var_map_s *this,bc_array_s *a_trg)
{/*{{{*/
  var_s_to_string(&this->key,a_trg);
  bc_array_s_push(a_trg,':');
  var_s_to_string(&this->value,a_trg);
}/*}}}*/
#endif

// -- var_map_tree_s --
@begin
inlines var_map_tree_s
@end

static inline int var_map_tree_s___compare_value(const var_map_tree_s *this,const var_map_s *a_first,const var_map_s *a_second)
{/*{{{*/
  (void)this;

  return loc_s_order(a_first->key,a_second->key);
}/*}}}*/

// === inline methods of loc_s types ===========================================

// - type BLANK -
static inline var_s loc_s_blank()
{/*{{{*/
  var_s var = loc_s___new();
  var->v_type = c_bi_type_blank;
  atomic_s_set(&var->v_ref_cnt,0);

  return var;
}/*}}}*/

static inline void loc_s_blank_clear(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_blank);
}/*}}}*/

static inline int loc_s_blank_order(var_s a_first,var_s a_second)
{/*{{{*/
  debug_assert(a_first->v_type == c_bi_type_blank && a_second->v_type == c_bi_type_blank);

  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void loc_s_blank_to_string(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_blank);

  bc_array_s_append_ptr(a_trg,"<blank>");
}/*}}}*/
#endif

// - type INTEGER -
static inline var_s loc_s_int(long long int a_value)
{/*{{{*/
  var_s var = loc_s___new();
  var->v_type = c_bi_type_integer;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.lli = a_value;

  return var;
}/*}}}*/

static inline void loc_s_int_clear(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_integer);
}/*}}}*/

static inline int loc_s_int_order(var_s a_first,var_s a_second)
{/*{{{*/
  debug_assert(a_first->v_type == c_bi_type_integer && a_second->v_type == c_bi_type_integer);

  long long int first = a_first->v_data.lli;
  long long int second = a_second->v_data.lli;

  return first < second ? -1 : (first > second ? 1 : 0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void loc_s_int_to_string(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_integer);

  lli_to_string(&this->v_data.lli,a_trg);
}/*}}}*/
#endif

static inline long long int loc_s_int_value(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_integer);

  return this->v_data.lli;
}/*}}}*/

// - type FLOAT -
static inline var_s loc_s_float(double a_value)
{/*{{{*/
  var_s var = loc_s___new();
  var->v_type = c_bi_type_float;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.bd = a_value;

  return var;
}/*}}}*/

static inline void loc_s_float_clear(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_float);
}/*}}}*/

static inline int loc_s_float_order(var_s a_first,var_s a_second)
{/*{{{*/
  debug_assert(a_first->v_type == c_bi_type_float && a_second->v_type == c_bi_type_float);

  double first = a_first->v_data.bd;
  double second = a_second->v_data.bd;

  return first < second ? -1 : (first > second ? 1 : 0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void loc_s_float_to_string(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_float);

  bd_to_string(&this->v_data.bd,a_trg);
}/*}}}*/
#endif

static inline double loc_s_float_value(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_float);

  return this->v_data.bd;
}/*}}}*/

// - type STRING -
static inline var_s loc_s_string(unsigned a_length,const char *a_data)
{/*{{{*/
  string_s *string = (string_s *)cmalloc(sizeof(string_s));
  string_s_init(string);
  string_s_set(string,a_length,a_data);

  var_s var = loc_s___new();
  var->v_type = c_bi_type_string;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.ptr = string;

  return var;
}/*}}}*/

static inline var_s loc_s_string_ptr(const char *a_data)
{/*{{{*/
  return loc_s_string(strlen(a_data),a_data);
}/*}}}*/

static inline var_s loc_s_string_str(string_s *a_src)
{/*{{{*/
  return loc_s_string(a_src->size - 1,a_src->data);
}/*}}}*/

static inline var_s loc_s_string_swap(string_s *a_src)
{/*{{{*/
  string_s *string = (string_s *)cmalloc(sizeof(string_s));
  string_s_init(string);
  string_s_swap(string,a_src);

  var_s var = loc_s___new();
  var->v_type = c_bi_type_string;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.ptr = string;

  return var;
}/*}}}*/

static inline void loc_s_string_clear(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_string);

  string_s *string = (string_s *)this->v_data.ptr;
  string_s_clear(string);
  cfree(string);
}/*}}}*/

static inline int loc_s_string_order(var_s a_first,var_s a_second)
{/*{{{*/
  debug_assert(a_first->v_type == c_bi_type_string && a_second->v_type == c_bi_type_string);

  string_s *first = (string_s *)a_first->v_data.ptr;
  string_s *second = (string_s *)a_second->v_data.ptr;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }

  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void loc_s_string_to_string(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_string);

  string_s *string = (string_s *)this->v_data.ptr;
  string_s_to_string(string,a_trg);
}/*}}}*/
#endif

static inline const string_s *loc_s_string_value(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_string);

  return (string_s *)this->v_data.ptr;
}/*}}}*/

// - type ARRAY -
static inline var_s loc_s_array()
{/*{{{*/
  var_array_s *array = (var_array_s *)cmalloc(sizeof(var_array_s));
  var_array_s_init(array);

  var_s var = loc_s___new();
  var->v_type = c_bi_type_array;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.ptr = array;

  return var;
}/*}}}*/

static inline void loc_s_array_clear(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_array);

  var_array_s *array = (var_array_s *)this->v_data.ptr;

  var_array_s_clear(array);
  cfree(array);
}/*}}}*/

static inline int loc_s_array_order(var_s a_first,var_s a_second)
{/*{{{*/
  debug_assert(a_first->v_type == c_bi_type_array && a_second->v_type == c_bi_type_array);

  var_array_s *first = (var_array_s *)a_first->v_data.ptr;
  var_array_s *second = (var_array_s *)a_second->v_data.ptr;

  if (first->used < second->used) { return -1; }
  if (first->used > second->used) { return 1; }

  return loc_s_array___order(first,second);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void loc_s_array_to_string(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_array);

  var_array_s *array = (var_array_s *)this->v_data.ptr;

  var_array_s_to_string(array,a_trg);
}/*}}}*/
#endif

static inline var_array_s *loc_s_array_value(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_array);

  return (var_array_s *)this->v_data.ptr;
}/*}}}*/

static inline unsigned loc_s_array_length(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_array);

  var_array_s *array = (var_array_s *)this->v_data.ptr;

  return array->used;
}/*}}}*/

static inline void loc_s_array_push(var_s this,var_s a_value)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_array);

  var_array_s *array = (var_array_s *)this->v_data.ptr;

  var_array_s_push_loc(array,a_value);
}/*}}}*/

static inline var_s loc_s_array_pop(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_array);

  var_array_s *array = (var_array_s *)this->v_data.ptr;

  return *var_array_s_pop(array);
}/*}}}*/

static inline var_s *loc_s_array_at(var_s this,unsigned a_idx)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_array);

  var_array_s *array = (var_array_s *)this->v_data.ptr;

  return var_array_s_at(array,a_idx);
}/*}}}*/

// - type DICT -
static inline var_s loc_s_dict()
{/*{{{*/
  var_map_tree_s *tree = (var_map_tree_s *)cmalloc(sizeof(var_map_tree_s));
  var_map_tree_s_init(tree);

  var_s var = loc_s___new();
  var->v_type = c_bi_type_dict;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.ptr = tree;

  return var;
}/*}}}*/

static inline void loc_s_dict_clear(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_dict);

  var_map_tree_s *tree = (var_map_tree_s *)this->v_data.ptr;

  var_map_tree_s_clear(tree);
  cfree(tree);
}/*}}}*/

static inline int loc_s_dict_order(var_s a_first,var_s a_second)
{/*{{{*/
  debug_assert(a_first->v_type == c_bi_type_dict && a_second->v_type == c_bi_type_dict);

  var_map_tree_s *first = (var_map_tree_s *)a_first->v_data.ptr;
  var_map_tree_s *second = (var_map_tree_s *)a_second->v_data.ptr;

  if (first->count < second->count) { return -1; }
  if (first->count > second->count) { return 1; }

  return loc_s_dict___order(first,second);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void loc_s_dict_to_string(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_dict);

  var_map_tree_s *tree = (var_map_tree_s *)this->v_data.ptr;

  var_map_tree_s_to_string(tree,a_trg);
}/*}}}*/
#endif

static inline var_map_tree_s *loc_s_dict_value(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_dict)

  return (var_map_tree_s *)this->v_data.ptr;
}/*}}}*/

static inline unsigned loc_s_dict_length(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_dict)

  var_map_tree_s *tree = (var_map_tree_s *)this->v_data.ptr;

  return tree->count;
}/*}}}*/

static inline int loc_s_dict_has_key(var_s this,var_s a_key)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_dict)

  var_map_tree_s *tree = (var_map_tree_s *)this->v_data.ptr;

  var_map_s search_map = {a_key,NULL};
  return c_idx_not_exist != var_map_tree_s_get_idx(tree,&search_map);
}/*}}}*/

static inline void loc_s_dict_remove_key(var_s this,var_s a_key)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_dict)

  var_map_tree_s *tree = (var_map_tree_s *)this->v_data.ptr;

  var_map_s search_map = {a_key,NULL};
  unsigned index = var_map_tree_s_get_idx(tree,&search_map);
  debug_assert(index != c_idx_not_exist);

  var_map_tree_s_remove(tree,index);
}/*}}}*/

static inline void loc_s_dict_set(var_s this,var_s a_key,var_s a_value)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_dict)

  var_map_tree_s *tree = (var_map_tree_s *)this->v_data.ptr;

  var_map_s insert_map = {a_key,a_value};
  unsigned index = var_map_tree_s_unique_insert(tree,&insert_map);

  var_map_s *map = &(tree->data + index)->object;

  if (map->value != a_value)
  {
    var_s_copy_loc(&map->value,a_value);
  }
}/*}}}*/

static inline var_s loc_s_dict_get(var_s this,var_s a_key)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_dict)

  var_map_tree_s *tree = (var_map_tree_s *)this->v_data.ptr;

  var_map_s search_map = {a_key,NULL};
  unsigned index = var_map_tree_s_get_idx(tree,&search_map);

  if (index == c_idx_not_exist)
  {
    return NULL;
  }

  return (tree->data + index)->object.value;
}/*}}}*/

#endif

