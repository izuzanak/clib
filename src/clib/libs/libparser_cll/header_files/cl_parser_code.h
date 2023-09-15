
#ifndef CL_LIB_PARSER_CODE_H
#define CL_LIB_PARSER_CODE_H

@begin
include "cl_struct.h"
@end

// - error codes -
#define ERROR_PARSER_CREATE_RULES_SYNTAX_ERROR false
#define ERROR_PARSER_CREATE_RULES_DUPLICATE_TERMINAL false
#define ERROR_PARSER_CREATE_RULES_DUPLICATE_NONTERMINAL false
#define ERROR_PARSER_CREATE_RULES_DUPLICATE_RULE false
#define ERROR_PARSER_CREATE_RULES_NO_TERMINALS_DEFINED false
#define ERROR_PARSER_CREATE_RULES_NO_NONTERMINALS_DEFINED false
#define ERROR_PARSER_CREATE_RULES_NO_RULES_DEFINED false
#define ERROR_PARSER_CREATE_RULES_UNDEFINED_TERMINAL false
#define ERROR_PARSER_CREATE_RULES_UNDEFINED_NONTERMINAL false
#define ERROR_PARSER_CREATE_MULTIPLE_PARSE_END_TERMINALS false
#define ERROR_PARSER_CREATE_TERMINAL_REGULAR_EXPRESSION_PARSE_ERROR false
#define ERROR_PARSER_CREATE_SLR1_PARSE_TABLE_CONFLICT false
#define ERROR_PARSER_CREATE_CANNOT_RESOLVE_RULE_HEAD_FOR_SHIFT_ACTION false
#define ERROR_PARSER_CREATE_UNSPECIFIED_ERROR false

// - basic constants and definitions -

#define c_base_char_cnt 256
#define c_def_special_char_cnt 3
#define c_no_char_base (c_base_char_cnt + c_def_special_char_cnt)

#define reg_char_white  (c_base_char_cnt)     // 'w' {' ','\b','\t','\n','\v','\f','\r'}
#define reg_char_digit  (c_base_char_cnt + 1) // 'd' {'0' - '9'}
#define reg_char_letter (c_base_char_cnt + 2) // 'l' {'a' - 'z','A' - 'Z'}

#define c_lalr_table_reduce_base 0x80000000

/*
 * identification of operations assigned to reductions
 */

enum
{
  c_reduce_null_0 = 0,
  c_reduce_alter,
  c_reduce_null_1,
  c_reduce_conc,
  c_reduce_null_2,
  c_reduce_iter,
  c_reduce_char_eps,
  c_reduce_char,
  c_reduce_char_white,
  c_reduce_char_digit,
  c_reduce_char_letter,
  c_reduce_char_no_char,
  c_reduce_char_str,
  c_reduce_char_range,
  c_reduce_char_alter,
  c_reduce_char_except,
  c_reduce_null_3
};

// parse constants
#define c_reg_rule_cnt 17
extern const unsigned reg_rule_head_idxs[c_reg_rule_cnt];
extern const unsigned reg_rule_body_lengths[c_reg_rule_cnt];

// lalr parse table
#define reg_blank c_idx_not_exist
#define REG_SHIFT(VALUE) VALUE
#define REG_REDUCE(VALUE) c_reg_lalr_table_reduce_base + VALUE
#define REG_GOTO(VALUE) VALUE

#define c_reg_lalr_table_reduce_base 0x80000000
#define c_reg_terminal_plus_nonterminal_cnt 21
#define c_reg_lalr_state_cnt 23

extern const unsigned reg_lalr_table[c_reg_lalr_state_cnt*c_reg_terminal_plus_nonterminal_cnt];

/*
 * definition of generated structures
 */

// -- bb_array_s --
@begin
array<bi> bb_array_s;
@end

// -- ui_arrays_s --
@begin
array<ui_array_s> ui_arrays_s;
@end

// -- ui_array_set_s --
@begin
array<ui>
ui_array_set_s
p_first_set_s
p_follow_set_s;
@end

int ui_array_set_s_set_compare(ui_array_set_s *this,ui_array_set_s *second);
void ui_array_set_s_add_to_set(ui_array_set_s *this,p_follow_set_s *add_set);

// -- ui_array_sets_s --
@begin
array<ui_array_set_s>
ui_array_sets_s
p_first_sets_s
p_follow_sets_s
p_rule_idxs_sets_s;
@end

