
#ifndef CL_LIB_VAR_H
#define CL_LIB_VAR_H

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

// - error codes -
#define ERROR_FROM_VAR_ERROR 1

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

static inline var_s loc_s___new(void);
static inline void loc_s___release(var_s this);
static inline int loc_s_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_to_string(var_s this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void loc_s_to_json(var_s this,bc_array_s *a_trg);
static inline void loc_s_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg);
#endif
libvar_cll_EXPORT var_s loc_s_at_path(var_s this,const char *a_path);

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

#if OPTION_TO_JSON == ENABLED
typedef void (*loc_s___to_json_t)(var_s this,bc_array_s *a_trg);
libvar_cll_EXPORT extern loc_s___to_json_t g_loc_s___to_json[LOC_S_MAX_TYPES];

typedef void (*loc_s___to_json_nice_t)(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg);
libvar_cll_EXPORT extern loc_s___to_json_nice_t g_loc_s___to_json_nice[LOC_S_MAX_TYPES];
#endif

libvar_cll_EXPORT unsigned loc_s_register_type(
  loc_s___clear_t a_clear,
  loc_s___order_t a_order,
#if OPTION_TO_STRING == ENABLED
  loc_s___to_string_t a_to_string,
#else
  void *a_to_string,
#endif
#if OPTION_TO_JSON == ENABLED
  loc_s___to_json_t a_to_json,
  loc_s___to_json_nice_t a_to_json_nice
#else
  void *a_to_json,
  void *a_to_json_nice
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
#if OPTION_TO_JSON == ENABLED
static inline void var_s_to_json(const var_s *this,bc_array_s *a_trg);
static inline void var_s_to_json_nice(const var_s *this,json_nice_s *a_json_nice,bc_array_s *a_trg);
#endif
#if OPTION_FROM_VAR == ENABLED
WUR static inline int var_s_from_var(var_s *this,var_s a_var);
#endif

// === definition of generated structures ======================================

// -- var_array_s --
@begin
array<var_s>
options ( to_json to_json_nice )
var_array_s;
@end

static inline void var_array_s_push_loc(var_array_s *this,var_s a_value);
static inline void var_array_s_push___locs(var_array_s *this,unsigned a_count,...);
#define var_array_s_push_locs(THIS,...) var_array_s_push___locs(THIS,VAR_LIST(__VA_ARGS__))
libvar_cll_EXPORT void var_array_s_push_locs_ap(var_array_s *this,unsigned a_count,va_list a_ap);

// -- var_arrays_s --
@begin
array<var_array_s> var_arrays_s;
@end

// -- var_queue_s --
@begin
queue<var_s> var_queue_s;
@end

// -- var_tree_s --
@begin
safe_rb_tree<var_s> var_tree_s;
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

#if OPTION_TO_JSON == ENABLED
void var_map_tree_s_to_json(const var_map_tree_s *this,bc_array_s *a_trg);
void var_map_tree_s_to_json_nice(const var_map_tree_s *this,json_nice_s *a_json_nice,bc_array_s *a_trg);
#endif

static inline void var_map_tree_s_set___locs(var_map_tree_s *this,unsigned a_count,...);
#define var_map_tree_s_set_locs(THIS,...) var_map_tree_s_set___locs(THIS,VAR_LIST(__VA_ARGS__))
libvar_cll_EXPORT void var_map_tree_s_set_locs_ap(var_map_tree_s *this,unsigned a_count,va_list a_ap);

// === definition of loc_s types ===============================================

// - type BLANK -
static inline var_s loc_s_blank(void);
static inline void loc_s_blank_clear(var_s this);
static inline int loc_s_blank_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_blank_to_string(var_s this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void loc_s_blank_to_json(var_s this,bc_array_s *a_trg);
static inline void loc_s_blank_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg);
#endif

// - type INTEGER -
static inline var_s loc_s_int(long long int a_value);
static inline void loc_s_int_clear(var_s this);
static inline int loc_s_int_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_int_to_string(var_s this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void loc_s_int_to_json(var_s this,bc_array_s *a_trg);
static inline void loc_s_int_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg);
#endif
static inline long long int loc_s_int_value(var_s this);
static inline long long int *loc_s_int_value_ptr(var_s this);

// - type FLOAT -
static inline var_s loc_s_float(double a_value);
static inline void loc_s_float_clear(var_s this);
static inline int loc_s_float_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_float_to_string(var_s this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void loc_s_float_to_json(var_s this,bc_array_s *a_trg);
static inline void loc_s_float_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg);
#endif
static inline double loc_s_float_value(var_s this);
static inline double *loc_s_float_value_ptr(var_s this);

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
#if OPTION_TO_JSON == ENABLED
static inline void loc_s_string_to_json(var_s this,bc_array_s *a_trg);
static inline void loc_s_string_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg);
#endif
static inline const string_s *loc_s_string_value(var_s this);

