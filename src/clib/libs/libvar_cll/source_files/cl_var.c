
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

#if OPTION_TO_JSON == ENABLED
loc_s___to_json_t g_loc_s___to_json[LOC_S_MAX_TYPES] =
{/*{{{*/
  loc_s_blank_to_json,
  loc_s_int_to_json,
  loc_s_float_to_json,
  loc_s_string_to_json,
  loc_s_array_to_json,
  loc_s_dict_to_json,
};/*}}}*/

loc_s___to_json_nice_t g_loc_s___to_json_nice[LOC_S_MAX_TYPES] =
{/*{{{*/
  loc_s_blank_to_json_nice,
  loc_s_int_to_json_nice,
  loc_s_float_to_json_nice,
  loc_s_string_to_json_nice,
  loc_s_array_to_json_nice,
  loc_s_dict_to_json_nice,
};/*}}}*/
#endif

unsigned loc_s_register_type(
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
  )
{/*{{{*/
  cassert(g_loc_s_type_cnt < LOC_S_MAX_TYPES);

  g_loc_s___clear[g_loc_s_type_cnt] = a_clear;
  g_loc_s___order[g_loc_s_type_cnt] = a_order;
#if OPTION_TO_STRING == ENABLED
  g_loc_s___to_string[g_loc_s_type_cnt] = a_to_string;
#endif
#if OPTION_TO_JSON == ENABLED
  g_loc_s___to_json[g_loc_s_type_cnt] = a_to_json;
  g_loc_s___to_json_nice[g_loc_s_type_cnt] = a_to_json_nice;
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

#if OPTION_TO_JSON == ENABLED
void var_map_tree_s_to_json(const var_map_tree_s *this,bc_array_s *a_trg)
{/*{{{*/
  if (this->root_idx != c_idx_not_exist)
  {
    bc_array_s_push(a_trg,'{');

    unsigned stack[RB_TREE_STACK_SIZE(var_map_tree_s,this)];
    unsigned *stack_ptr = stack;

    unsigned idx = var_map_tree_s_get_stack_min_value_idx(this,this->root_idx,&stack_ptr);
    do {
      var_map_s *var_map = &(this->data + idx)->object;
      debug_assert(var_map->key->v_type == c_bi_type_string);

      var_s_to_json(&var_map->key,a_trg);
      bc_array_s_push(a_trg,':');
      var_s_to_json(&var_map->value,a_trg);

      idx = var_map_tree_s_get_stack_next_idx(this,idx,&stack_ptr,stack);
      if (idx == c_idx_not_exist)
      {
        break;
      }

      bc_array_s_push(a_trg,',');
    } while(1);

    bc_array_s_push(a_trg,'}');
  }
  else
  {
    bc_array_s_append(a_trg,2,"{}");
  }
}/*}}}*/

void var_map_tree_s_to_json_nice(const var_map_tree_s *this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  if (this->root_idx != c_idx_not_exist)
  {
    bc_array_s_push(a_trg,'{');
    json_nice_s_push_indent(a_json_nice,a_trg);

    unsigned stack[RB_TREE_STACK_SIZE(var_map_tree_s,this)];
    unsigned *stack_ptr = stack;

    unsigned idx = var_map_tree_s_get_stack_min_value_idx(this,this->root_idx,&stack_ptr);
    do {
      var_map_s *var_map = &(this->data + idx)->object;
      debug_assert(var_map->key->v_type == c_bi_type_string);

      var_s_to_json_nice(&var_map->key,a_json_nice,a_trg);
      bc_array_s_push(a_trg,':');
      bc_array_s_push(a_trg,' ');
      var_s_to_json_nice(&var_map->value,a_json_nice,a_trg);

      idx = var_map_tree_s_get_stack_next_idx(this,idx,&stack_ptr,stack);
      if (idx == c_idx_not_exist)
      {
        break;
      }

      bc_array_s_push(a_trg,',');
      json_nice_s_indent(a_json_nice,a_trg);
    } while(1);

    json_nice_s_pop_indent(a_json_nice,a_trg);
    bc_array_s_push(a_trg,'}');
  }
  else
  {
    bc_array_s_append(a_trg,2,"{}");
  }
}/*}}}*/
#endif

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

