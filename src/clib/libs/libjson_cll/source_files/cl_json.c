
@begin
include "cl_json.h"
include "cl_json_parse_actions.h"
@end

// === constants and definitions ===============================================

const unsigned json_rule_head_idxs[c_json_rule_cnt] = {17, 18, 19, 20, 20, 21, 22, 22, 23, 24, 25, 25, 26, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 29, };
const unsigned json_rule_body_lengths[c_json_rule_cnt] = {1, 2, 2, 1, 2, 1, 3, 1, 3, 2, 1, 2, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, };

const unsigned json_lalr_table[json_lalr_state_cnt*c_json_terminal_plus_nonterminal_cnt] =
{/*{{{*/
   JSON_SHIFT(11),    JSON_SHIFT(4),    JSON_SHIFT(5),    JSON_SHIFT(8),    JSON_SHIFT(9),   JSON_SHIFT(10),   JSON_SHIFT(16),       json_blank,   JSON_SHIFT(17),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,     JSON_GOTO(1),     JSON_GOTO(6),    JSON_GOTO(12),    JSON_GOTO(14),       json_blank,       json_blank,     JSON_GOTO(7),    JSON_GOTO(13),    JSON_GOTO(15),       json_blank,     JSON_GOTO(2),     JSON_GOTO(3),
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_SHIFT(18),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(15),       json_blank,  JSON_REDUCE(15),  JSON_REDUCE(15),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(15),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(16),       json_blank,  JSON_REDUCE(16),  JSON_REDUCE(16),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(16),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(17),       json_blank,  JSON_REDUCE(17),  JSON_REDUCE(17),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(17),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(18),       json_blank,  JSON_REDUCE(18),  JSON_REDUCE(18),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(18),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(19),       json_blank,  JSON_REDUCE(19),  JSON_REDUCE(19),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(19),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(20),       json_blank,  JSON_REDUCE(20),  JSON_REDUCE(20),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(20),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(21),       json_blank,  JSON_REDUCE(21),  JSON_REDUCE(21),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(21),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(22),       json_blank,  JSON_REDUCE(22),  JSON_REDUCE(22),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(22),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(23),       json_blank,  JSON_REDUCE(23),  JSON_REDUCE(23),  JSON_REDUCE(23),       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(23),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_SHIFT(19),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_SHIFT(20),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
   JSON_SHIFT(11),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(3),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,    JSON_GOTO(21),    JSON_GOTO(22),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,    JSON_GOTO(23),
   JSON_SHIFT(11),    JSON_SHIFT(4),    JSON_SHIFT(5),    JSON_SHIFT(8),    JSON_SHIFT(9),   JSON_SHIFT(10),   JSON_SHIFT(16),       json_blank,   JSON_SHIFT(17),  JSON_REDUCE(10),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,     JSON_GOTO(6),    JSON_GOTO(12),    JSON_GOTO(14),       json_blank,       json_blank,     JSON_GOTO(7),    JSON_GOTO(13),    JSON_GOTO(15),    JSON_GOTO(24),    JSON_GOTO(25),     JSON_GOTO(3),
   JSON_REDUCE(5),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(5),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
  JSON_REDUCE(12),  JSON_REDUCE(12),  JSON_REDUCE(12),  JSON_REDUCE(12),  JSON_REDUCE(12),  JSON_REDUCE(12),  JSON_REDUCE(12),       json_blank,  JSON_REDUCE(12),  JSON_REDUCE(12),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(2),       json_blank,   JSON_REDUCE(2),   JSON_REDUCE(2),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(2),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(9),       json_blank,   JSON_REDUCE(9),   JSON_REDUCE(9),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(9),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(4),       json_blank,       json_blank,   JSON_SHIFT(26),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(7),       json_blank,       json_blank,   JSON_REDUCE(7),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_SHIFT(27),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(11),   JSON_SHIFT(28),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(14),  JSON_REDUCE(14),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
   JSON_SHIFT(11),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,    JSON_GOTO(29),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,    JSON_GOTO(23),
   JSON_SHIFT(11),    JSON_SHIFT(4),    JSON_SHIFT(5),    JSON_SHIFT(8),    JSON_SHIFT(9),   JSON_SHIFT(10),   JSON_SHIFT(16),       json_blank,   JSON_SHIFT(17),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,     JSON_GOTO(6),    JSON_GOTO(12),    JSON_GOTO(14),       json_blank,       json_blank,     JSON_GOTO(7),    JSON_GOTO(13),    JSON_GOTO(15),       json_blank,    JSON_GOTO(30),     JSON_GOTO(3),
   JSON_SHIFT(11),    JSON_SHIFT(4),    JSON_SHIFT(5),    JSON_SHIFT(8),    JSON_SHIFT(9),   JSON_SHIFT(10),   JSON_SHIFT(16),       json_blank,   JSON_SHIFT(17),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,     JSON_GOTO(6),    JSON_GOTO(12),    JSON_GOTO(14),       json_blank,       json_blank,     JSON_GOTO(7),    JSON_GOTO(13),    JSON_GOTO(15),       json_blank,    JSON_GOTO(31),     JSON_GOTO(3),
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(6),       json_blank,       json_blank,   JSON_REDUCE(6),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(8),       json_blank,       json_blank,   JSON_REDUCE(8),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(13),  JSON_REDUCE(13),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
};/*}}}*/

