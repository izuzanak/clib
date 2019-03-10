
@begin
include "cl_rtsp_sdp_parser.h"
@end

// - parse constants -
#define c_rtsp_sdp_rule_cnt 7
const unsigned rtsp_sdp_rule_head_idxs[c_rtsp_sdp_rule_cnt] = {5, 6, 6, 7, 7, 7, 7, };
const unsigned rtsp_sdp_rule_body_lengths[c_rtsp_sdp_rule_cnt] = {2, 2, 1, 1, 1, 1, 1, };

// - lalr parse table -
#define rtsp_sdp_blank c_idx_not_exist
#define RTSP_SDP_SHIFT(VALUE) VALUE
#define RTSP_SDP_REDUCE(VALUE) c_rtsp_sdp_lalr_table_reduce_base + VALUE
#define RTSP_SDP_GOTO(VALUE) VALUE

#define c_rtsp_sdp_lalr_table_reduce_base 0x80000000
#define c_rtsp_sdp_terminal_plus_nonterminal_cnt 8
#define rtsp_sdp_lalr_state_cnt 9

const unsigned rtsp_sdp_lalr_table[rtsp_sdp_lalr_state_cnt*c_rtsp_sdp_terminal_plus_nonterminal_cnt] =
{/*{{{*/
    RTSP_SDP_SHIFT(3),    RTSP_SDP_SHIFT(4),    RTSP_SDP_SHIFT(5),    RTSP_SDP_SHIFT(6),       rtsp_sdp_blank,       rtsp_sdp_blank,     RTSP_SDP_GOTO(1),     RTSP_SDP_GOTO(2),
    RTSP_SDP_SHIFT(3),    RTSP_SDP_SHIFT(4),    RTSP_SDP_SHIFT(5),    RTSP_SDP_SHIFT(6),    RTSP_SDP_SHIFT(8),       rtsp_sdp_blank,       rtsp_sdp_blank,     RTSP_SDP_GOTO(7),
   RTSP_SDP_REDUCE(2),   RTSP_SDP_REDUCE(2),   RTSP_SDP_REDUCE(2),   RTSP_SDP_REDUCE(2),   RTSP_SDP_REDUCE(2),       rtsp_sdp_blank,       rtsp_sdp_blank,       rtsp_sdp_blank,
   RTSP_SDP_REDUCE(3),   RTSP_SDP_REDUCE(3),   RTSP_SDP_REDUCE(3),   RTSP_SDP_REDUCE(3),   RTSP_SDP_REDUCE(3),       rtsp_sdp_blank,       rtsp_sdp_blank,       rtsp_sdp_blank,
   RTSP_SDP_REDUCE(4),   RTSP_SDP_REDUCE(4),   RTSP_SDP_REDUCE(4),   RTSP_SDP_REDUCE(4),   RTSP_SDP_REDUCE(4),       rtsp_sdp_blank,       rtsp_sdp_blank,       rtsp_sdp_blank,
   RTSP_SDP_REDUCE(5),   RTSP_SDP_REDUCE(5),   RTSP_SDP_REDUCE(5),   RTSP_SDP_REDUCE(5),   RTSP_SDP_REDUCE(5),       rtsp_sdp_blank,       rtsp_sdp_blank,       rtsp_sdp_blank,
   RTSP_SDP_REDUCE(6),   RTSP_SDP_REDUCE(6),   RTSP_SDP_REDUCE(6),   RTSP_SDP_REDUCE(6),   RTSP_SDP_REDUCE(6),       rtsp_sdp_blank,       rtsp_sdp_blank,       rtsp_sdp_blank,
   RTSP_SDP_REDUCE(1),   RTSP_SDP_REDUCE(1),   RTSP_SDP_REDUCE(1),   RTSP_SDP_REDUCE(1),   RTSP_SDP_REDUCE(1),       rtsp_sdp_blank,       rtsp_sdp_blank,       rtsp_sdp_blank,
       rtsp_sdp_blank,       rtsp_sdp_blank,       rtsp_sdp_blank,       rtsp_sdp_blank,       rtsp_sdp_blank,       rtsp_sdp_blank,       rtsp_sdp_blank,       rtsp_sdp_blank,
};/*}}}*/

