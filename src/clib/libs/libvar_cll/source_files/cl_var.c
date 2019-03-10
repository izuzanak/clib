
@begin
include "cl_var.h"
@end

// === of structure loc_s function maps ========================================

unsigned g_loc_s_type_cnt = c_bi_type_count;

loc_s___clear_t g_loc_s___clear[LOC_S_MAX_TYPES] =
{/*{{{*/
  loc_s_blank_clear,
  loc_s_int_clear,
  loc_s_float_clear,
  loc_s_string_clear,
  loc_s_array_clear,
  loc_s_dict_clear,
};/*}}}*/

loc_s___order_t g_loc_s___order[LOC_S_MAX_TYPES] =
{/*{{{*/
  loc_s_blank_order,
  loc_s_int_order,
  loc_s_float_order,
  loc_s_string_order,
  loc_s_array_order,
  loc_s_dict_order,
};/*}}}*/

#if OPTION_TO_STRING == ENABLED
loc_s___to_string_t g_loc_s___to_string[LOC_S_MAX_TYPES] =
{/*{{{*/
  loc_s_blank_to_string,
  loc_s_int_to_string,
  loc_s_float_to_string,
  loc_s_string_to_string,
  loc_s_array_to_string,
  loc_s_dict_to_string,
};/*}}}*/
#endif

unsigned loc_s_register_type(
  loc_s___clear_t a_clear,
  loc_s___order_t a_order,
#if OPTION_TO_STRING == ENABLED
  loc_s___to_string_t a_to_string
#else
  void *a_to_string
#endif
  )
{/*{{{*/
  cassert(g_loc_s_type_cnt < LOC_S_MAX_TYPES);

  g_loc_s___clear[g_loc_s_type_cnt] = a_clear;
  g_loc_s___order[g_loc_s_type_cnt] = a_order;
#if OPTION_TO_STRING == ENABLED
  g_loc_s___to_string[g_loc_s_type_cnt] = a_to_string;
#endif

  return g_loc_s_type_cnt++;
}/*}}}*/

// === methods of generated structures =========================================

// -- var_array_s --
@begin
methods var_array_s
@end

// -- var_arrays_s --
@begin
methods var_arrays_s
@end

// -- var_map_s --
@begin
methods var_map_s
@end

// -- var_map_tree_s --
@begin
methods var_map_tree_s
@end

// === methods of loc_s types ==================================================
//
// - type ARRAY -
int loc_s_array___order(const var_array_s *a_first,const var_array_s *a_second)
{/*{{{*/
  debug_assert(a_first->used == a_second->used);

  int result;

  if (a_first->used != 0)
  {
    var_s *f_ptr = a_first->data;
    var_s *f_ptr_end = f_ptr + a_first->used;
    var_s *s_ptr = a_second->data;

    do {
      result = loc_s_order(*f_ptr,*s_ptr);
      if (result != 0)
      {
        return result;
      }
    } while(++s_ptr,++f_ptr < f_ptr_end);
  }

  return 0;
}/*}}}*/

// - type DICT -
int loc_s_dict___order(const var_map_tree_s *a_first,const var_map_tree_s *a_second)
{/*{{{*/
  debug_assert(a_first->count == a_second->count);

  int result;

  unsigned f_stack[RB_TREE_STACK_SIZE(var_map_tree_s,a_first)];
  unsigned s_stack[RB_TREE_STACK_SIZE(var_map_tree_s,a_second)];

  unsigned *f_stack_ptr = f_stack;
  unsigned *s_stack_ptr = s_stack;

  unsigned ft_idx = var_map_tree_s_get_stack_min_value_idx(a_first,a_first->root_idx,&f_stack_ptr);
  unsigned st_idx = var_map_tree_s_get_stack_min_value_idx(a_second,a_second->root_idx,&s_stack_ptr);
  do {
    result = var_map_s_order(
      &(a_first->data + ft_idx)->object,
      &(a_second->data + st_idx)->object);

    if (result != 0)
    {
      return result;
    }

    ft_idx = var_map_tree_s_get_stack_next_idx(a_first,ft_idx,&f_stack_ptr,f_stack);
    st_idx = var_map_tree_s_get_stack_next_idx(a_second,st_idx,&s_stack_ptr,s_stack);
  } while(ft_idx != c_idx_not_exist);

  return 0;
}/*}}}*/