// - type ARRAY -
static inline var_s loc_s_array(void);
static inline var_s loc_s_array___locs(unsigned a_count,...);
#define loc_s_array_locs(...) loc_s_array___locs(VAR_LIST(__VA_ARGS__))
static inline void loc_s_array_clear(var_s this);
static inline int loc_s_array_order(var_s a_first,var_s a_second);
libvar_cll_EXPORT int loc_s_array___order(const var_array_s *a_first,const var_array_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_array_to_string(var_s this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void loc_s_array_to_json(var_s this,bc_array_s *a_trg);
static inline void loc_s_array_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg);
#endif
static inline var_array_s *loc_s_array_value(var_s this);
static inline unsigned loc_s_array_length(var_s this);
static inline void loc_s_array_push(var_s this,var_s a_value);
static inline var_s loc_s_array_pop(var_s this);
static inline var_s *loc_s_array_at(var_s this,unsigned a_idx);

// - type DICT -
static inline var_s loc_s_dict(void);
static inline var_s loc_s_dict___locs(unsigned a_count,...);
#define loc_s_dict_locs(...) loc_s_dict___locs(VAR_LIST(__VA_ARGS__))
static inline void loc_s_dict_clear(var_s this);
static inline int loc_s_dict_order(var_s a_first,var_s a_second);
libvar_cll_EXPORT int loc_s_dict___order(const var_map_tree_s *a_first,const var_map_tree_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_dict_to_string(var_s this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void loc_s_dict_to_json(var_s this,bc_array_s *a_trg);
static inline void loc_s_dict_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg);
#endif
static inline var_map_tree_s *loc_s_dict_value(var_s this);
static inline unsigned loc_s_dict_length(var_s this);
static inline int loc_s_dict_has_key(var_s this,var_s a_key);
static inline void loc_s_dict_remove_key(var_s this,var_s a_key);
static inline var_s loc_s_dict_first_key(var_s this);
static inline void loc_s_dict_set(var_s this,var_s a_key,var_s a_value);
static inline void loc_s_dict_str_set(var_s this,const char *a_str_key,var_s a_value);
static inline var_s loc_s_dict_get(var_s this,var_s a_key);
static inline var_s loc_s_dict_str_get(var_s this,const char *a_str_key);

// === inline methods of structure loc_s =======================================

static inline var_s loc_s___new(void)
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

#if OPTION_TO_JSON == ENABLED
static inline void loc_s_to_json(var_s this,bc_array_s *a_trg)
{/*{{{*/
  g_loc_s___to_json[this->v_type](this,a_trg);
}/*}}}*/

static inline void loc_s_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  g_loc_s___to_json_nice[this->v_type](this,a_json_nice,a_trg);
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

#if OPTION_TO_JSON == ENABLED
static inline void var_s_to_json(const var_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (*this != NULL)
  {
    loc_s_to_json(*this,a_trg);
  }
  else
  {
    bc_array_s_append_ptr(a_trg,"null");
  }
}/*}}}*/

static inline void var_s_to_json_nice(const var_s *this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  if (*this != NULL)
  {
    loc_s_to_json_nice(*this,a_json_nice,a_trg);
  }
  else
  {
    bc_array_s_append_ptr(a_trg,"null");
  }
}/*}}}*/
#endif

#if OPTION_FROM_VAR == ENABLED
static inline int var_s_from_var(var_s *this,var_s a_var)
{/*{{{*/
  var_s_copy_loc(this,a_var);

  return 0;
}/*}}}*/
#endif

#define VAR(NAME,VALUE) \
  CONT_INIT(var_s,NAME);\
  var_s_copy_loc(&NAME,VALUE)

#define VAR_CLEAR(NAME,VALUE) \
  CONT_INIT_CLEAR(var_s,NAME);\
  var_s_copy_loc(&NAME,VALUE)

#define VAR_LIST_LENGTH(...) (sizeof((var_s[]){__VA_ARGS__})/sizeof(var_s))
#define VAR_LIST(...) VAR_LIST_LENGTH(__VA_ARGS__),__VA_ARGS__

// === definition of from_var methods for basic data types =====================

#if OPTION_FROM_VAR == ENABLED

#define BASIC_TYPE_FROM_VAR_DEFINE(TYPE) \
WUR static inline int TYPE ## _from_var(TYPE *this,var_s a_var);

BASIC_TYPE_FROM_VAR_DEFINE(bc);
BASIC_TYPE_FROM_VAR_DEFINE(uc);
BASIC_TYPE_FROM_VAR_DEFINE(si);
BASIC_TYPE_FROM_VAR_DEFINE(usi);
BASIC_TYPE_FROM_VAR_DEFINE(bi);
BASIC_TYPE_FROM_VAR_DEFINE(ui);
BASIC_TYPE_FROM_VAR_DEFINE(lli);
BASIC_TYPE_FROM_VAR_DEFINE(ulli);
BASIC_TYPE_FROM_VAR_DEFINE(bf);
BASIC_TYPE_FROM_VAR_DEFINE(bd);

WUR static inline int string_s_from_var(string_s *this,var_s a_var);

#endif

// === inline methods of generated structures ==================================

// -- var_array_s --
@begin
inlines var_array_s
@end

static inline void var_array_s_push_loc(var_array_s *this,var_s a_value)
{/*{{{*/
  var_array_s_push(this,&a_value);
}/*}}}*/

static inline void var_array_s_push___locs(var_array_s *this,unsigned a_count,...)
{/*{{{*/
  va_list ap;
  va_start(ap,a_count);
  var_array_s_push_locs_ap(this,a_count,ap);
  va_end(ap);
}/*}}}*/

// -- var_arrays_s --
@begin
inlines var_arrays_s
@end

// -- var_queue_s --
@begin
inlines var_queue_s
@end

// -- var_tree_s --
@begin
inlines var_tree_s
@end

static inline int var_tree_s___compare_value(const var_tree_s *this,const var_s *a_first,const var_s *a_second)
{/*{{{*/
  (void)this;

  return loc_s_order(*a_first,*a_second);
}/*}}}*/

// -- var_map_s --
@begin
inlines var_map_s
@end

static inline int var_map_s_order(const var_map_s *a_first,const var_map_s *a_second)
{/*{{{*/
  int res;

  res = loc_s_order(a_first->key,a_second->key);
  if (res != 0)
  {
    return res;
  }

  res = loc_s_order(a_first->value,a_second->value);
  if (res != 0)
  {
    return res;
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

static inline void var_map_tree_s_set___locs(var_map_tree_s *this,unsigned a_count,...)
{/*{{{*/
  va_list ap;
  va_start(ap,a_count);
  var_map_tree_s_set_locs_ap(this,a_count,ap);
  va_end(ap);
}/*}}}*/

// === inline methods of loc_s types ===========================================

// - type BLANK -
static inline var_s loc_s_blank(void)
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

#if OPTION_TO_JSON == ENABLED
static inline void loc_s_blank_to_json(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_blank);

  bc_array_s_append_ptr(a_trg,"null");
}/*}}}*/

static inline void loc_s_blank_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  (void)a_json_nice;

  loc_s_blank_to_json(this,a_trg);
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

#if OPTION_TO_JSON == ENABLED
static inline void loc_s_int_to_json(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_integer);

  lli_to_json(&this->v_data.lli,a_trg);
}/*}}}*/

static inline void loc_s_int_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  (void)a_json_nice;

  loc_s_int_to_json(this,a_trg);
}/*}}}*/
#endif

static inline long long int loc_s_int_value(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_integer);

  return this->v_data.lli;
}/*}}}*/