// === methods of generated structures =========================================

// -- lli_tree_s --
@begin
methods lli_tree_s
@end

// -- bd_tree_s --
@begin
methods bd_tree_s
@end

// -- json_parser_s --
@begin
methods json_parser_s
@end

void json_parser_s_process_json_string(const char *a_ptr,const char *a_ptr_end,bc_array_s *a_trg)
{/*{{{*/
  a_trg->used = 0;

  if (a_ptr < a_ptr_end)
  {
    do
    {
      if (*a_ptr == '\\')
      {
        a_ptr++;

        // - process character represented by unicode number -
        if (*a_ptr == 'u')
        {
          a_ptr++;

          unsigned value = 0;

          // - retrieve character value -
          const char *a_ptr_end = a_ptr + 4;
          do {
            value <<= 4;

            if (*a_ptr >= '0' && *a_ptr <= '9')
            {
              value += *a_ptr - '0';
            }
            else if (*a_ptr >= 'a' && *a_ptr <= 'f')
            {
              value += 10 + (*a_ptr - 'a');
            }
            else if (*a_ptr >= 'A' && *a_ptr <= 'F')
            {
              value += 10 + (*a_ptr - 'A');
            }
            else
            {
              debug_assert(0);
            }

          } while(++a_ptr < a_ptr_end);

          // - convert utf16/32 value to utf8 character string -
          if (value <= 0x7f)
          {
            bc_array_s_push(a_trg,value);
          }
          else if (value <= 0x7ff)
          {
            bc_array_s_push(a_trg,0xc0 | value >> 6);
            bc_array_s_push(a_trg,0x80 | (value & 0x3f));
          }
          else if (value <= 0xffff)
          {
            bc_array_s_push(a_trg,0xe0 |   value >> 12);
            bc_array_s_push(a_trg,0x80 | ((value >>  6) & 0x3f));
            bc_array_s_push(a_trg,0x80 |  (value        & 0x3f));
          }
        }
        else
        {
          switch (*a_ptr++)
          {
          case 'b':
            bc_array_s_push(a_trg,'\b');
            break;
          case 'f':
            bc_array_s_push(a_trg,'\f');
            break;
          case 'n':
            bc_array_s_push(a_trg,'\n');
            break;
          case 'r':
            bc_array_s_push(a_trg,'\r');
            break;
          case 't':
            bc_array_s_push(a_trg,'\t');
            break;
          case '\\':
            bc_array_s_push(a_trg,'\\');
            break;
          case '"':
            bc_array_s_push(a_trg,'"');
            break;
          default:
            debug_assert(0);
          }
        }
      }
      else
      {
        bc_array_s_push(a_trg,*a_ptr++);
      }
    }
    while(a_ptr < a_ptr_end);
  }

  // - modification of character buffer -
  bc_array_s_push(a_trg,'\0');
}/*}}}*/

