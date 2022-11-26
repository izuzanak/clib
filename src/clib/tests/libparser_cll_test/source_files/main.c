
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libparser_cll_test";

const char *test_names[] =
{/*{{{*/
  "finite_automata_create",
  "parser_create",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_finite_automata_create,
  test_parser_create,
};/*}}}*/

// === methods of generated structures =========================================

// === test execution functions ================================================

void test_finite_automata_create()
{/*{{{*/
  CONT_INIT_CLEAR(parser_fa_s,fa);
  string_array_s reg_exps = {3,3,(string_s[]){
    STRING_S("\"Ahoj\""),
    STRING_S("\"Hello\""),
    STRING_S("\"Bonjour\""),
  }};

  cassert(parser_fa_s_create(&fa,&reg_exps) == 0);

  char *input = "BonjourHelloAhojBonjourHelloAhoj";
  unsigned input_len = strlen(input);

  CONT_INIT_CLEAR(ui_array_s,terminals);

  unsigned input_idx = 0;
  do {
    unsigned terminal = parser_fa_s_recognize(&fa,input,&input_idx,input_len);
    if (terminal == c_idx_not_exist) { break; }
    ui_array_s_push(&terminals,terminal);
  } while(1);

  ui_array_s reference = {6,6,(unsigned[]){2,1,0,2,1,0}};
  cassert(ui_array_s_compare(&terminals,&reference));
}/*}}}*/