static inline long long int *loc_s_int_value_ptr(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_integer);

  return &this->v_data.lli;
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

#if OPTION_TO_JSON == ENABLED
static inline void loc_s_float_to_json(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_float);

  bd_to_json(&this->v_data.bd,a_trg);
}/*}}}*/

static inline void loc_s_float_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  (void)a_json_nice;

  loc_s_float_to_json(this,a_trg);
}/*}}}*/
#endif

static inline double loc_s_float_value(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_float);

  return this->v_data.bd;
}/*}}}*/

static inline double *loc_s_float_value_ptr(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_float);

  return &this->v_data.bd;
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

#if OPTION_TO_JSON == ENABLED
static inline void loc_s_string_to_json(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_string);

  string_s *string = (string_s *)this->v_data.ptr;

  string_s_to_json(string,a_trg);
}/*}}}*/

static inline void loc_s_string_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  (void)a_json_nice;

  loc_s_string_to_json(this,a_trg);
}/*}}}*/
#endif

static inline const string_s *loc_s_string_value(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_string);

  return (string_s *)this->v_data.ptr;
}/*}}}*/

// - type ARRAY -
static inline var_s loc_s_array(void)
{/*{{{*/
  var_array_s *array = (var_array_s *)cmalloc(sizeof(var_array_s));
  var_array_s_init(array);

  var_s var = loc_s___new();
  var->v_type = c_bi_type_array;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.ptr = array;

  return var;
}/*}}}*/

