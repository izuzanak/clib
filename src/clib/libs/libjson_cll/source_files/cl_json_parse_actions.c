
@begin
include "cl_json.h"
include "cl_json_parse_actions.h"
@end

// === json parse actions access ===============================================

#define c_json_parse_action_cnt 24
void (*json_pa_callers[c_json_parse_action_cnt])(json_parser_s *this) =
{/*{{{*/
  pa_json_null,
  pa_json_null,
  pa_json_null,
  pa_json_null,
  pa_json_null,
  pa_json_object_begin,
  pa_json_null,
  pa_json_null,
  pa_json_object_pair,
  pa_json_null,
  pa_json_null,
  pa_json_null,
  pa_json_array_begin,
  pa_json_array_value,
  pa_json_array_value,
  pa_json_val_string,
  pa_json_val_integer,
  pa_json_val_float,
  pa_json_val_object,
  pa_json_val_array,
  pa_json_val_true,
  pa_json_val_false,
  pa_json_val_null,
  pa_json_string,
};/*}}}*/

// === json parse actions ======================================================

void pa_json_null(json_parser_s *this)
{/*{{{*/
  (void)this;

  debug_assert(0);
}/*}}}*/

void pa_json_object_begin(json_parser_s *this)
{/*{{{*/
  var_array_s *objects = &this->objects;

  // *****

  var_array_s_push_loc(objects,loc_s_dict());
}/*}}}*/

void pa_json_object_pair(json_parser_s *this)
{/*{{{*/
  var_array_s *string_vars = &this->string_vars;
  ui_array_s *string_idxs = &this->string_idxs;
  var_array_s *values = &this->values;
  var_array_s *objects = &this->objects;

  // *****

  // - retrieve key and value variables -
  var_s key_var = *var_array_s_at(string_vars,ui_array_s_pop(string_idxs));
  var_s value_var = *var_array_s_pop(values);

  // - retrieve object variable -
  var_s obj_var = *var_array_s_last(objects);

  // - set object key value pair -
  loc_s_dict_set(obj_var,key_var,value_var);
}/*}}}*/

void pa_json_array_begin(json_parser_s *this)
{/*{{{*/
  var_array_s *arrays = &this->arrays;

  // *****

  var_array_s_push_loc(arrays,loc_s_array());
}/*}}}*/

void pa_json_array_value(json_parser_s *this)
{/*{{{*/
  var_array_s *values = &this->values;
  var_array_s *arrays = &this->arrays;

  // *****

  var_s array_var = *var_array_s_last(arrays);
  var_s value_var = *var_array_s_pop(values);

  // - push value to array -
  loc_s_array_push(array_var,value_var);
}/*}}}*/

void pa_json_val_string(json_parser_s *this)
{/*{{{*/
  var_array_s *string_vars = &this->string_vars;
  ui_array_s *string_idxs = &this->string_idxs;
  var_array_s *values = &this->values;

  // *****

  var_s string_var = *var_array_s_at(string_vars,ui_array_s_pop(string_idxs));

  // - insert string variable to values -
  var_array_s_push(values,&string_var);
}/*}}}*/

void pa_json_val_integer(json_parser_s *this)
{/*{{{*/
  const bc_array_s *source = (const bc_array_s *)this->source;
  lli_tree_s *const_integers = &this->const_integers;
  var_array_s *integer_vars = &this->integer_vars;
  var_array_s *values = &this->values;
  lalr_stack_s *lalr_stack = &this->lalr_stack;

  // *****

  lalr_stack_element_s *lse = lalr_stack_s_last(lalr_stack);

  // - retrieve number from source -
  long long int const_int = strtoll(source->data + lse->terminal_start,NULL,10);

  // - get constant position in array -
  unsigned cd_idx = lli_tree_s_unique_insert(const_integers,const_int);

  if (cd_idx >= integer_vars->used)
  {
    // - skip rb_tree indexes gap -
    while (cd_idx > integer_vars->used)
    {
      var_array_s_push_blank(integer_vars);
    }

    // - create new integer variable -
    var_array_s_push_loc(integer_vars,loc_s_int(const_int));
  }

  var_s integer_var = *var_array_s_at(integer_vars,cd_idx);

  // - insert integer variable to values -
  var_array_s_push(values,&integer_var);
}/*}}}*/