unsigned json_parser_s_recognize_terminal(const bc_array_s *a_source,unsigned *a_input_idx)
{/*{{{*/
#define JSON_GET_NEXT_CHAR() \
  {\
    if (*a_input_idx < a_source->used) {\
      in_char = (unsigned char)a_source->data[*a_input_idx];\
    }\
    else {\
      in_char = '\0';\
    }\
  }

#define JSON_CLOSE_CHAR(RET_TERM_IDX) \
  {\
    if (in_char == '\0') {\
      return RET_TERM_IDX;\
    }\
    \
    ++*a_input_idx;\
  }

   unsigned char in_char;

// - STATE 0 -
   JSON_GET_NEXT_CHAR();

   if (in_char == 0) {
      goto state_1_label;
   }
   if (in_char >= 8 && in_char < 14) {
      goto state_17_label;
   }
   if (in_char == 32) {
      goto state_17_label;
   }
   if (in_char == 34) {
      goto state_2_label;
   }
   if (in_char == 35) {
      goto state_3_label;
   }
   if (in_char == 44) {
      goto state_4_label;
   }
   if (in_char == 45) {
      goto state_5_label;
   }
   if (in_char == 47) {
      goto state_6_label;
   }
   if (in_char == 48) {
      goto state_7_label;
   }
   if (in_char >= 49 && in_char < 58) {
      goto state_8_label;
   }
   if (in_char == 58) {
      goto state_9_label;
   }
   if (in_char == 91) {
      goto state_10_label;
   }
   if (in_char == 93) {
      goto state_11_label;
   }
   if (in_char == 102) {
      goto state_12_label;
   }
   if (in_char == 110) {
      goto state_13_label;
   }
   if (in_char == 116) {
      goto state_14_label;
   }
   if (in_char == 123) {
      goto state_15_label;
   }
   if (in_char == 125) {
      goto state_16_label;
   }
   return c_idx_not_exist;

// - STATE 1 -
state_1_label:
   JSON_CLOSE_CHAR(16);
   return 16;

// - STATE 2 -
state_2_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char < 34) {
      goto state_2_label;
   }
   if (in_char == 34) {
      goto state_18_label;
   }
   if (in_char >= 35 && in_char < 92) {
      goto state_2_label;
   }
   if (in_char == 92) {
      goto state_19_label;
   }
   if (in_char >= 93) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 3 -
state_3_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_3_label;
   }
   if (in_char == 10) {
      goto state_20_label;
   }
   if (in_char >= 11) {
      goto state_3_label;
   }
   return c_idx_not_exist;

// - STATE 4 -
state_4_label:
   JSON_CLOSE_CHAR(10);
   return 10;

// - STATE 5 -
state_5_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 48) {
      goto state_7_label;
   }
   if (in_char >= 49 && in_char < 58) {
      goto state_8_label;
   }
   return c_idx_not_exist;

// - STATE 6 -
state_6_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 42) {
      goto state_21_label;
   }
   if (in_char == 47) {
      goto state_22_label;
   }
   return c_idx_not_exist;

// - STATE 7 -
state_7_label:
   JSON_CLOSE_CHAR(1);
   JSON_GET_NEXT_CHAR();

   if (in_char == 46) {
      goto state_23_label;
   }
   if (in_char == 69) {
      goto state_24_label;
   }
   if (in_char == 101) {
      goto state_24_label;
   }
   return 1;

// - STATE 8 -
state_8_label:
   JSON_CLOSE_CHAR(1);
   JSON_GET_NEXT_CHAR();

   if (in_char == 46) {
      goto state_23_label;
   }
   if (in_char >= 48 && in_char < 58) {
      goto state_8_label;
   }
   if (in_char == 69) {
      goto state_24_label;
   }
   if (in_char == 101) {
      goto state_24_label;
   }
   return 1;

// - STATE 9 -
state_9_label:
   JSON_CLOSE_CHAR(11);
   return 11;

// - STATE 10 -
state_10_label:
   JSON_CLOSE_CHAR(8);
   return 8;

// - STATE 11 -
state_11_label:
   JSON_CLOSE_CHAR(9);
   return 9;

// - STATE 12 -
state_12_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 97) {
      goto state_25_label;
   }
   return c_idx_not_exist;

// - STATE 13 -
state_13_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 117) {
      goto state_26_label;
   }
   return c_idx_not_exist;

// - STATE 14 -
state_14_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 114) {
      goto state_27_label;
   }
   return c_idx_not_exist;

// - STATE 15 -
state_15_label:
   JSON_CLOSE_CHAR(6);
   return 6;

// - STATE 16 -
state_16_label:
   JSON_CLOSE_CHAR(7);
   return 7;

// - STATE 17 -
state_17_label:
   JSON_CLOSE_CHAR(12);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 8 && in_char < 14) {
      goto state_17_label;
   }
   if (in_char == 32) {
      goto state_17_label;
   }
   return 12;

