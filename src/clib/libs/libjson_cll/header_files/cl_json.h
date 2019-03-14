
#ifndef __JSON_H
#define __JSON_H

@begin
include "cl_var.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libjson_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libjson_cll_EXPORTS
#define libjson_cll_EXPORT __declspec(dllexport)
#else
#define libjson_cll_EXPORT __declspec(dllimport)
#endif
#endif

// === constants and definitions ===============================================

#define max_number_string_length 128

#define c_json_rule_cnt 24
extern const unsigned json_rule_head_idxs[c_json_rule_cnt];
extern const unsigned json_rule_body_lengths[c_json_rule_cnt];

#define json_blank c_idx_not_exist
#define JSON_SHIFT(VALUE) VALUE
#define JSON_REDUCE(VALUE) c_json_lalr_table_reduce_base + VALUE
#define JSON_GOTO(VALUE) VALUE

#define c_json_lalr_table_reduce_base 0x80000000
#define c_json_terminal_plus_nonterminal_cnt 30
#define json_lalr_state_cnt 32

extern const unsigned json_lalr_table[json_lalr_state_cnt*c_json_terminal_plus_nonterminal_cnt];

// - error codes -
#define ERROR_JSON_PARSE_UNRECOGNIZED_TERMINAL 1
#define ERROR_JSON_PARSE_INVALID_SYNTAX 2

#define ERROR_JSON_CREATE_NO_STRING_DICT_KEY 1
#define ERROR_JSON_CREATE_UNSUPPORTED_TYPE 2

// === definition of generated structures ======================================

// -- lli_tree_s --
@begin
rb_tree<lli> lli_tree_s;
@end

// -- bd_tree_s --
@begin
rb_tree<bd> bd_tree_s;
@end

// -- json_parser_s --
@begin
struct
<
pointer:source
ui:input_idx
ui:old_input_idx

lli_tree_s:const_integers
bd_tree_s:const_floats
string_tree_s:const_strings

var_s:null_var
var_s:true_var
var_s:false_var

var_array_s:integer_vars
var_array_s:float_vars
var_array_s:string_vars

ui_array_s:string_idxs

var_array_s:values
var_array_s:arrays
var_array_s:objects

lalr_stack_s:lalr_stack
>
json_parser_s;
@end

unsigned json_parser_s_recognize_terminal(json_parser_s *this);
WUR libjson_cll_EXPORT int json_parser_s_parse(json_parser_s *this,const bc_array_s *a_src,var_s *a_trg);

// -- json_create_stack_element_s --
@begin
struct
<
var_s:var
bi:initialize
ui:index
>
json_create_stack_element_s;
@end

// -- json_create_stack_s --
@begin
array<json_create_stack_element_s> json_create_stack_s;
@end

// === definition of global functions ==========================================

void json_create_append_string(const string_s *a_src,bc_array_s *a_trg);
WUR libjson_cll_EXPORT int json_create(var_s a_obj,bc_array_s *a_trg);
WUR libjson_cll_EXPORT int json_create_nice(var_s a_obj,const string_s
    *a_tabulator,const string_s *a_indent,bc_array_s *a_trg);
WUR static inline int json_parse(const bc_array_s *a_src,var_s *a_trg);

// === inline methods of generated structures ==================================

// -- lli_tree_s --
@begin
inlines lli_tree_s
@end

static inline int lli_tree_s___compare_value(const lli_tree_s *this,const lli *a_first,const lli *a_second)
{/*{{{*/
  (void)this;

  return *a_first < *a_second ? -1 : *a_first > *a_second ? 1 : 0;
}/*}}}*/

// -- bd_tree_s --
@begin
inlines bd_tree_s
@end

static inline int bd_tree_s___compare_value(const bd_tree_s *this,const bd *a_first,const bd *a_second)
{/*{{{*/
  (void)this;

  return *a_first < *a_second ? -1 : *a_first > *a_second ? 1 : 0;
}/*}}}*/

// -- json_parser_s --
@begin
inlines json_parser_s
@end

// -- json_create_stack_element_s --
@begin
inlines json_create_stack_element_s
@end

// -- json_create_stack_s --
@begin
inlines json_create_stack_s
@end

// === inline global functions =================================================

static inline int json_parse(const bc_array_s *a_src,var_s *a_trg)
{/*{{{*/
  CONT_INIT(json_parser_s,json_parser);
  int res = json_parser_s_parse(&json_parser,a_src,a_trg);
  json_parser_s_clear(&json_parser);
  return res;
}/*}}}*/

#endif