// -- ui_array_sets_array_s --
@begin
array<ui_array_sets_s>
ui_array_sets_array_s
p_first_rules_sets_s;
@end

// -- reg_mul_state_move_s --
@begin
struct
<
usi:move_char
ui:from_state
ui_array_set_s:mul_state
>
reg_mul_state_move_s;
@end

// -- reg_mul_state_move_array_s --
@begin
array<reg_mul_state_move_s>
reg_mul_state_move_array_s;
@end

unsigned reg_mul_state_move_array_s_get_idx_by_move_char(reg_mul_state_move_array_s *this,
    usi a_move_char);

// -- reg_mul_state_move_queue_s --
@begin
queue<reg_mul_state_move_s> reg_mul_state_move_queue_s;
@end

// -- reg_mul_state_map_s --
@begin
struct
<
ui_array_set_s:mul_state
ui:dka_state_idx
>
reg_mul_state_map_s;
@end

// -- reg_mul_state_map_array_s --
@begin
array<reg_mul_state_map_s>
reg_mul_state_map_array_s;
@end

unsigned reg_mul_state_map_array_s_get_idx_by_mul_state(reg_mul_state_map_array_s *this,
    ui_array_set_s *a_mul_state);

// -- fa_state_move_s --
@begin
struct
<
usi:idx
ui:value
>
fa_state_move_s
reg_state_move_s;
@end

// -- fa_state_moves_s --
@begin
array<fa_state_move_s>
fa_state_moves_s
reg_state_moves_s;
@end

unsigned fa_state_moves_s_get_map_idx(fa_state_moves_s *this,
    fa_state_move_s *a_value);

// -- fa_state_moves_array_s --
@begin
array<fa_state_moves_s> fa_state_moves_array_s;
@end

// -- us_array_s --
@begin
array<usi> us_array_s;
@end

// -- fa_state_s --
@begin
struct
<
ui:final
fa_state_moves_s:moves
>
fa_state_s;
@end

// -- fa_states_s --
@begin
array<fa_state_s> fa_states_s;
@end

unsigned fa_states_s_recognize(fa_states_s *this,
    const char *input,unsigned *input_idx,unsigned input_length);
void fa_states_s_to_dot_code(fa_states_s *this,bc_array_s *a_trg);

// -- fa_states_array_s --
@begin
array<fa_states_s> fa_states_array_s;
@end

// -- reg_state_s --
@begin
struct
<
ui:ui_first
ui:ui_second
>
reg_state_s
p_kernel_rule_dot_s
p_kernel_goto_s
pcs_state_position_s;
@end

// -- reg_states_s --
@begin
array<reg_state_s>
reg_states_s
p_kernel_rule_dots_s
p_kernel_gotos_s
pcs_state_positions_s;
@end

#define p_kernel_rule_dots_s_compare reg_states_s_compare

// -- fa_state_descr_s --
@begin
struct
<
ui:from_state_id
usi:with_char
reg_states_s:reg_states
>
fa_state_descr_s;
@end

// -- fa_state_reg_states_s --
@begin
array<reg_states_s> fa_state_reg_states_s;
@end

// -- fa_state_reg_state_tree_s --
@begin
rb_tree<reg_states_s> fa_state_reg_state_tree_s;
@end

// -- fa_state_descr_queue_s --
@begin
queue<fa_state_descr_s> fa_state_descr_queue_s;
@end

// -- finite_automata_s --
@begin
struct
<
ui:state_idx
fa_states_s:states
ui_arrays_s:state_moves
>
finite_automata_s;
@end

void finite_automata_s_create_new(finite_automata_s *this,
    fa_states_array_s *states_array);
void finite_automata_s_moves_from_states(finite_automata_s *this);
unsigned finite_automata_s_recognize(finite_automata_s *this,
    const char *input,unsigned *input_idx,unsigned input_length);

// -- reg_parser_s --
@begin
struct
<
lalr_stack_s:lalr_stack
ui:fas_idx
fa_states_s:states
>
reg_parser_s;
@end

