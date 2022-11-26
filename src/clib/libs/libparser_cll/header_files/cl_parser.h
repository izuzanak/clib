
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

#endif