// === rtsp parse actions access ==========================================

#define c_rtsp_sdp_parse_action_cnt 7
void (*rtsp_sdp_pa_callers[c_rtsp_sdp_parse_action_cnt])(rtsp_sdp_parser_s *this) =
{/*{{{*/
  pa_rtsp_sdp_null,
  pa_rtsp_sdp_null,
  pa_rtsp_sdp_null,
  pa_rtsp_sdp_null,
  pa_rtsp_sdp_video,
  pa_rtsp_sdp_audio,
  pa_rtsp_sdp_control,
};/*}}}*/

// === rtsp parse actions =================================================

void pa_rtsp_sdp_null(rtsp_sdp_parser_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

void pa_rtsp_sdp_video(rtsp_sdp_parser_s *this)
{/*{{{*/
  this->media_type = rtsp_sdp_media_type_VIDEO;
}/*}}}*/

void pa_rtsp_sdp_audio(rtsp_sdp_parser_s *this)
{/*{{{*/
  this->media_type = rtsp_sdp_media_type_AUDIO;
}/*}}}*/

void pa_rtsp_sdp_control(rtsp_sdp_parser_s *this)
{/*{{{*/
  string_s *source_string = &this->source_string;
  lalr_stack_s *lalr_stack = &this->lalr_stack;

  // *****

  lalr_stack_element_s *lse = lalr_stack_s_last(lalr_stack);
  unsigned str_length = lse->terminal_end - 2 - (lse->terminal_start + 10);
  char *str_data = source_string->data + lse->terminal_start + 10;

  switch (this->media_type)
  {
  case rtsp_sdp_media_type_VIDEO:
    string_s_set(&this->video_control,str_length,str_data);
    break;
  case rtsp_sdp_media_type_AUDIO:
    string_s_set(&this->audio_control,str_length,str_data);
    break;
  }
}/*}}}*/

// === methods of generated structures =========================================

// -- rtsp_sdp_parser_s --
@begin
methods rtsp_sdp_parser_s
@end

unsigned rtsp_sdp_parser_s_recognize_terminal(rtsp_sdp_parser_s *this) // NOLINT
{/*{{{*/
  unsigned source_string_length = this->source_string.size - 1;

#define RTSP_SDP_GET_NEXT_CHAR() \
  {\
    if (this->input_idx < source_string_length) {\
      in_char = (unsigned char)this->source_string.data[this->input_idx];\
    }\
    else {\
      in_char = '\0';\
    }\
  }

#define RTSP_SDP_CLOSE_CHAR(RET_TERM_IDX) \
  {\
    if (in_char == '\0') {\
      return RET_TERM_IDX;\
    }\
    \
    ++this->input_idx;\
  }

   unsigned char in_char;

// - STATE 0 -
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char == 0) {
      goto state_1_label;
   }
   if (in_char >= 1 && in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char >= 14 && in_char < 97) {
      goto state_2_label;
   }
   if (in_char == 97) {
      goto state_3_label;
   }
   if (in_char >= 98 && in_char < 109) {
      goto state_2_label;
   }
   if (in_char == 109) {
      goto state_4_label;
   }
   if (in_char >= 110) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 1 -
state_1_label:
   RTSP_SDP_CLOSE_CHAR(4);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14) {
      goto state_2_label;
   }
   return 4;

// - STATE 2 -
state_2_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 3 -
state_3_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 61) {
      goto state_2_label;
   }
   if (in_char == 61) {
      goto state_6_label;
   }
   if (in_char >= 62) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 4 -
state_4_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 61) {
      goto state_2_label;
   }
   if (in_char == 61) {
      goto state_7_label;
   }
   if (in_char >= 62) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 5 -