static inline usi reg_parser_s_process_char(char **c_ptr);
unsigned reg_parser_s_recognize_terminal(const string_s *source_string,unsigned *input_idx);
int reg_parser_s_parse_reg_exp(reg_parser_s *this,const string_s *source_string,unsigned a_final);
int reg_parser_s_NKA_to_DKA(reg_parser_s *this);
static inline int reg_parser_s_process_reg_exp(reg_parser_s *this,const string_s *source_string,unsigned a_final);
int reg_parser_sprocess_reg_exps(reg_parser_s *this,
    string_array_s *source_strings,fa_states_array_s *states_array);

// -- p_terminal_s --
@begin
struct
<
string_s:idx
string_s:value
ui:source_pos
>
p_terminal_s;
@end

// -- p_terminals_s --
@begin
array<p_terminal_s>
p_terminals_s;
@end

unsigned p_terminals_s_get_map_idx(p_terminals_s *this,
    p_terminal_s *a_value);
unsigned p_terminals_s_get_char_ptr_idx(p_terminals_s *this,
    unsigned a_length,const char *a_data);

// -- p_nonterminal_s --
@begin
struct
<
string_s:name
>
p_nonterminal_s;
@end

// -- p_nonterminals_s --
@begin
array<p_nonterminal_s>
p_nonterminals_s;
@end

unsigned p_nonterminals_s_get_char_ptr_idx(p_nonterminals_s *this,
    unsigned a_length,const char *a_data);

// -- p_rule_s --
@begin
struct
<
ui:head
ui_array_s:body
>
p_rule_s;
@end

// -- p_rules_s --
@begin
array<p_rule_s> p_rules_s;
@end

// -- p_kernel_s --
@begin
struct
<
p_kernel_rule_dots_s:rule_dots
p_kernel_gotos_s:gotos
>
p_kernel_s;
@end

// -- p_kernels_s --
@begin
array<p_kernel_s>
p_kernels_s;
@end

unsigned p_kernels_s_get_idx_by_rule_dots(p_kernels_s *this,p_kernel_s *a_value);

// -- p_lalr_table_s --
@begin
struct
<
ui:x_size
ui:y_size
ui_array_s:table
>
p_lalr_table_s;
@end

static inline unsigned *p_lalr_table_s_value(p_lalr_table_s *this,unsigned a_x_idx,unsigned a_y_idx);
static inline void p_lalr_table_s_resize(p_lalr_table_s *this,unsigned a_x_size,unsigned a_y_size);
static inline void p_lalr_table_s_fill(p_lalr_table_s *this,unsigned a_value);

// -- p_creat_descr_s --
@begin
struct
<
finite_automata_s:rule_file_fa
finite_automata_s:key_terminals_fa
p_terminals_s:terminals
p_nonterminals_s:nonterminals
p_rules_s:rules
p_first_sets_s:firsts
p_first_rules_sets_s:first_rules
p_follow_sets_s:follows
p_kernels_s:kernels
>
p_creat_descr_s;
@end

int p_creat_descr_s_load_finite_automata_set_new(p_creat_descr_s *this);
int p_creat_descr_s_load_from_rule_char_ptr(p_creat_descr_s *this,unsigned a_length,char *a_data);
int p_creat_descr_s_find_key_terminals(p_creat_descr_s *this,
    unsigned *end_terminal,ui_array_s *skip_terminals);
int p_creat_descr_s_create_final_automata_new(p_creat_descr_s *this,
    finite_automata_s *final_automata);

/*
 * compute set first for nonterminal symbols
 */
int p_creat_descr_s_compute_firsts(p_creat_descr_s *this);

/*
 * compute set follow of one nonterminal symbol
 */
int p_creat_descr_s_compute_follows_of_nonterminal(p_creat_descr_s *this,
    unsigned nonterm_idx,bb_array_s *follows_created,ui_array_s *nonterm_used,p_follow_set_s *ret_follows);

/*
 * compute set follow for nonterminal symbols
 */
int p_creat_descr_s_compute_follows(p_creat_descr_s *this);

/*
 * compute parse kernels
 */
int p_creat_descr_s_compute_kernels(p_creat_descr_s *this);

/*
 * create parse table based on parse kernels
 */
int p_creat_descr_s_create_lalr_table(p_creat_descr_s *this,p_lalr_table_s *lalr_table);

// -- p_rule_descr_s --
@begin
struct
<
ui:head
ui:body_size
>
p_rule_descr_s;
@end

// -- p_rule_descrs_s --
@begin
array<p_rule_descr_s> p_rule_descrs_s;
@end