// - STATE 18 -
state_18_label:
   JSON_CLOSE_CHAR(0);
   return 0;

// - STATE 19 -
state_19_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 34) {
      goto state_2_label;
   }
   if (in_char == 47) {
      goto state_2_label;
   }
   if (in_char == 92) {
      goto state_2_label;
   }
   if (in_char == 98) {
      goto state_2_label;
   }
   if (in_char == 102) {
      goto state_2_label;
   }
   if (in_char == 110) {
      goto state_2_label;
   }
   if (in_char == 114) {
      goto state_2_label;
   }
   if (in_char == 116) {
      goto state_2_label;
   }
   if (in_char == 117) {
      goto state_28_label;
   }
   return c_idx_not_exist;

// - STATE 20 -
state_20_label:
   JSON_CLOSE_CHAR(13);
   return 13;

// - STATE 21 -
state_21_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char < 42) {
      goto state_21_label;
   }
   if (in_char == 42) {
      goto state_29_label;
   }
   if (in_char >= 43) {
      goto state_21_label;
   }
   return c_idx_not_exist;

// - STATE 22 -
state_22_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_22_label;
   }
   if (in_char == 10) {
      goto state_30_label;
   }
   if (in_char >= 11) {
      goto state_22_label;
   }
   return c_idx_not_exist;

// - STATE 23 -
state_23_label:
   JSON_CLOSE_CHAR(2);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_23_label;
   }
   if (in_char == 69) {
      goto state_24_label;
   }
   if (in_char == 101) {
      goto state_24_label;
   }
   return 2;

// - STATE 24 -
state_24_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 43) {
      goto state_31_label;
   }
   if (in_char == 45) {
      goto state_31_label;
   }
   if (in_char >= 48 && in_char < 58) {
      goto state_32_label;
   }
   return c_idx_not_exist;

// - STATE 25 -
state_25_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 108) {
      goto state_33_label;
   }
   return c_idx_not_exist;

// - STATE 26 -
state_26_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 108) {
      goto state_34_label;
   }
   return c_idx_not_exist;

// - STATE 27 -
state_27_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 117) {
      goto state_35_label;
   }
   return c_idx_not_exist;

// - STATE 28 -
state_28_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_36_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_36_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_36_label;
   }
   return c_idx_not_exist;

// - STATE 29 -
state_29_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char < 47) {
      goto state_21_label;
   }
   if (in_char == 47) {
      goto state_37_label;
   }
   if (in_char >= 48) {
      goto state_21_label;
   }
   return c_idx_not_exist;

// - STATE 30 -
state_30_label:
   JSON_CLOSE_CHAR(14);
   return 14;

// - STATE 31 -
state_31_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_32_label;
   }
   return c_idx_not_exist;

// - STATE 32 -
state_32_label:
   JSON_CLOSE_CHAR(2);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_32_label;
   }
   return 2;

// - STATE 33 -
state_33_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 115) {
      goto state_38_label;
   }
   return c_idx_not_exist;

// - STATE 34 -
state_34_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 108) {
      goto state_39_label;
   }
   return c_idx_not_exist;

// - STATE 35 -
state_35_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_40_label;
   }
   return c_idx_not_exist;

// - STATE 36 -
state_36_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_41_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_41_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_41_label;
   }
   return c_idx_not_exist;

// - STATE 37 -
state_37_label:
   JSON_CLOSE_CHAR(15);
   return 15;

// - STATE 38 -
state_38_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_42_label;
   }
   return c_idx_not_exist;

// - STATE 39 -
state_39_label:
   JSON_CLOSE_CHAR(5);
   return 5;

// - STATE 40 -
state_40_label:
   JSON_CLOSE_CHAR(3);
   return 3;

// - STATE 41 -
state_41_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_43_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_43_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_43_label;
   }
   return c_idx_not_exist;

// - STATE 42 -
state_42_label:
   JSON_CLOSE_CHAR(4);
   return 4;

// - STATE 43 -
state_43_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_2_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_2_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_2_label;
   }
   return c_idx_not_exist;

}/*}}}*/