static inline var_s loc_s_array___locs(unsigned a_count,...)
{/*{{{*/
  var_array_s *array = (var_array_s *)cmalloc(sizeof(var_array_s));
  var_array_s_init(array);

  va_list ap;
  va_start(ap,a_count);
  var_array_s_push_locs_ap(array,a_count,ap);
  va_end(ap);

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

#if OPTION_TO_JSON == ENABLED
static inline void loc_s_array_to_json(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_array);

  var_array_s *array = (var_array_s *)this->v_data.ptr;

  var_array_s_to_json(array,a_trg);
}/*}}}*/

static inline void loc_s_array_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_array);

  var_array_s *array = (var_array_s *)this->v_data.ptr;

  var_array_s_to_json_nice(array,a_json_nice,a_trg);
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
static inline var_s loc_s_dict(void)
{/*{{{*/
  var_map_tree_s *tree = (var_map_tree_s *)cmalloc(sizeof(var_map_tree_s));
  var_map_tree_s_init(tree);

  var_s var = loc_s___new();
  var->v_type = c_bi_type_dict;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.ptr = tree;

  return var;
}/*}}}*/

static inline var_s loc_s_dict___locs(unsigned a_count,...)
{/*{{{*/
  var_map_tree_s *tree = (var_map_tree_s *)cmalloc(sizeof(var_map_tree_s));
  var_map_tree_s_init(tree);

  va_list ap;
  va_start(ap,a_count);
  var_map_tree_s_set_locs_ap(tree,a_count,ap);
  va_end(ap);

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

#if OPTION_TO_JSON == ENABLED
static inline void loc_s_dict_to_json(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_dict);

  var_map_tree_s *tree = (var_map_tree_s *)this->v_data.ptr;

  var_map_tree_s_to_json(tree,a_trg);
}/*}}}*/

