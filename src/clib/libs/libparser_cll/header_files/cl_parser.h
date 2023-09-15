
#ifndef CL_LIB_PARSER_H
#define CL_LIB_PARSER_H

@begin
include "cl_parser_code.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libparser_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libparser_cll_EXPORTS
#define libparser_cll_EXPORT __declspec(dllexport)
#else
#define libparser_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_FINAL_AUTOMATA_REGULAR_EXPRESSIONS_EMPTY 1
#define ERROR_FINAL_AUTOMATA_REGULAR_EXPRESSION_PARSE_ERROR 2
#define ERROR_FINAL_AUTOMATA_INVALID_TERMINAL_STRING_RANGE 3
#define ERROR_PARSER_PARSE_UNRECOGNIZED_TERMINAL 4
#define ERROR_PARSER_PARSE_SYNTAX_ERROR 5
#define ERROR_PARSER_PARSE_STATE_OUTSIDE_OF_REDUCE_CALLBACK 6
#define ERROR_PARSER_PARSE_STATE_INDEX_EXCEEDS_RULE_BODY_SIZE 7

// === definition of generated structures ======================================

// -- parser_fa_s --
@begin
struct
<
finite_automata_s:fa
>
parser_fa_s;
@end

WUR libparser_cll_EXPORT int parser_fa_s_create(parser_fa_s *this,
    const string_array_s *a_reg_exps);
WUR static inline unsigned parser_fa_s_recognize(parser_fa_s *this,
    char *input,unsigned *input_idx,unsigned input_length);

// -- ui_tree_s --
@begin
safe_rb_tree<ui>
options ( to_json )
ui_tree_s;
@end

// -- ui_tree_array_s --
@begin
array<ui_tree_s> ui_tree_array_s;
@end

// -- inverted_index_s --
@begin
struct
<
fa_states_s:states
ui_tree_array_s:targets
>
inverted_index_s;
@end

void inverted_index_s_update(inverted_index_s *this,
    fa_states_array_s *states_array);
void inverted_index_s_remove_targets(inverted_index_s *this,
    ui_array_s *a_targets);

// -- inverted_indexes_s --
@begin
array<inverted_index_s> inverted_indexes_s;
@end

void inverted_indexes_s_merge(inverted_indexes_s *this,inverted_index_s *a_trg);

// -- inverted_index_tree_s --
@begin
safe_rb_tree<inverted_index_s> inverted_index_tree_s;
@end

// === inline methods of generated structures ==================================

// -- parser_fa_s --
@begin
inlines parser_fa_s
@end

static inline unsigned parser_fa_s_recognize(parser_fa_s *this,
    char *input,unsigned *input_idx,unsigned input_length)
{/*{{{*/
  return finite_automata_s_recognize(&this->fa,input,input_idx,input_length);
}/*}}}*/

// -- ui_tree_s --
@begin
inlines ui_tree_s
@end

static inline int ui_tree_s___compare_value(const ui_tree_s *this,const ui *a_first,const ui *a_second)
{/*{{{*/
  (void)this;

  return *a_first < *a_second ? -1 : *a_first > *a_second ? 1 : 0;
}/*}}}*/

// -- ui_tree_array_s --
@begin
inlines ui_tree_array_s
@end

// -- inverted_index_s --
@begin
inlines inverted_index_s
@end

// -- inverted_indexes_s --
@begin
inlines inverted_indexes_s
@end

// -- inverted_index_tree_s --
@begin
inlines inverted_index_tree_s
@end

static inline int inverted_index_tree_s___compare_value(const inverted_index_tree_s *this,const inverted_index_s *a_first,const inverted_index_s *a_second)
{/*{{{*/
  (void)this;

  unsigned fs_used = a_first->states.used;
  unsigned ss_used = a_second->states.used;

  return fs_used < ss_used ? -1 : fs_used > ss_used ? 1 : 0;
}/*}}}*/

#endif