int json_parser_s_parse(json_parser_s *this,const bc_array_s *a_src,var_s *a_trg)
{/*{{{*/
  this->source = (pointer)a_src;

  this->input_idx = 0;
  this->old_input_idx = 0;

  lalr_stack_s_clear(&this->lalr_stack);
  lalr_stack_s_push_state(&this->lalr_stack,0);

  unsigned ret_term = c_idx_not_exist;

  do
  {
    // - retrieve next terminal symbol -
    while (ret_term == c_idx_not_exist)
    {
      this->old_input_idx = this->input_idx;

      ret_term = json_parser_s_recognize_terminal((const bc_array_s *)this->source,&this->input_idx);

      // - PARSE ERROR unrecognized terminal -
      if (ret_term == c_idx_not_exist)
      {
        throw_error(JSON_PARSE_UNRECOGNIZED_TERMINAL);
      }

      // - skipping of _SKIP_ terminals -
      if (ret_term >= 12 && ret_term <= 15)
      {
        ret_term = c_idx_not_exist;
      }
    }

    // - retrieve action from table of actions -
    unsigned parse_action = json_lalr_table[lalr_stack_s_last(&this->lalr_stack)->lalr_state*c_json_terminal_plus_nonterminal_cnt + ret_term];

    // - PARSE ERROR wrong syntax -
    if (parse_action == c_idx_not_exist)
    {
      throw_error(JSON_PARSE_INVALID_SYNTAX);
    }

    // - action JSON_SHIFT -
    if (parse_action < c_json_lalr_table_reduce_base)
    {
      if (ret_term == 16)
      {
        break;
      }

      // - insertion of state, and terminal position in source string -
      lalr_stack_s_push_state_all(&this->lalr_stack,parse_action,this->old_input_idx,this->input_idx);
      ret_term = c_idx_not_exist;
    }

    // - action JSON_REDUCE-
    else
    {
      parse_action -= c_json_lalr_table_reduce_base;

      // - call function assigned to reduction -
      if (json_pa_callers[parse_action] != pa_json_null)
      {
        json_pa_callers[parse_action](this);
      }

      // - remove rule body from lalr stack -
      this->lalr_stack.used -= json_rule_body_lengths[parse_action];

      // - insert new automata state to lalr stack -
      unsigned goto_val = json_lalr_table[lalr_stack_s_last(&this->lalr_stack)->lalr_state*c_json_terminal_plus_nonterminal_cnt + json_rule_head_idxs[parse_action]];
      lalr_stack_s_push_state(&this->lalr_stack,goto_val);
    }
  }
  while(1);

  // - retrieve parsed value -
  var_s_copy(a_trg,var_array_s_pop(&this->values));

  return 0;
}/*}}}*/

// -- json_create_stack_element_s --
@begin
methods json_create_stack_element_s
@end

// -- json_create_stack_s --
@begin
methods json_create_stack_s
@end

// -- from_json_s --
@begin
methods from_json_s
@end

int from_json_s_get_integer(from_json_s *this,const bc_array_s *a_src,long long int *a_value)
{/*{{{*/
  do {
    this->old_input_idx = this->input_idx;
    this->terminal = json_parser_s_recognize_terminal(a_src,&this->input_idx);

    switch (this->terminal)
    {
      case c_json_terminal_integer:
        *a_value = strtoll(a_src->data + this->old_input_idx,NULL,10);
        break;
      case c_json_terminal_true:
        *a_value = 1;
        break;
      case c_json_terminal_false:
        *a_value = 0;
        break;
      case c_json_terminal__SKIP_0:
      case c_json_terminal__SKIP_1:
      case c_json_terminal__SKIP_2:
      case c_json_terminal__SKIP_3:
        continue;
      default:
        return 1;
    }

    return 0;
  } while(1);
}/*}}}*/

int from_json_s_get_float(from_json_s *this,const bc_array_s *a_src,double *a_value)
{/*{{{*/
  do {
    this->old_input_idx = this->input_idx;
    this->terminal = json_parser_s_recognize_terminal(a_src,&this->input_idx);

    switch (this->terminal)
    {
      case c_json_terminal_integer:
        *a_value = strtoll(a_src->data + this->old_input_idx,NULL,10);
        break;
      case c_json_terminal_float:
        *a_value = strtod(a_src->data + this->old_input_idx,NULL);
        break;
      case c_json_terminal__SKIP_0:
      case c_json_terminal__SKIP_1:
      case c_json_terminal__SKIP_2:
      case c_json_terminal__SKIP_3:
        continue;
      default:
        return 1;
    }

    return 0;
  } while(1);
}/*}}}*/