void pa_json_val_float(json_parser_s *this)
{/*{{{*/
  const bc_array_s *source = (const bc_array_s *)this->source;
  bd_tree_s *const_floats = &this->const_floats;
  var_array_s *float_vars = &this->float_vars;
  var_array_s *values = &this->values;
  lalr_stack_s *lalr_stack = &this->lalr_stack;

  // *****

  lalr_stack_element_s *lse = lalr_stack_s_last(lalr_stack);

  // - retrieve number from source -
  double const_float = strtod(source->data + lse->terminal_start,NULL);

  // - get constant position in array -
  unsigned cd_idx = bd_tree_s_unique_insert(const_floats,const_float);

  if (cd_idx >= float_vars->used)
  {
    // - skip rb_tree indexes gap -
    while (cd_idx > float_vars->used)
    {
      var_array_s_push_blank(float_vars);
    }

    // - create new float variable -
    var_array_s_push_loc(float_vars,loc_s_float(const_float));
  }

  var_s float_var = *var_array_s_at(float_vars,cd_idx);

  // - insert float variable to values -
  var_array_s_push(values,&float_var);
}/*}}}*/

void pa_json_val_object(json_parser_s *this)
{/*{{{*/
  var_array_s *values = &this->values;
  var_array_s *objects = &this->objects;

  // *****

  var_array_s_push(values,var_array_s_pop(objects));
}/*}}}*/

void pa_json_val_array(json_parser_s *this)
{/*{{{*/
  var_array_s *values = &this->values;
  var_array_s *arrays = &this->arrays;

  // *****

  var_array_s_push(values,var_array_s_pop(arrays));
}/*}}}*/

void pa_json_val_true(json_parser_s *this)
{/*{{{*/
  var_array_s *values = &this->values;

  // *****

  if (this->true_var == NULL)
  {
    var_s_copy_loc(&this->true_var,loc_s_int(1));
  }

  var_array_s_push(values,&this->true_var);
}/*}}}*/

void pa_json_val_false(json_parser_s *this)
{/*{{{*/
  var_array_s *values = &this->values;

  // *****

  if (this->false_var == NULL)
  {
    var_s_copy_loc(&this->false_var,loc_s_int(0));
  }

  var_array_s_push(values,&this->false_var);
}/*}}}*/

void pa_json_val_null(json_parser_s *this)
{/*{{{*/
  var_array_s *values = &this->values;

  // *****

  if (this->null_var == NULL)
  {
    var_s_copy_loc(&this->null_var,loc_s_blank());
  }

  var_array_s_push(values,&this->null_var);
}/*}}}*/

void pa_json_string(json_parser_s *this)
{/*{{{*/
  const bc_array_s *source = (const bc_array_s *)this->source;
  string_tree_s *const_strings = &this->const_strings;
  var_array_s *string_vars = &this->string_vars;
  ui_array_s *string_idxs = &this->string_idxs;
  lalr_stack_s *lalr_stack = &this->lalr_stack;

  // *****

  lalr_stack_element_s *lse = lalr_stack_s_last(lalr_stack);

  char *ptr = source->data + lse->terminal_start + 1;
  char *ptr_end = source->data + lse->terminal_end - 1;

  bc_array_s char_buffer;
  bc_array_s_init_size(&char_buffer,(ptr_end - ptr) + 1);

  json_parser_s_process_json_string(ptr,ptr_end,&char_buffer);

  string_s const_str;
  const_str.size = char_buffer.used;
  const_str.data = char_buffer.data;

  // - get constant position in array -
  unsigned cs_idx = string_tree_s_unique_swap_insert(const_strings,&const_str);
  string_s_clear(&const_str);

  if (cs_idx >= string_vars->used)
  {
    // - skip rb_tree indexes gap -
    while (cs_idx > string_vars->used)
    {
      var_array_s_push_blank(string_vars);
    }

    // - retrieve string -
    string_s *string = string_tree_s_at(const_strings,cs_idx);

    // - create new string variable -
    var_array_s_push_loc(string_vars,loc_s_string_str(string));
  }

  // - store string index -
  ui_array_s_push(string_idxs,cs_idx);
}/*}}}*/

