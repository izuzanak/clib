
@begin
include "od_odb.h"
@end

// === methods of generated structures =========================================

// -- odb_node_s --
@begin
methods odb_node_s
@end

unsigned g_type_odb_node = c_idx_not_exist;

// -- odb_node_key_s --
@begin
methods odb_node_key_s
@end

// -- odb_node_key_array_s --
@begin
methods odb_node_key_array_s
@end

// -- odb_database_s --
@begin
methods odb_database_s
@end

void odb_database_s_create(odb_database_s *this)
{/*{{{*/
  var_s_copy_loc(&this->node_var,loc_s_odb_node(0,""));
  var_s_copy_loc(&this->data_var,loc_s_dict());
}/*}}}*/

void odb_database_s_set_value(odb_database_s *this,const char *a_path,var_s a_value_var)
{/*{{{*/
  var_s data_var = this->data_var;

  const char *key_ptr = a_path;
  const char *key_end_ptr;
  do {
    key_end_ptr = strchrnul(key_ptr,'/');

    VAR_CLEAR(key_var,loc_s_string(key_end_ptr - key_ptr,key_ptr));

    // - not last key -
    if (*key_end_ptr != '\0')
    {
      // - key does exists -
      if (loc_s_dict_has_key(data_var,key_var))
      {
        var_s next_data_var = loc_s_dict_get(data_var,key_var);

        // - next data_var is not dict -
        if (next_data_var->v_type != c_bi_type_dict)
        {
          var_s dict = loc_s_dict();
          loc_s_dict_set(data_var,key_var,dict);
          data_var = dict;
        }
        else
        {
          data_var = next_data_var;
        }
      }

      // - key does not exist -
      else
      {
        var_s dict = loc_s_dict();
        loc_s_dict_set(data_var,key_var,dict);
        data_var = dict;
      }
    }

    // - last key -
    else
    {
      loc_s_dict_set(data_var,key_var,a_value_var);
      break;
    }

    key_ptr = key_end_ptr + 1;
  } while(1);
}/*}}}*/

void odb_database_s_get_value(odb_database_s *this,const char *a_path,var_s *a_value_var)
{/*{{{*/
  var_s data_var = this->data_var;

  const char *key_ptr = a_path;
  const char *key_end_ptr;
  do {
    key_end_ptr = strchrnul(key_ptr,'/');

    string_s key_str = {key_end_ptr - key_ptr + 1,(char *)key_ptr};
    loc_s key_loc = {c_bi_type_string,{0},{.ptr = &key_str}};

    // - data_var is dictionary, and key does exist -
    if (data_var->v_type == c_bi_type_dict && loc_s_dict_has_key(data_var,&key_loc))
    {
      data_var = loc_s_dict_get(data_var,&key_loc);
    }
    else
    {
      var_s_copy_loc(a_value_var,NULL);
      return;
    }

    key_ptr = key_end_ptr + 1;
  } while(*key_end_ptr != '\0');

  var_s_copy_loc(a_value_var,data_var);
}/*}}}*/

#define ODB_DATABASE_S_ADD_FIND_NODE(NOT_EXIST_CODE) \
/*{{{*/\
  var_s node_var = this->node_var;\
\
  this->path_stack.used = 0;\
\
  const char *key_ptr = a_path;\
  const char *key_end_ptr;\
  do {\
    key_end_ptr = strchrnul(key_ptr,'/');\
\
    VAR_CLEAR(key_var,loc_s_string(key_end_ptr - key_ptr,key_ptr));\
\
    /* - push node and key to stacks - */\
    odb_node_key_s node_key = {node_var,key_var};\
    odb_node_key_array_s_push(&this->path_stack,&node_key);\
\
    odb_node_s *node = loc_s_odb_node_value(node_var);\
\
    /* - key does exists - */\
    if (loc_s_dict_has_key(node->nodes,key_var))\
    {\
      node_var = loc_s_dict_get(node->nodes,key_var);\
    }\
\
    /* - key does not exist - */\
    else\
    {\
      NOT_EXIST_CODE;\
    }\
\
    key_ptr = key_end_ptr + 1;\
  } while(*key_end_ptr != '\0');\
\
  var_s_copy_loc(a_info_var,node_var);\
/*}}}*/

void odb_database_s_add_node(odb_database_s *this,const char *a_path,var_s *a_info_var)
{/*{{{*/
  ODB_DATABASE_S_ADD_FIND_NODE(
    var_s new_node_var = loc_s_odb_node(key_end_ptr - a_path,a_path);
    loc_s_dict_set(node->nodes,key_var,new_node_var);
    node_var = new_node_var;
  );

  var_s_copy_loc(a_info_var,node_var);
}/*}}}*/

void odb_database_s_find_node(odb_database_s *this,const char *a_path,var_s *a_info_var)
{/*{{{*/
  var_s_copy_loc(a_info_var,NULL);

  ODB_DATABASE_S_ADD_FIND_NODE(
    return;
  );
}/*}}}*/

void odb_database_s_remove_node(odb_database_s *this)
{/*{{{*/
  while (this->path_stack.used != 0)
  {
    odb_node_key_s *node_key = odb_node_key_array_s_pop(&this->path_stack);

    odb_node_s *node = loc_s_odb_node_value(node_key->node_var);
    loc_s_dict_remove_key(node->nodes,node_key->key_var);

    // - stop if node is not empty -
    if (node->info != NULL || loc_s_dict_length(node->nodes) != 0)
    {
      break;
    }
  }
}/*}}}*/

void odb_database_s_nodes_path(odb_database_s *this,var_array_s *a_nodes)
{/*{{{*/
  if (this->path_stack.used != 0)
  {
    odb_node_key_s *nk_ptr = this->path_stack.data;
    odb_node_key_s *nk_ptr_end = nk_ptr + this->path_stack.used;
    do {
      if (loc_s_odb_node_value(nk_ptr->node_var)->info != NULL)
      {
        var_array_s_push(a_nodes,&nk_ptr->node_var);
      }
    } while(++nk_ptr < nk_ptr_end);
  }
}/*}}}*/

void odb_database_s_nodes_tree(var_s a_node_var,var_array_s *a_nodes)
{/*{{{*/
  odb_node_s *node = loc_s_odb_node_value(a_node_var);

  if (node->info != NULL)
  {
    var_array_s_push(a_nodes,&a_node_var);
  }

  var_map_tree_s *tree = loc_s_dict_value(node->nodes);
  if (tree->root_idx != c_idx_not_exist)
  {
    var_map_tree_s_node *ptr = tree->data;
    var_map_tree_s_node *ptr_end = ptr + tree->used;
    do {
      if (ptr->valid)
      {
        odb_database_s_nodes_tree(ptr->object.value,a_nodes);
      }
    } while(++ptr < ptr_end);
  }
}/*}}}*/

// === global functions ========================================================

void libodb_odl_init()
{/*{{{*/

  // - loc_s_register_type -
  g_type_odb_node = loc_s_register_type(
    loc_s_odb_node_clear,
    loc_s_odb_node_order,
#if OPTION_TO_STRING == ENABLED
    loc_s_odb_node_to_string,
#else
    NULL,
#endif
#if OPTION_TO_JSON == ENABLED
    loc_s_odb_node_to_json,
    loc_s_odb_node_to_json_nice
#else
    NULL,
    NULL
#endif
    );

}/*}}}*/

void libodb_odl_clear()
{/*{{{*/
}/*}}}*/