// -- parser_s --
@begin
struct
<
ui:terminal_cnt
ui:end_terminal
ui_array_s:skip_terminals
p_rule_descrs_s:rule_descrs
finite_automata_s:final_automata
p_lalr_table_s:lalr_table
>
parser_s;
@end

int parser_s_create_from_rule_char_ptr(parser_s *this,unsigned a_length,char *a_data);

/*
 * inline methods of generated structures
 */

// -- bb_array_s --
@begin
inlines bb_array_s
@end

// -- ui_arrays_s --
@begin
inlines ui_arrays_s
@end

// -- ui_array_set_s --
@begin
inlines ui_array_set_s
@end

// -- ui_array_sets_s --
@begin
inlines ui_array_sets_s
@end

// -- ui_array_sets_array_s --
@begin
inlines ui_array_sets_array_s
@end

// -- reg_mul_state_move_s --
@begin
inlines reg_mul_state_move_s
@end

// -- reg_mul_state_move_array_s --
@begin
inlines reg_mul_state_move_array_s
@end

// -- reg_mul_state_move_queue_s --
@begin
inlines reg_mul_state_move_queue_s
@end

// -- reg_mul_state_map_s --
@begin
inlines reg_mul_state_map_s
@end

// -- reg_mul_state_map_array_s --
@begin
inlines reg_mul_state_map_array_s
@end

// -- fa_state_move_s --
@begin
inlines fa_state_move_s
@end

// -- fa_state_moves_s --
@begin
inlines fa_state_moves_s
@end

// -- fa_state_moves_array_s --
@begin
inlines fa_state_moves_array_s
@end

// -- us_array_s --
@begin
inlines us_array_s
@end

// -- fa_state_s --
@begin
inlines fa_state_s
@end

// -- fa_states_s --
@begin
inlines fa_states_s
@end

// -- fa_states_array_s --
@begin
inlines fa_states_array_s
@end

// -- reg_state_s --
@begin
inlines reg_state_s
@end

// -- reg_states_s --
@begin
inlines reg_states_s
@end

// -- fa_state_descr_s --
@begin
inlines fa_state_descr_s
@end

// -- fa_state_reg_states_s --
@begin
inlines fa_state_reg_states_s
@end

// -- fa_state_reg_state_tree_s --
@begin
inlines fa_state_reg_state_tree_s
@end

// -- fa_state_descr_queue_s --
@begin
inlines fa_state_descr_queue_s
@end

static inline int fa_state_reg_state_tree_s___compare_value(const fa_state_reg_state_tree_s *this,const reg_states_s *a_first,const reg_states_s *a_second)
{/*{{{*/
  (void)this;

  if (a_first->used != a_second->used)
  {
    return a_first->used < a_second->used ? -1 : 1;
  }

  if (a_first->used != 0)
  {
    reg_state_s *frs_ptr = a_first->data;
    reg_state_s *frs_ptr_end = frs_ptr + a_first->used;
    reg_state_s *srs_ptr = a_second->data;
    do {
      if (frs_ptr->ui_first != srs_ptr->ui_first)
      {
        return frs_ptr->ui_first < srs_ptr->ui_first ? -1 : 1;
      }

      if (frs_ptr->ui_second != srs_ptr->ui_second)
      {
        return frs_ptr->ui_second < srs_ptr->ui_second ? -1 : 1;
      }
    } while(++srs_ptr,++frs_ptr < frs_ptr_end);
  }

  return 0;
}/*}}}*/

// -- finite_automata_s --
@begin
inlines finite_automata_s
@end

// -- reg_parser_s --
@begin
inlines reg_parser_s
@end

