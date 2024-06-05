
@begin
include "cl_var.h"
@end

// === methods of structure loc_s ==============================================

var_s loc_s_at_part_path(var_s this,const char *a_path,const char **a_rest)
{/*{{{*/
  var_s data_var = this;

  if (a_path != NULL && a_path[0] != '\0')
  {
    const char *key_ptr = a_path;
    const char *key_end_ptr;
    do {
      key_end_ptr = strchrnul(key_ptr,'/');

      switch (data_var->v_type)
      {
      case c_bi_type_dict:
        {/*{{{*/
          string_s key_str = {key_end_ptr - key_ptr + 1,(char *)key_ptr};
          loc_s key_loc = {c_bi_type_string,{0},{.ptr = &key_str}};

          if (loc_s_dict_has_key(data_var,&key_loc))
          {
            data_var = loc_s_dict_get(data_var,&key_loc);
          }
          else
          {
            *a_rest = key_ptr;
            return data_var;
          }
        }/*}}}*/
        break;
      case c_bi_type_array:
        {/*{{{*/
          char *end_ptr;
          lli index = strtoll(key_ptr,&end_ptr,10);

          if (end_ptr == key_end_ptr && index < loc_s_array_length(data_var))
          {
            data_var = *loc_s_array_at(data_var,index);
          }
          else
          {
            *a_rest = key_ptr;
            return data_var;
          }
        }/*}}}*/
        break;
      default:
        {/*{{{*/
          *a_rest = key_ptr;
          return data_var;
        }/*}}}*/
      }

      key_ptr = key_end_ptr + 1;
    } while(*key_end_ptr != '\0');
  }

  *a_rest = NULL;
  return data_var;
}/*}}}*/

// === structure loc_s function maps ===========================================

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
#else
  (void)a_to_string;
#endif
#if OPTION_TO_JSON == ENABLED
  g_loc_s___to_json[g_loc_s_type_cnt] = a_to_json;
  g_loc_s___to_json_nice[g_loc_s_type_cnt] = a_to_json_nice;
#else
  (void)a_to_json;
  (void)a_to_json_nice;
#endif

  return g_loc_s_type_cnt++;
}/*}}}*/

// === methods of generated structures =========================================

// -- var_array_s --
@begin
methods var_array_s
@end

void var_array_s_push_locs_ap(var_array_s *this,unsigned a_count,va_list a_ap)
{/*{{{*/
  while (a_count-- > 0)
  {
    var_array_s_push_loc(this,va_arg(a_ap,var_s));
  }
}/*}}}*/

// -- var_arrays_s --
@begin
methods var_arrays_s
@end

// -- var_queue_s --
@begin
methods var_queue_s
@end

// -- var_tree_s --
@begin
methods var_tree_s
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
  bc_array_s_push(a_trg,'{');

  if (this->root_idx != c_idx_not_exist)
  {
    var_map_tree_s_node *ptr = this->data;
    var_map_tree_s_node *ptr_end = ptr + this->used;
    int comma = 0;
    do {
      if (ptr->valid)
      {
        var_map_s *var_map = &ptr->object;
        debug_assert(var_map->key->v_type == c_bi_type_string);

        if (comma)
        {
          bc_array_s_push(a_trg,',');
        }
        else
        {
          comma = 1;
        }

        var_s_to_json(&var_map->key,a_trg);
        bc_array_s_push(a_trg,':');
        var_s_to_json(&var_map->value,a_trg);
      }
    } while(++ptr < ptr_end);
  }

  bc_array_s_push(a_trg,'}');
}/*}}}*/

void var_map_tree_s_to_json_nice(const var_map_tree_s *this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  if (this->root_idx != c_idx_not_exist)
  {
    bc_array_s_push(a_trg,'{');
    json_nice_s_push_indent(a_json_nice,a_trg);

    var_map_tree_s_node *ptr = this->data;
    var_map_tree_s_node *ptr_end = ptr + this->used;
    int comma = 0;
    do {
      if (ptr->valid)
      {
        var_map_s *var_map = &ptr->object;
        debug_assert(var_map->key->v_type == c_bi_type_string);

        if (comma)
        {
          bc_array_s_push(a_trg,',');
          json_nice_s_indent(a_json_nice,a_trg);
        }
        else
        {
          comma = 1;
        }

        var_s_to_json_nice(&var_map->key,a_json_nice,a_trg);
        bc_array_s_push(a_trg,':');
        bc_array_s_push(a_trg,' ');
        var_s_to_json_nice(&var_map->value,a_json_nice,a_trg);
      }
    } while(++ptr < ptr_end);

    json_nice_s_pop_indent(a_json_nice,a_trg);
    bc_array_s_push(a_trg,'}');
  }
  else
  {
    bc_array_s_push(a_trg,'{');
    bc_array_s_push(a_trg,'}');
  }
}/*}}}*/
#endif

void var_map_tree_s_set_locs_ap(var_map_tree_s *this,unsigned a_count,va_list a_ap)
{/*{{{*/
  debug_assert(!(a_count & 0x01));

  while (a_count > 0)
  {
    var_s key_var = va_arg(a_ap,var_s);
    var_s value_var = va_arg(a_ap,var_s);

    unsigned index = var_map_tree_s_unique_insert(this,
        &(var_map_s){key_var,value_var});

    var_map_s *map = &(this->data + index)->object;

    if (map->value != value_var)
    {
      VAR_CLEAR(temp,key_var);
      var_s_copy_loc(&map->value,value_var);
    }

    a_count -= 2;
  }
}/*}}}*/

// === methods of loc_s types ==================================================

// - type ARRAY -
int loc_s_array___order(const var_array_s *a_first,const var_array_s *a_second)
{/*{{{*/
  debug_assert(a_first->used == a_second->used);

  int res;

  if (a_first->used != 0)
  {
    var_s *f_ptr = a_first->data;
    var_s *f_ptr_end = f_ptr + a_first->used;
    var_s *s_ptr = a_second->data;

    do {
      res = loc_s_order(*f_ptr,*s_ptr);
      if (res != 0)
      {
        return res;
      }
    } while(++s_ptr,++f_ptr < f_ptr_end);
  }

  return 0;
}/*}}}*/

// - type DICT -
int loc_s_dict___order(const var_map_tree_s *a_first,const var_map_tree_s *a_second)
{/*{{{*/
  debug_assert(a_first->count == a_second->count);

  int res;

  if (a_first->count != 0)
  {
    unsigned f_stack[RB_TREE_STACK_SIZE(var_map_tree_s,a_first)];
    unsigned s_stack[RB_TREE_STACK_SIZE(var_map_tree_s,a_second)];

    unsigned *f_stack_ptr = f_stack;
    unsigned *s_stack_ptr = s_stack;

    unsigned ft_idx = var_map_tree_s_get_stack_min_value_idx(a_first,a_first->root_idx,&f_stack_ptr);
    unsigned st_idx = var_map_tree_s_get_stack_min_value_idx(a_second,a_second->root_idx,&s_stack_ptr);
    do {
      res = var_map_s_order(
        &(a_first->data + ft_idx)->object,
        &(a_second->data + st_idx)->object);

      if (res != 0)
      {
        return res;
      }

      ft_idx = var_map_tree_s_get_stack_next_idx(a_first,ft_idx,&f_stack_ptr,f_stack);
      st_idx = var_map_tree_s_get_stack_next_idx(a_second,st_idx,&s_stack_ptr,s_stack);
    } while(ft_idx != c_idx_not_exist);
  }

  return 0;
}/*}}}*/