void test_parser_create()
{/*{{{*/
  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/libparser_cll_test/resources/uclang_parser.rules","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,rules);
  cassert(file_s_read_close(&file,&rules) == 0);

  cassert(file_s_open(&file,"tests/libparser_cll_test/resources/uclang_parser.src","r") == 0);

  CONT_INIT_CLEAR(bc_array_s,source);
  cassert(file_s_read_close(&file,&source) == 0);

  CONT_INIT_CLEAR(parser_s,parser);
  cassert(parser_s_create_from_rule_char_ptr(&parser,rules.used,rules.data) == 1);

  CONT_INIT_CLEAR(ui_array_s,reduce_actions);

  // - initialize lalr parser stack -
  CONT_INIT_CLEAR(lalr_stack_s,lalr_stack);
  lalr_stack_s_push_state_all(&lalr_stack,0,0,0);

  // - variables describing state of lexical automata -
  unsigned old_input_idx = 0;
  unsigned input_idx = 0;
  unsigned ret_term = c_idx_not_exist;
  unsigned parse_action;

  do
  {
    // - recognize next terminal symbol -
    while (ret_term == c_idx_not_exist)
    {
      old_input_idx = input_idx;

      ret_term = finite_automata_s_recognize(&parser.final_automata,source.data,&input_idx,source.used);

      // - ERROR -
      cassert(ret_term != c_idx_not_exist)

      if (ui_array_s_get_idx(&parser.skip_terminals,ret_term) != c_idx_not_exist)
      {
        ret_term = c_idx_not_exist;
      }
    }

    // - select parse action from parse action table -
    parse_action = *p_lalr_table_s_value(&parser.lalr_table,ret_term,lalr_stack_s_last(&lalr_stack)->lalr_state);

    // - ERROR -
    cassert(parse_action != c_idx_not_exist);

    // - action SHIFT -
    if (parse_action < c_lalr_table_reduce_base)
    {
      // - if end terminal was received terminate parsing process -
      if (ret_term == parser.end_terminal)
      {
        break;
      }

      // - insert new parse state and location of terminal to parse stack -
      lalr_stack_s_push_state_all(&lalr_stack,parse_action,old_input_idx,input_idx);

      ret_term = c_idx_not_exist;
    }

    // - action REDUCE -
    else
    {
      parse_action -= c_lalr_table_reduce_base;

      // - store parse action -
      ui_array_s_push(&reduce_actions,parse_action);

      p_rule_descr_s *rule_descr = p_rule_descrs_s_at(&parser.rule_descrs,parse_action);

      // - retrieve nonterminal start and end -
      unsigned nonterm_start = lalr_stack_s_at(&lalr_stack,lalr_stack.used - rule_descr->body_size)->terminal_start;
      unsigned nonterm_end = lalr_stack_s_last(&lalr_stack)->terminal_end;

      lalr_stack.used -= rule_descr->body_size;

      unsigned goto_val = *p_lalr_table_s_value(&parser.lalr_table,rule_descr->head,lalr_stack_s_last(&lalr_stack)->lalr_state);
      lalr_stack_s_push_state_all(&lalr_stack,goto_val,nonterm_start,nonterm_end);
    }
  }
  while(1);

  // - reference reduce actions -
  ui_array_s reference = {1081,1081,(unsigned[]){
    6,6,17,17,16,15,15,10,16,10,16,10,14,9,20,19,28,30,21,
    19,43,48,47,44,53,190,189,187,133,129,151,133,127,151,133,
    127,151,133,127,126,123,119,111,108,96,95,86,55,52,42,37,
    34,33,33,32,31,27,26,25,25,18,8,7,13,9,17,17,16,15,15,
    12,20,19,28,17,17,16,15,15,29,21,19,43,48,47,44,53,151,
    133,129,126,123,119,111,108,16,164,155,170,168,153,133,129,
    126,123,119,111,108,97,96,95,87,55,151,133,129,126,123,119,
    111,108,17,17,16,15,15,164,155,170,190,189,187,133,129,126,
    123,119,111,108,96,172,190,189,187,133,129,126,123,119,111,
    108,96,171,169,153,133,129,126,123,119,111,108,97,96,95,87,
    54,151,133,129,126,123,119,111,108,17,17,16,15,164,163,149,
    149,147,145,144,142,133,129,126,123,119,111,108,97,96,95,87,
    54,151,173,165,173,170,190,189,187,133,129,126,123,119,111,
    108,96,172,169,167,133,129,126,123,119,111,108,96,95,87,54,
    151,133,129,126,123,119,111,108,91,184,133,129,126,123,119,
    111,108,96,93,184,133,129,126,123,119,111,108,96,92,184,133,
    129,126,123,119,111,108,96,92,184,133,129,126,123,119,111,
    108,96,92,184,133,129,126,123,119,111,108,96,92,90,88,133,
    129,126,123,119,111,108,97,96,95,87,54,74,73,151,133,129,
    126,123,119,111,108,96,72,57,56,71,54,74,73,151,133,129,
    126,123,119,111,108,96,72,57,56,71,54,151,133,129,126,123,
    119,111,108,16,164,155,170,91,184,133,129,126,123,119,111,
    108,96,93,184,133,129,126,123,119,111,108,96,92,184,133,129,
    126,123,119,111,108,96,92,184,133,129,126,123,119,111,108,
    96,92,184,133,129,126,123,119,111,108,96,92,90,88,133,129,
    126,123,119,111,108,96,172,169,153,133,129,126,123,119,111,
    108,97,96,95,87,54,184,133,129,126,123,119,151,133,129,126,
    123,118,111,108,96,94,59,190,189,187,133,151,158,132,129,
    126,123,119,111,108,150,173,170,168,167,133,129,126,123,119,
    111,108,96,95,87,55,58,56,65,64,54,175,177,184,133,129,126,
    123,119,111,108,96,95,86,174,173,170,91,89,88,133,129,126,
    123,119,111,108,96,172,169,167,133,129,126,123,119,111,108,
    96,95,87,54,175,48,47,176,184,133,129,126,123,119,111,108,
    96,95,86,174,173,170,91,184,133,129,126,123,119,111,108,96,
    93,90,88,133,129,126,123,119,111,108,96,172,169,167,133,129,
    126,123,119,111,108,96,95,87,54,151,133,129,126,123,119,111,
    108,175,48,47,176,184,133,151,130,129,126,123,119,111,108,
    96,95,86,174,133,129,126,123,119,111,108,97,96,95,87,54,
    151,133,129,126,123,119,111,108,175,48,47,176,59,184,133,
    129,126,123,119,111,108,96,95,86,55,58,56,174,133,129,126,
    123,119,111,108,97,96,95,87,54,151,133,129,126,123,119,111,
    108,175,48,47,48,46,176,59,151,133,129,126,123,119,111,108,
    183,133,129,126,123,119,111,108,97,96,95,87,55,68,151,133,
    129,126,123,119,151,133,129,126,123,114,111,108,96,94,59,
    151,133,129,126,123,119,111,108,151,133,129,126,123,119,111,
    108,98,96,95,87,55,151,136,133,129,126,123,119,111,108,96,
    95,87,54,58,56,67,54,58,56,174,133,129,126,123,119,111,108,
    97,96,95,87,54,74,73,16,164,155,170,183,133,129,126,123,
    119,111,108,96,172,184,133,129,126,123,119,111,108,96,171,
    184,133,129,126,123,119,111,108,96,171,169,153,133,129,126,
    123,119,111,108,96,72,57,56,71,54,151,133,129,126,123,119,
    111,108,190,189,190,188,190,188,187,133,129,126,123,119,111,
    108,97,96,95,87,54,151,133,129,126,123,119,111,108,184,133,
    129,126,123,119,111,108,184,133,129,126,123,119,111,108,96,
    183,133,129,126,123,119,111,108,96,160,97,96,95,87,54,183,
    133,129,126,123,119,111,108,183,133,129,126,123,119,111,108,
    183,133,129,126,123,119,111,108,183,133,129,126,123,119,111,
    108,96,162,96,161,150,133,129,126,123,119,111,108,183,133,
    129,126,123,119,111,108,184,133,129,126,123,119,111,108,96,
    161,96,162,96,161,96,95,87,54,151,133,129,126,123,119,111,
    108,184,133,129,126,123,119,111,108,97,96,95,87,54,190,189,
    187,133,151,133,129,184,133,127,126,123,119,111,108,150,158,
    132,129,126,123,119,111,108,150,173,170,168,167,133,129,126,
    123,119,111,108,96,95,87,54,190,189,187,133,151,158,132,129,
    126,123,119,111,108,150,173,170,168,167,133,129,126,123,119,
    111,108,96,95,87,54,151,133,129,126,123,119,111,108,182,133,
    129,126,123,182,133,129,126,120,119,111,108,97,96,95,87,54,
    52,42,37,34,33,33,32,31,27,26,25,25,18,8,13,11,8,7,13,9,
    8,5,4,4,3}};

  cassert(ui_array_s_compare(&reference,&reduce_actions));
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