static inline void loc_s_dict_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_dict);

  var_map_tree_s *tree = (var_map_tree_s *)this->v_data.ptr;

  var_map_tree_s_to_json_nice(tree,a_json_nice,a_trg);
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

static inline var_s loc_s_dict_first_key(var_s this)
{/*{{{*/
  debug_assert(this->v_type == c_bi_type_dict)

  var_map_tree_s *tree = (var_map_tree_s *)this->v_data.ptr;

  if (tree->root_idx != c_idx_not_exist)
  {
    unsigned index = var_map_tree_s_get_min_value_idx(tree,tree->root_idx);

    return (tree->data + index)->object.key;
  }

  return NULL;
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

static inline void loc_s_dict_str_set(var_s this,const char *a_str_key,var_s a_value)
{/*{{{*/
  string_s string = {strlen(a_str_key) + 1,(char *)a_str_key};
  loc_s loc = {c_bi_type_string,{0},{.ptr = &string}};

  loc_s_dict_set(this,&loc,a_value);
  debug_assert(loc.v_ref_cnt.counter == 0);
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

static inline var_s loc_s_dict_str_get(var_s this,const char *a_str_key)
{/*{{{*/
  string_s string = {strlen(a_str_key) + 1,(char *)a_str_key};
  loc_s loc = {c_bi_type_string,{0},{.ptr = &string}};

  return loc_s_dict_get(this,&loc);
}/*}}}*/

// === inline from_var methods for basic data types ============================

#if OPTION_FROM_VAR == ENABLED

#define INTEGER_FROM_VAR_INLINE(TYPE) \
static inline int TYPE ## _from_var(TYPE *this,var_s a_var)\
{/*{{{*/\
  if (a_var == NULL || a_var->v_type != c_bi_type_integer)\
  {\
    throw_error(FROM_VAR_ERROR);\
  }\
\
  *this = loc_s_int_value(a_var);\
\
  return 0;\
}/*}}}*/

INTEGER_FROM_VAR_INLINE(bc);
INTEGER_FROM_VAR_INLINE(uc);
INTEGER_FROM_VAR_INLINE(si);
INTEGER_FROM_VAR_INLINE(usi);
INTEGER_FROM_VAR_INLINE(bi);
INTEGER_FROM_VAR_INLINE(ui);
INTEGER_FROM_VAR_INLINE(lli);
INTEGER_FROM_VAR_INLINE(ulli);

#define FLOAT_FROM_VAR_INLINE(TYPE) \
static inline int TYPE ## _from_var(TYPE *this,var_s a_var)\
{/*{{{*/\
  if (a_var == NULL)\
  {\
    throw_error(FROM_VAR_ERROR);\
  }\
\
  switch (a_var->v_type)\
  {\
  case c_bi_type_integer:\
    *this = loc_s_int_value(a_var);\
    break;\
  case c_bi_type_float:\
    *this = loc_s_float_value(a_var);\
    break;\
  default:\
    throw_error(FROM_VAR_ERROR);\
  }\
\
  return 0;\
}/*}}}*/

FLOAT_FROM_VAR_INLINE(bf);
FLOAT_FROM_VAR_INLINE(bd);

static inline int string_s_from_var(string_s *this,var_s a_var)
{/*{{{*/
  string_s_clear(this);

  if (a_var == NULL || a_var->v_type != c_bi_type_string)
  {
    throw_error(FROM_VAR_ERROR);
  }

  string_s_copy(this,loc_s_string_value(a_var));

  return 0;
}/*}}}*/

#endif

#endif