state_5_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_8_label;
   }
   return c_idx_not_exist;

// - STATE 6 -
state_6_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 99) {
      goto state_2_label;
   }
   if (in_char == 99) {
      goto state_9_label;
   }
   if (in_char >= 100) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 7 -
state_7_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 97) {
      goto state_2_label;
   }
   if (in_char == 97) {
      goto state_10_label;
   }
   if (in_char >= 98 && in_char < 118) {
      goto state_2_label;
   }
   if (in_char == 118) {
      goto state_11_label;
   }
   if (in_char >= 119) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 8 -
state_8_label:
   RTSP_SDP_CLOSE_CHAR(0);
   return 0;

// - STATE 9 -
state_9_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 111) {
      goto state_2_label;
   }
   if (in_char == 111) {
      goto state_12_label;
   }
   if (in_char >= 112) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 10 -
state_10_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 117) {
      goto state_2_label;
   }
   if (in_char == 117) {
      goto state_13_label;
   }
   if (in_char >= 118) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 11 -
state_11_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 105) {
      goto state_2_label;
   }
   if (in_char == 105) {
      goto state_14_label;
   }
   if (in_char >= 106) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 12 -
state_12_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 110) {
      goto state_2_label;
   }
   if (in_char == 110) {
      goto state_15_label;
   }
   if (in_char >= 111) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 13 -
state_13_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 100) {
      goto state_2_label;
   }
   if (in_char == 100) {
      goto state_16_label;
   }
   if (in_char >= 101) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 14 -
state_14_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 100) {
      goto state_2_label;
   }
   if (in_char == 100) {
      goto state_17_label;
   }
   if (in_char >= 101) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 15 -
state_15_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 116) {
      goto state_2_label;
   }
   if (in_char == 116) {
      goto state_18_label;
   }
   if (in_char >= 117) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 16 -
state_16_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 105) {
      goto state_2_label;
   }
   if (in_char == 105) {
      goto state_19_label;
   }
   if (in_char >= 106) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 17 -
state_17_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 101) {
      goto state_2_label;
   }
   if (in_char == 101) {
      goto state_20_label;
   }
   if (in_char >= 102) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 18 -
state_18_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 114) {
      goto state_2_label;
   }
   if (in_char == 114) {
      goto state_21_label;
   }
   if (in_char >= 115) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 19 -
state_19_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 111) {
      goto state_2_label;
   }
   if (in_char == 111) {
      goto state_22_label;
   }
   if (in_char >= 112) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 20 -
state_20_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 111) {
      goto state_2_label;
   }
   if (in_char == 111) {
      goto state_23_label;
   }
   if (in_char >= 112) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 21 -
state_21_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 111) {
      goto state_2_label;
   }
   if (in_char == 111) {
      goto state_24_label;
   }
   if (in_char >= 112) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 22 -
state_22_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_22_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_22_label;
   }
   if (in_char == 13) {
      goto state_25_label;
   }
   if (in_char >= 14) {
      goto state_22_label;
   }
   return c_idx_not_exist;

// - STATE 23 -
state_23_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_23_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_23_label;
   }
   if (in_char == 13) {
      goto state_26_label;
   }
   if (in_char >= 14) {
      goto state_23_label;
   }
   return c_idx_not_exist;

// - STATE 24 -
state_24_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 108) {
      goto state_2_label;
   }
   if (in_char == 108) {
      goto state_27_label;
   }
   if (in_char >= 109) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 25 -
state_25_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_28_label;
   }
   return c_idx_not_exist;

// - STATE 26 -
state_26_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_29_label;
   }
   return c_idx_not_exist;

// - STATE 27 -
state_27_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_2_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_2_label;
   }
   if (in_char == 13) {
      goto state_5_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_2_label;
   }
   if (in_char == 58) {
      goto state_30_label;
   }
   if (in_char >= 59) {
      goto state_2_label;
   }
   return c_idx_not_exist;