int from_json_s_get_string(from_json_s *this,const bc_array_s *a_src)
{/*{{{*/
  do {
    this->old_input_idx = this->input_idx;
    this->terminal = json_parser_s_recognize_terminal(a_src,&this->input_idx);

    switch (this->terminal)
    {
      case c_json_terminal_string:
        {
          this->buffer.used = 0;

          json_parser_s_process_json_string(
            a_src->data + this->old_input_idx + 1,
            a_src->data + this->input_idx - 1,
            &this->buffer);
        }
        break;
      case c_json_terminal__SKIP_0:
      case c_json_terminal__SKIP_1:
      case c_json_terminal__SKIP_2:
      case c_json_terminal__SKIP_3:
        continue;
      default:
        return 1;
    }

    return 0;
  } while(1);
}/*}}}*/

int from_json_s_get_terminal(from_json_s *this,const bc_array_s *a_src,unsigned a_terminal)
{/*{{{*/
  do {
    this->old_input_idx = this->input_idx;
    this->terminal = json_parser_s_recognize_terminal(a_src,&this->input_idx);

    switch (this->terminal)
    {
      case c_json_terminal__SKIP_0:
      case c_json_terminal__SKIP_1:
      case c_json_terminal__SKIP_2:
      case c_json_terminal__SKIP_3:
        continue;
      default:
        if (this->terminal != a_terminal)
        {
          return 1;
        }
    }

    return 0;
  } while(1);
}/*}}}*/

// === global functions ========================================================

int json_create(var_s a_obj,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(a_obj != NULL);

  // - reset target buffer -
  a_trg->used = 0;

  // - initialize create stack -
  CONT_INIT(json_create_stack_s,create_stack);

  // - insert source variable to create stack -
  json_create_stack_s_push_blank(&create_stack);
  json_create_stack_element_s_set(
    json_create_stack_s_last(&create_stack),&a_obj,1,c_idx_not_exist);

  do {

    // - reference to last stack element -
    json_create_stack_element_s *cs_elm = json_create_stack_s_last(&create_stack);

    // - retrieve variable -
    var_s var = cs_elm->var;
    debug_assert(var != NULL);

    switch (var->v_type)
    {
    case c_bi_type_blank:
      {/*{{{*/
        bc_array_s_append(a_trg,strlen("null"),"null");
        json_create_stack_s_pop(&create_stack);
      }/*}}}*/
      break;

    case c_bi_type_integer:
      {/*{{{*/
        long long int value = loc_s_int_value(var);

        bc_array_s_reserve(a_trg,max_number_string_length);
        a_trg->used += snprintf(a_trg->data + a_trg->used,max_number_string_length,"%" HOST_LL_FORMAT "d",value);

        json_create_stack_s_pop(&create_stack);
      }/*}}}*/
      break;

    case c_bi_type_float:
      {/*{{{*/
        double value = loc_s_float_value(var);

        bc_array_s_reserve(a_trg,max_number_string_length);
        a_trg->used += snprintf(a_trg->data + a_trg->used,max_number_string_length,"%f",value);

        json_create_stack_s_pop(&create_stack);
      }/*}}}*/
      break;

    case c_bi_type_string:
      {/*{{{*/
        string_s_to_json(loc_s_string_value(var),a_trg);
        json_create_stack_s_pop(&create_stack);
      }/*}}}*/
      break;

    case c_bi_type_array:
      {/*{{{*/
        var_array_s *array = loc_s_array_value(var);

        if (cs_elm->initialize)
        {
          bc_array_s_push(a_trg,'[');

          cs_elm->index = 0;
          cs_elm->initialize = 0;
        }
        else
        {
          if (cs_elm->index < array->used)
          {
            bc_array_s_push(a_trg,',');
          }
        }

        if (cs_elm->index < array->used)
        {
          var_s item_var = array->data[cs_elm->index++];

          // - insert value object to create stack -
          json_create_stack_s_push_blank(&create_stack);
          json_create_stack_element_s_set(
            json_create_stack_s_last(&create_stack),&item_var,1,c_idx_not_exist);
        }
        else
        {
          bc_array_s_push(a_trg,']');

          json_create_stack_s_pop(&create_stack);
        }
      }/*}}}*/
      break;

    case c_bi_type_dict:
      {/*{{{*/
        var_map_tree_s *tree = loc_s_dict_value(var);

        if (cs_elm->index == c_idx_not_exist)
        {
          cs_elm->index = 0;
        }

        if (cs_elm->index < tree->used)
        {
          var_map_tree_s_node *node = tree->data + cs_elm->index++;

          if (node->valid)
          {
            bc_array_s_push(a_trg,cs_elm->initialize ? '{' : ',');
            cs_elm->initialize = 0;

            var_s key_var = node->object.key;
            var_s item_var = node->object.value;

            // - ERROR -
            if (key_var->v_type != c_bi_type_string)
            {
              json_create_stack_s_clear(&create_stack);

              throw_error(JSON_CREATE_NO_STRING_DICT_KEY);
            }

            // - retrieve key string -
            string_s_to_json(loc_s_string_value(key_var),a_trg);
            bc_array_s_push(a_trg,':');

            // - insert value object to create stack -
            json_create_stack_s_push_blank(&create_stack);
            json_create_stack_element_s_set(
              json_create_stack_s_last(&create_stack),&item_var,1,c_idx_not_exist);
          }
        }
        else
        {
          if (cs_elm->initialize)
          {
            bc_array_s_push(a_trg,'{');
          }

          bc_array_s_push(a_trg,'}');

          json_create_stack_s_pop(&create_stack);
        }
      }/*}}}*/
      break;

    // - ERROR -
    default:
      json_create_stack_s_clear(&create_stack);

      throw_error(JSON_CREATE_UNSUPPORTED_TYPE);
    }

  } while(create_stack.used > 0);

  // - release create stack -
  json_create_stack_s_clear(&create_stack);

  return 0;
}/*}}}*/

