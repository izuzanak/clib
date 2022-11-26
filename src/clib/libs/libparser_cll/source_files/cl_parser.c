
@begin
include "cl_parser.h"
@end

// === methods of generated structures =========================================

// -- parser_fa_s --
@begin
methods parser_fa_s
@end

int parser_fa_s_create(parser_fa_s *this, const string_array_s *a_reg_exps)
{/*{{{*/
  parser_fa_s_clear(this);

  // - ERROR -
  if (a_reg_exps->used == 0)
  {
    throw_error(FINAL_AUTOMATA_REGULAR_EXPRESSIONS_EMPTY);
  }

  CONT_INIT_CLEAR(fa_states_array_s,states_array);
  CONT_INIT_CLEAR(reg_parser_s,reg_parser);

  string_s *re_ptr = a_reg_exps->data;
  string_s *re_ptr_end = re_ptr + a_reg_exps->used;
  do {

    // - ERROR -
    if (!reg_parser_s_process_reg_exp(&reg_parser,re_ptr))
    {
      throw_error(FINAL_AUTOMATA_REGULAR_EXPRESSION_PARSE_ERROR);
    }

    fa_states_array_s_push_blank(&states_array);
    fa_states_s_swap(fa_states_array_s_last(&states_array),&reg_parser.states);

  } while(++re_ptr < re_ptr_end);


  finite_automata_s_create_new(&this->fa,&states_array);

  return 0;
}/*}}}*/