static inline usi reg_parser_s_process_char(char **c_ptr)
{/*{{{*/
  if (*(*c_ptr) == '\\')
  {
    (*c_ptr)++;

    // - process char represented by octal number -
    if (*(*c_ptr) >= '0' && *(*c_ptr) <= '7')
    {
      unsigned short new_char = *(*c_ptr)++ - '0';

      if (*(*c_ptr) >= '0' && *(*c_ptr) <= '7')
      {
        new_char = (new_char << 3) | (*(*c_ptr)++ - '0');

        if (*(*c_ptr) >= '0' && *(*c_ptr) <= '7')
        {
          new_char = (new_char << 3) | (*(*c_ptr)++ - '0');
        }
      }

      return new_char;
    }

    // - process char represented by hexadecimal number -
    if (*(*c_ptr) == 'x')
    {
      (*c_ptr)++;
      unsigned short new_char = 0;

      // - first char in hexadecimal representation -
      if (*(*c_ptr) >= '0' && *(*c_ptr) <= '9')
      {
        new_char = *(*c_ptr)++ - '0';
      }
      else if (*(*c_ptr) >= 'a' && *(*c_ptr) <= 'f')
      {
        new_char = 10 + (*(*c_ptr)++ - 'a');
      }
      else if (*(*c_ptr) >= 'A' && *(*c_ptr) <= 'F')
      {
        new_char = 10 + (*(*c_ptr)++ - 'A');
      }
      else
      {
        debug_assert(0);
      }

      // - second char in hexadecimal representation -
      if (*(*c_ptr) >= '0' && *(*c_ptr) <= '9')
      {
        new_char = (new_char << 4) | (*(*c_ptr)++ - '0');
      }
      else if (*(*c_ptr) >= 'a' && *(*c_ptr) <= 'f')
      {
        new_char = (new_char << 4) | (10 + (*(*c_ptr)++ - 'a'));
      }
      else if (*(*c_ptr) >= 'A' && *(*c_ptr) <= 'F')
      {
        new_char = (new_char << 4) | (10 + (*(*c_ptr)++ - 'A'));
      }

      return new_char;
    }

    switch (*(*c_ptr)++)
    {
    case 'a':
      return '\a';
    case 'b':
      return '\b';
    case 'f':
      return '\f';
    case 'n':
      return '\n';
    case 'r':
      return '\r';
    case 't':
      return '\t';
    case 'v':
      return '\v';
    case '\\':
      return '\\';
    case '?':
      return'?';
    case '\'':
      return '\'';
    case '"':
      return'"';
    case '>':
      return '>';
    case '}':
      return '}';
    case ']':
      return ']';
    case '|':
      return '|';
    default:
      cassert(0);
      return 0xffff;
    }
  }

  return (unsigned char)*(*c_ptr)++;
}/*}}}*/

static inline int reg_parser_s_process_reg_exp(reg_parser_s *this,
    const string_s *source_string,unsigned a_final)
{/*{{{*/
  return
    reg_parser_s_parse_reg_exp(this,source_string,a_final) &&
    reg_parser_s_NKA_to_DKA(this);
}/*}}}*/

// -- p_terminal_s --
@begin
inlines p_terminal_s
@end

// -- p_terminals_s --
@begin
inlines p_terminals_s
@end

// -- p_nonterminal_s --
@begin
inlines p_nonterminal_s
@end

// -- p_nonterminals_s --
@begin
inlines p_nonterminals_s
@end

// -- p_rule_s --
@begin
inlines p_rule_s
@end

// -- p_rules_s --
@begin
inlines p_rules_s
@end

// -- p_kernel_s --
@begin
inlines p_kernel_s
@end

// -- p_kernels_s --
@begin
inlines p_kernels_s
@end

// -- p_lalr_table_s --
@begin
inlines p_lalr_table_s
@end

static inline unsigned *p_lalr_table_s_value(p_lalr_table_s *this,unsigned a_x_idx,unsigned a_y_idx)
{/*{{{*/
  unsigned t_idx = a_y_idx*this->x_size + a_x_idx;
  debug_assert(t_idx < this->table.used);
  return this->table.data + t_idx;
}/*}}}*/

static inline void p_lalr_table_s_resize(p_lalr_table_s *this,unsigned a_x_size,unsigned a_y_size)
{/*{{{*/
  this->x_size = a_x_size;
  this->y_size = a_y_size;
  ui_array_s_copy_resize(&this->table,a_y_size*a_x_size);
}/*}}}*/

static inline void p_lalr_table_s_fill(p_lalr_table_s *this,unsigned a_value)
{/*{{{*/
  ui_array_s_fill(&this->table,a_value);
}/*}}}*/

// -- p_creat_descr_s --
@begin
inlines p_creat_descr_s
@end

// -- p_rule_descr_s --
@begin
inlines p_rule_descr_s
@end

// -- p_rule_descrs_s --
@begin
inlines p_rule_descrs_s
@end

// -- parser_s --
@begin
inlines parser_s
@end

#endif