int json_create_nice(var_s a_obj,const string_s *a_tabulator,
    const string_s *a_indent,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(a_obj != NULL);

  // - reset target buffer -
  a_trg->used = 0;

#define JSON_CREATE_NICE_CLEAR() \
{/*{{{*/\
  json_create_stack_s_clear(&create_stack);\
  bc_array_s_clear(&indent_buffer);\
}/*}}}*/

#define JSON_CREATE_NICE_PUSH_TAB() \
{/*{{{*/\
  if ((indent_size += a_tabulator->size - 1) > indent_buffer.used)\
  {\
    bc_array_s_append(&indent_buffer,a_tabulator->size - 1,a_tabulator->data);\
  }\
}/*}}}*/

#define JSON_CREATE_NICE_POP_TAB() \
{/*{{{*/\
  indent_size -= a_tabulator->size - 1;\
}/*}}}*/

#define JSON_CREATE_NICE_INDENT() \
{/*{{{*/\
  bc_array_s_push(a_trg,'\n');\
  bc_array_s_append(a_trg,indent_size,indent_buffer.data);\
}/*}}}*/

  // - initialize indent buffer -
  CONT_INIT(bc_array_s,indent_buffer);
  bc_array_s_append(&indent_buffer,a_indent->size - 1,a_indent->data);

  // - initialize actual indent size -
  unsigned indent_size = indent_buffer.used;

  // - initialize create stack -
  CONT_INIT(json_create_stack_s,create_stack);

  // - insert source variable to create stack -
  json_create_stack_s_push_blank(&create_stack);
  json_create_stack_element_s_set(
    json_create_stack_s_last(&create_stack),&a_obj,1,c_idx_not_exist);

  do {

    // - reference to last stack element -
    json_create_stack_element_s *cs_elm = json_create_stack_s_last(&create_stack);

    // - retrieve variable -
    var_s var = cs_elm->var;
    debug_assert(var != NULL);

    switch (var->v_type)
    {
    case c_bi_type_blank:
      {/*{{{*/
        bc_array_s_append(a_trg,strlen("null"),"null");
        json_create_stack_s_pop(&create_stack);
      }/*}}}*/
      break;

    case c_bi_type_integer:
      {/*{{{*/
        long long int value = loc_s_int_value(var);

        bc_array_s_reserve(a_trg,max_number_string_length);
        a_trg->used += snprintf(a_trg->data + a_trg->used,max_number_string_length,"%" HOST_LL_FORMAT "d",value);

        json_create_stack_s_pop(&create_stack);
      }/*}}}*/
      break;

    case c_bi_type_float:
      {/*{{{*/
        double value = loc_s_float_value(var);

        bc_array_s_reserve(a_trg,max_number_string_length);
        a_trg->used += snprintf(a_trg->data + a_trg->used,max_number_string_length,"%f",value);

        json_create_stack_s_pop(&create_stack);
      }/*}}}*/
      break;

    case c_bi_type_string:
      {/*{{{*/
        string_s_to_json(loc_s_string_value(var),a_trg);
        json_create_stack_s_pop(&create_stack);
      }/*}}}*/
      break;

    case c_bi_type_array:
      {/*{{{*/
        var_array_s *array = loc_s_array_value(var);

        if (cs_elm->initialize)
        {
          bc_array_s_push(a_trg,'[');

          // - if array is not empty -
          if (array->used != 0)
          {
            // - push tabulator to indent -
            JSON_CREATE_NICE_PUSH_TAB();

            // - insert indentation -
            JSON_CREATE_NICE_INDENT();
          }

          cs_elm->index = 0;
          cs_elm->initialize = 0;
        }
        else
        {
          if (cs_elm->index < array->used)
          {
            bc_array_s_push(a_trg,',');

            // - insert indentation -
            JSON_CREATE_NICE_INDENT();
          }
        }

        if (cs_elm->index < array->used)
        {
          var_s item_var = array->data[cs_elm->index++];

          // - insert value object to create stack -
          json_create_stack_s_push_blank(&create_stack);
          json_create_stack_element_s_set(
            json_create_stack_s_last(&create_stack),&item_var,1,c_idx_not_exist);
        }
        else
        {
          // - if array is not empty -
          if (array->used != 0)
          {
            // - remove tabulator from indent -
            JSON_CREATE_NICE_POP_TAB();

            // - insert indentation -
            JSON_CREATE_NICE_INDENT();
          }

          bc_array_s_push(a_trg,']');

          json_create_stack_s_pop(&create_stack);
        }
      }/*}}}*/
      break;

    case c_bi_type_dict:
      {/*{{{*/
        var_map_tree_s *tree = loc_s_dict_value(var);

        if (cs_elm->index == c_idx_not_exist)
        {
          cs_elm->index = 0;
        }

        if (cs_elm->index < tree->used)
        {
          var_map_tree_s_node *node = tree->data + cs_elm->index++;

          if (node->valid)
          {
            if (cs_elm->initialize)
            {
              bc_array_s_push(a_trg,'{');

              // - push tabulator to indent -
              JSON_CREATE_NICE_PUSH_TAB();

              // - insert indentation -
              JSON_CREATE_NICE_INDENT();
            }
            else
            {
              bc_array_s_push(a_trg,',');

              // - insert indentation -
              JSON_CREATE_NICE_INDENT();
            }

            cs_elm->initialize = 0;

            var_s key_var = node->object.key;
            var_s item_var = node->object.value;

            // - ERROR -
            if (key_var->v_type != c_bi_type_string)
            {
              JSON_CREATE_NICE_CLEAR();

              throw_error(JSON_CREATE_NO_STRING_DICT_KEY);
            }

            // - retrieve key string -
            string_s_to_json(loc_s_string_value(key_var),a_trg);
            bc_array_s_push(a_trg,':');
            bc_array_s_push(a_trg,' ');

            // - insert value object to create stack -
            json_create_stack_s_push_blank(&create_stack);
            json_create_stack_element_s_set(
              json_create_stack_s_last(&create_stack),&item_var,1,c_idx_not_exist);
          }
        }
        else
        {
          if (cs_elm->initialize)
          {
            bc_array_s_push(a_trg,'{');
          }
          else
          {
            // - remove tabulator from indent -
            JSON_CREATE_NICE_POP_TAB();

            // - insert indentation -
            JSON_CREATE_NICE_INDENT();
          }

          bc_array_s_push(a_trg,'}');

          json_create_stack_s_pop(&create_stack);
        }
      }/*}}}*/
      break;

    // - ERROR -
    default:
      JSON_CREATE_NICE_CLEAR();

      throw_error(JSON_CREATE_UNSUPPORTED_TYPE);
    }
  } while(create_stack.used > 0);

  // - release create stack -
  json_create_stack_s_clear(&create_stack);

  // - release indent buffer -
  bc_array_s_clear(&indent_buffer);

  return 0;
}/*}}}*/