// - STATE 28 -
state_28_label:
   RTSP_SDP_CLOSE_CHAR(2);
   return 2;

// - STATE 29 -
state_29_label:
   RTSP_SDP_CLOSE_CHAR(1);
   return 1;

// - STATE 30 -
state_30_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_30_label;
   }
   if (in_char >= 11 && in_char < 13) {
      goto state_30_label;
   }
   if (in_char == 13) {
      goto state_31_label;
   }
   if (in_char >= 14) {
      goto state_30_label;
   }
   return c_idx_not_exist;

// - STATE 31 -
state_31_label:
   RTSP_SDP_CLOSE_CHAR(c_idx_not_exist);
   RTSP_SDP_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_32_label;
   }
   return c_idx_not_exist;

// - STATE 32 -
state_32_label:
   RTSP_SDP_CLOSE_CHAR(3);
   return 3;

}/*}}}*/

int rtsp_sdp_parser_s_parse(rtsp_sdp_parser_s *this,string_s *a_src)
{/*{{{*/
  string_s_swap(&this->source_string,a_src);

  this->input_idx = 0;
  this->old_input_idx = 0;

  this->media_type = rtsp_sdp_media_type_UNKNOW;

  this->lalr_stack.used = 0;
  lalr_stack_s_push_state(&this->lalr_stack,0);

  unsigned ret_term = c_idx_not_exist;

#define RTSP_SDP_PARSE_SOURCE_RETURN() \
  string_s_swap(&this->source_string,a_src);

  do
  {
    // - retrieve next terminal symbol -
    while (ret_term == c_idx_not_exist)
    {
      this->old_input_idx = this->input_idx;

      ret_term = rtsp_sdp_parser_s_recognize_terminal(this);

      // - PARSE ERROR unrecognized terminal -
      if (ret_term == c_idx_not_exist)
      {
        RTSP_SDP_PARSE_SOURCE_RETURN();

        throw_error(RTSP_SDP_PARSE_UNRECOGNIZED_TERMINAL);
      }
    }

    // - retrieve action from table of actions -
    unsigned parse_action = rtsp_sdp_lalr_table[lalr_stack_s_last(&this->lalr_stack)->lalr_state*c_rtsp_sdp_terminal_plus_nonterminal_cnt + ret_term];

    // - PARSE ERROR wrong syntax -
    if (parse_action == c_idx_not_exist)
    {
      RTSP_SDP_PARSE_SOURCE_RETURN();

      throw_error(RTSP_SDP_PARSE_INVALID_SYNTAX);
    }

    // - action RTSP_SDP_SHIFT -
    if (parse_action < c_rtsp_sdp_lalr_table_reduce_base)
    {
      if (ret_term == 4)
      {
        break;
      }

      // - insertion of state, and terminal position in source string -
      lalr_stack_s_push_state_all(&this->lalr_stack,parse_action,this->old_input_idx,this->input_idx);
      ret_term = c_idx_not_exist;
    }

    // - action RTSP_SDP_REDUCE-
    else
    {
      parse_action -= c_rtsp_sdp_lalr_table_reduce_base;

      // - call function assigned to reduction -
      if (rtsp_sdp_pa_callers[parse_action] != pa_rtsp_sdp_null)
      {
        rtsp_sdp_pa_callers[parse_action](this);
      }

      // - remove rule body from lalr stack -
      this->lalr_stack.used -= rtsp_sdp_rule_body_lengths[parse_action];

      // - insert new automata state to lalr stack -
      unsigned goto_val = rtsp_sdp_lalr_table[lalr_stack_s_last(&this->lalr_stack)->lalr_state*c_rtsp_sdp_terminal_plus_nonterminal_cnt + rtsp_sdp_rule_head_idxs[parse_action]];
      lalr_stack_s_push_state(&this->lalr_stack,goto_val);
    }
  }
  while(1);

  RTSP_SDP_PARSE_SOURCE_RETURN();

  return 0;
}/*}}}*/

