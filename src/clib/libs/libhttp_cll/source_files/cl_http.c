
@begin
include "cl_http.h"
@end

// === methods of generated structures =========================================

// -- bc_buffer_s --
@begin
methods bc_buffer_s
@end

// -- bc_buffers_s --
@begin
methods bc_buffers_s
@end

// -- bc_buffer_pair_s --
@begin
methods bc_buffer_pair_s
@end

// -- bc_buffer_pairs_s --
@begin
methods bc_buffer_pairs_s
@end

// -- string_pair_s --
@begin
methods string_pair_s
@end

// -- string_pair_tree_s --
@begin
methods string_pair_tree_s
@end

// -- http_conn_s --
@begin
methods http_conn_s
@end

unsigned http_conn_s_recognize_request_terminal(http_conn_s *this,const bc_array_s *a_source) // lgtm [cpp/use-of-goto]
{/*{{{*/
#define HTTP_REQUEST_GET_NEXT_CHAR() \
{\
   if (this->input_idx < a_source->used) {\
      in_char = (unsigned char)a_source->data[this->input_idx];\
   }\
   else {\
      in_char = '\0';\
   }\
}

#define HTTP_REQUEST_CLOSE_CHAR(RET_TERM_IDX) \
{\
   if (this->input_idx >= a_source->used) {\
      return RET_TERM_IDX;\
   }\
\
   this->input_idx++;\
}

   unsigned char in_char;

// - STATE 0 -
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 8 && in_char < 14) {
      goto state_7_label;
   }
   if (in_char == 32) {
      goto state_7_label;
   }
   if (in_char == 38) {
      goto state_1_label;
   }
   if (in_char == 47) {
      goto state_2_label;
   }
   if (in_char == 61) {
      goto state_3_label;
   }
   if (in_char == 63) {
      goto state_1_label;
   }
   if (in_char == 71) {
      goto state_4_label;
   }
   if (in_char == 72) {
      goto state_5_label;
   }
   if (in_char == 80) {
      goto state_6_label;
   }
   return c_idx_not_exist;

// - STATE 1 -
state_1_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char < 37) {
      goto state_8_label;
   }
   if (in_char == 37) {
      goto state_9_label;
   }
   if (in_char < 61) {
      goto state_8_label;
   }
   if (in_char >= 62) {
      goto state_8_label;
   }
   return c_idx_not_exist;

// - STATE 2 -
state_2_label:
   HTTP_REQUEST_CLOSE_CHAR(4);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char < 32) {
      goto state_2_label;
   }
   if (in_char >= 33 && in_char < 37) {
      goto state_2_label;
   }
   if (in_char == 37) {
      goto state_10_label;
   }
   if (in_char >= 38 && in_char < 47) {
      goto state_2_label;
   }
   if (in_char >= 48 && in_char < 63) {
      goto state_2_label;
   }
   if (in_char >= 64) {
      goto state_2_label;
   }
   return 4;

// - STATE 3 -
state_3_label:
   HTTP_REQUEST_CLOSE_CHAR(8);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char < 32) {
      goto state_3_label;
   }
   if (in_char >= 33 && in_char < 37) {
      goto state_3_label;
   }
   if (in_char == 37) {
      goto state_11_label;
   }
   if (in_char >= 39) {
      goto state_3_label;
   }
   return 8;

// - STATE 4 -
state_4_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 69) {
      goto state_12_label;
   }
   return c_idx_not_exist;

// - STATE 5 -
state_5_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 69) {
      goto state_13_label;
   }
   if (in_char == 84) {
      goto state_14_label;
   }
   return c_idx_not_exist;

// - STATE 6 -
state_6_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 79) {
      goto state_15_label;
   }
   if (in_char == 85) {
      goto state_16_label;
   }
   return c_idx_not_exist;

// - STATE 7 -
state_7_label:
   HTTP_REQUEST_CLOSE_CHAR(11);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 8 && in_char < 14) {
      goto state_7_label;
   }
   if (in_char == 32) {
      goto state_7_label;
   }
   return 11;

// - STATE 8 -
state_8_label:
   HTTP_REQUEST_CLOSE_CHAR(6);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char < 37) {
      goto state_8_label;
   }
   if (in_char == 37) {
      goto state_17_label;
   }
   if (in_char < 61) {
      goto state_8_label;
   }
   if (in_char >= 62) {
      goto state_8_label;
   }
   return 6;

// - STATE 9 -
state_9_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_18_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_18_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_18_label;
   }
   return c_idx_not_exist;

// - STATE 10 -
state_10_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_19_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_19_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_19_label;
   }
   return c_idx_not_exist;

// - STATE 11 -
state_11_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_20_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_20_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_20_label;
   }
   return c_idx_not_exist;

// - STATE 12 -
state_12_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_21_label;
   }
   return c_idx_not_exist;

// - STATE 13 -
state_13_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 65) {
      goto state_22_label;
   }
   return c_idx_not_exist;

// - STATE 14 -
state_14_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_23_label;
   }
   return c_idx_not_exist;

// - STATE 15 -
state_15_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 83) {
      goto state_24_label;
   }
   return c_idx_not_exist;

// - STATE 16 -
state_16_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_25_label;
   }
   return c_idx_not_exist;

// - STATE 17 -
state_17_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_26_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_26_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_26_label;
   }
   return c_idx_not_exist;

// - STATE 18 -
state_18_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_27_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_27_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_27_label;
   }
   return c_idx_not_exist;

// - STATE 19 -
state_19_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_28_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_28_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_28_label;
   }
   return c_idx_not_exist;

// - STATE 20 -
state_20_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_29_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_29_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_29_label;
   }
   return c_idx_not_exist;

// - STATE 21 -
state_21_label:
   HTTP_REQUEST_CLOSE_CHAR(0);
   return 0;

// - STATE 22 -
state_22_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 68) {
      goto state_30_label;
   }
   return c_idx_not_exist;

// - STATE 23 -
state_23_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 80) {
      goto state_31_label;
   }
   return c_idx_not_exist;

// - STATE 24 -
state_24_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 84) {
      goto state_32_label;
   }
   return c_idx_not_exist;

// - STATE 25 -
state_25_label:
   HTTP_REQUEST_CLOSE_CHAR(3);
   return 3;

// - STATE 26 -
state_26_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_27_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_27_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_27_label;
   }
   return c_idx_not_exist;

// - STATE 27 -
state_27_label:
   HTTP_REQUEST_CLOSE_CHAR(7);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char < 37) {
      goto state_27_label;
   }
   if (in_char == 37) {
      goto state_17_label;
   }
   if (in_char < 61) {
      goto state_27_label;
   }
   if (in_char >= 62) {
      goto state_27_label;
   }
   return 7;

// - STATE 28 -
state_28_label:
   HTTP_REQUEST_CLOSE_CHAR(5);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char < 32) {
      goto state_28_label;
   }
   if (in_char >= 33 && in_char < 37) {
      goto state_28_label;
   }
   if (in_char == 37) {
      goto state_10_label;
   }
   if (in_char >= 38 && in_char < 47) {
      goto state_28_label;
   }
   if (in_char >= 48 && in_char < 63) {
      goto state_28_label;
   }
   if (in_char >= 64) {
      goto state_28_label;
   }
   return 5;

// - STATE 29 -
state_29_label:
   HTTP_REQUEST_CLOSE_CHAR(9);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char < 32) {
      goto state_29_label;
   }
   if (in_char >= 33 && in_char < 37) {
      goto state_29_label;
   }
   if (in_char == 37) {
      goto state_11_label;
   }
   if (in_char >= 39) {
      goto state_29_label;
   }
   return 9;

// - STATE 30 -
state_30_label:
   HTTP_REQUEST_CLOSE_CHAR(1);
   return 1;

// - STATE 31 -
state_31_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 47) {
      goto state_33_label;
   }
   return c_idx_not_exist;

// - STATE 32 -
state_32_label:
   HTTP_REQUEST_CLOSE_CHAR(2);
   return 2;

// - STATE 33 -
state_33_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 49) {
      goto state_34_label;
   }
   return c_idx_not_exist;

// - STATE 34 -
state_34_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char == 46) {
      goto state_35_label;
   }
   return c_idx_not_exist;

// - STATE 35 -
state_35_label:
   HTTP_REQUEST_CLOSE_CHAR(c_idx_not_exist);
   HTTP_REQUEST_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 50) {
      goto state_36_label;
   }
   return c_idx_not_exist;

// - STATE 36 -
state_36_label:
   HTTP_REQUEST_CLOSE_CHAR(10);
   return 10;

}/*}}}*/

unsigned http_conn_s_recognize_header_terminal(http_conn_s *this,const bc_array_s *a_source) // lgtm [cpp/use-of-goto]
{/*{{{*/
#define HTTP_HEADER_GET_NEXT_CHAR() \
{\
   if (this->input_idx < a_source->used) {\
      in_char = (unsigned char)a_source->data[this->input_idx];\
   }\
   else {\
      in_char = '\0';\
   }\
}

#define HTTP_HEADER_CLOSE_CHAR(RET_TERM_IDX) \
{\
   if (this->input_idx >= a_source->used) {\
      return RET_TERM_IDX;\
   }\
\
   this->input_idx++;\
}

   unsigned char in_char;

// - STATE 0 -
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char == 13) {
      goto state_2_label;
   }
   if (in_char < 58) {
      goto state_1_label;
   }
   if (in_char == 58) {
      goto state_3_label;
   }
   if (in_char < 67) {
      goto state_1_label;
   }
   if (in_char == 67) {
      goto state_4_label;
   }
   if (in_char < 99) {
      goto state_1_label;
   }
   if (in_char == 99) {
      goto state_4_label;
   }
   goto state_1_label;

// - STATE 1 -
state_1_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59) {
      goto state_1_label;
   }
   return 2;

// - STATE 2 -
state_2_label:
   HTTP_HEADER_CLOSE_CHAR(c_idx_not_exist);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char == 10) {
      goto state_5_label;
   }
   return c_idx_not_exist;

// - STATE 3 -
state_3_label:
   HTTP_HEADER_CLOSE_CHAR(0);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char == 9) {
      goto state_3_label;
   }
   if (in_char == 32) {
      goto state_3_label;
   }
   return 0;

// - STATE 4 -
state_4_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 79) {
      goto state_1_label;
   }
   if (in_char == 79) {
      goto state_6_label;
   }
   if (in_char >= 80 && in_char < 111) {
      goto state_1_label;
   }
   if (in_char == 111) {
      goto state_6_label;
   }
   if (in_char >= 112) {
      goto state_1_label;
   }
   return 2;

// - STATE 5 -
state_5_label:
   HTTP_HEADER_CLOSE_CHAR(1);
   return 1;

// - STATE 6 -
state_6_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 78) {
      goto state_1_label;
   }
   if (in_char == 78) {
      goto state_7_label;
   }
   if (in_char >= 79 && in_char < 110) {
      goto state_1_label;
   }
   if (in_char == 110) {
      goto state_7_label;
   }
   if (in_char >= 111) {
      goto state_1_label;
   }
   return 2;

// - STATE 7 -
state_7_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 84) {
      goto state_1_label;
   }
   if (in_char == 84) {
      goto state_8_label;
   }
   if (in_char >= 85 && in_char < 116) {
      goto state_1_label;
   }
   if (in_char == 116) {
      goto state_8_label;
   }
   if (in_char >= 117) {
      goto state_1_label;
   }
   return 2;

// - STATE 8 -
state_8_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 69) {
      goto state_1_label;
   }
   if (in_char == 69) {
      goto state_9_label;
   }
   if (in_char >= 70 && in_char < 101) {
      goto state_1_label;
   }
   if (in_char == 101) {
      goto state_9_label;
   }
   if (in_char >= 102) {
      goto state_1_label;
   }
   return 2;

// - STATE 9 -
state_9_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 78) {
      goto state_1_label;
   }
   if (in_char == 78) {
      goto state_10_label;
   }
   if (in_char >= 79 && in_char < 110) {
      goto state_1_label;
   }
   if (in_char == 110) {
      goto state_10_label;
   }
   if (in_char >= 111) {
      goto state_1_label;
   }
   return 2;

// - STATE 10 -
state_10_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 84) {
      goto state_1_label;
   }
   if (in_char == 84) {
      goto state_11_label;
   }
   if (in_char >= 85 && in_char < 116) {
      goto state_1_label;
   }
   if (in_char == 116) {
      goto state_11_label;
   }
   if (in_char >= 117) {
      goto state_1_label;
   }
   return 2;

// - STATE 11 -
state_11_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 45) {
      goto state_1_label;
   }
   if (in_char == 45) {
      goto state_12_label;
   }
   if (in_char >= 46 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59) {
      goto state_1_label;
   }
   return 2;

// - STATE 12 -
state_12_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 76) {
      goto state_1_label;
   }
   if (in_char == 76) {
      goto state_13_label;
   }
   if (in_char >= 77 && in_char < 84) {
      goto state_1_label;
   }
   if (in_char == 84) {
      goto state_14_label;
   }
   if (in_char >= 85 && in_char < 108) {
      goto state_1_label;
   }
   if (in_char == 108) {
      goto state_13_label;
   }
   if (in_char >= 109 && in_char < 116) {
      goto state_1_label;
   }
   if (in_char == 116) {
      goto state_14_label;
   }
   if (in_char >= 117) {
      goto state_1_label;
   }
   return 2;

// - STATE 13 -
state_13_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 69) {
      goto state_1_label;
   }
   if (in_char == 69) {
      goto state_15_label;
   }
   if (in_char >= 70 && in_char < 101) {
      goto state_1_label;
   }
   if (in_char == 101) {
      goto state_15_label;
   }
   if (in_char >= 102) {
      goto state_1_label;
   }
   return 2;

// - STATE 14 -
state_14_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 89) {
      goto state_1_label;
   }
   if (in_char == 89) {
      goto state_16_label;
   }
   if (in_char >= 90 && in_char < 121) {
      goto state_1_label;
   }
   if (in_char == 121) {
      goto state_16_label;
   }
   if (in_char >= 122) {
      goto state_1_label;
   }
   return 2;

// - STATE 15 -
state_15_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 78) {
      goto state_1_label;
   }
   if (in_char == 78) {
      goto state_17_label;
   }
   if (in_char >= 79 && in_char < 110) {
      goto state_1_label;
   }
   if (in_char == 110) {
      goto state_17_label;
   }
   if (in_char >= 111) {
      goto state_1_label;
   }
   return 2;

// - STATE 16 -
state_16_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 80) {
      goto state_1_label;
   }
   if (in_char == 80) {
      goto state_18_label;
   }
   if (in_char >= 81 && in_char < 112) {
      goto state_1_label;
   }
   if (in_char == 112) {
      goto state_18_label;
   }
   if (in_char >= 113) {
      goto state_1_label;
   }
   return 2;

// - STATE 17 -
state_17_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 71) {
      goto state_1_label;
   }
   if (in_char == 71) {
      goto state_19_label;
   }
   if (in_char >= 72 && in_char < 103) {
      goto state_1_label;
   }
   if (in_char == 103) {
      goto state_19_label;
   }
   if (in_char >= 104) {
      goto state_1_label;
   }
   return 2;

// - STATE 18 -
state_18_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 69) {
      goto state_1_label;
   }
   if (in_char == 69) {
      goto state_20_label;
   }
   if (in_char >= 70 && in_char < 101) {
      goto state_1_label;
   }
   if (in_char == 101) {
      goto state_20_label;
   }
   if (in_char >= 102) {
      goto state_1_label;
   }
   return 2;

// - STATE 19 -
state_19_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 84) {
      goto state_1_label;
   }
   if (in_char == 84) {
      goto state_21_label;
   }
   if (in_char >= 85 && in_char < 116) {
      goto state_1_label;
   }
   if (in_char == 116) {
      goto state_21_label;
   }
   if (in_char >= 117) {
      goto state_1_label;
   }
   return 2;

// - STATE 20 -
state_20_label:
   HTTP_HEADER_CLOSE_CHAR(3);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59) {
      goto state_1_label;
   }
   return 3;

// - STATE 21 -
state_21_label:
   HTTP_HEADER_CLOSE_CHAR(2);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59 && in_char < 72) {
      goto state_1_label;
   }
   if (in_char == 72) {
      goto state_22_label;
   }
   if (in_char >= 73 && in_char < 104) {
      goto state_1_label;
   }
   if (in_char == 104) {
      goto state_22_label;
   }
   if (in_char >= 105) {
      goto state_1_label;
   }
   return 2;

// - STATE 22 -
state_22_label:
   HTTP_HEADER_CLOSE_CHAR(4);
   HTTP_HEADER_GET_NEXT_CHAR();

   if (in_char < 13) {
      goto state_1_label;
   }
   if (in_char >= 14 && in_char < 58) {
      goto state_1_label;
   }
   if (in_char >= 59) {
      goto state_1_label;
   }
   return 4;

}/*}}}*/

unsigned http_conn_s_decode_url(char *a_ptr,const char *a_ptr_end)
{/*{{{*/
  char *i_ptr = a_ptr;
  char *o_ptr = a_ptr;

  do
  {
    // - decode character -
    if (*i_ptr == '%')
    {
      char in;
      char out;

      // - first code character -
      in = i_ptr[1];

      if (in <= '9')
      {
        out = (in - '0') << 4;
      }
      else if (in <= 'F')
      {
        out = (in - 'A' + 10) << 4;
      }
      else
      {
        out = (in - 'a' + 10) << 4;
      }

      // - second code character -
      in = i_ptr[2];

      if (in <= '9')
      {
        out |= in - '0';
      }
      else if (in <= 'F')
      {
        out |= in - 'A' + 10;
      }
      else
      {
        out |= in - 'a' + 10;
      }

      *o_ptr++ = out;
      i_ptr += 3;
    }

    // - copy character -
    else
    {
      *o_ptr++ = *i_ptr++;
    }

  } while(i_ptr < a_ptr_end);

  return o_ptr - a_ptr;
}/*}}}*/

void http_conn_s_get_headers_tree(http_conn_s *this,string_pair_tree_s *a_trg)
{/*{{{*/
  string_pair_tree_s_clear(a_trg);

  if (this->headers.used != 0)
  {
    bc_buffer_pair_s *bbp_ptr = this->headers.data;
    bc_buffer_pair_s *bbp_ptr_end = bbp_ptr + this->headers.used;
    do {
      CONT_INIT_CLEAR(string_pair_s,pair);

      string_s_set(&pair.id,bbp_ptr->id.length,this->headers_data + bbp_ptr->id.begin);
      string_s_set(&pair.value,bbp_ptr->value.length,this->headers_data + bbp_ptr->value.begin);

      string_pair_tree_s_unique_swap_insert(a_trg,&pair);
    } while(++bbp_ptr < bbp_ptr_end);
  }
}/*}}}*/

// -- http_conns_s --
@begin
methods http_conns_s
@end

// -- http_server_s --
@begin
methods http_server_s
@end

int http_server_s_create(http_server_s *this,
    const char *a_ip,unsigned short a_port,
    http_conn_request_callback_t a_conn_request_callback,
    http_conn_response_callback_t a_conn_response_callback,
    void *a_cb_object)
{/*{{{*/
  debug_message_3(fprintf(stderr,"http_server_s_create\n"));

  http_server_s_clear(this);

  this->conn_request_callback = a_conn_request_callback;
  this->conn_response_callback = a_conn_response_callback;
  this->cb_object = a_cb_object;

  // - create tcp server -
  if (tcp_server_s_create(&this->server,a_ip,a_port,
        http_server_s_tcp_conn_new,
        http_server_s_tcp_conn_drop,
        http_server_s_tcp_conn_recv,
        http_server_s_tcp_conn_send,
        this))
  {
    throw_error(HTTP_SERVER_TCP_SERVER_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

int http_server_s_tcp_conn_new(void *a_http_server,unsigned a_index)
{/*{{{*/
  debug_message_3(fprintf(stderr,"http_server_s_tcp_conn_new: %u\n",a_index));

  http_server_s *this = (http_server_s *)a_http_server;

  while (a_index >= this->conns.used)
  {
    http_conns_s_push_blank(&this->conns);
  }

  http_conn_s *conn = http_conns_s_at(&this->conns,a_index);

  // - initialize connection -
  conn->state = c_http_conn_state_REQUEST;
  conn->input_idx = 0;
  conn->headers_size = 0;
  conn->message_size = 0;
  conn->receive_size = 0;

  return 0;
}/*}}}*/

int http_server_s_tcp_conn_drop(void *a_http_server,unsigned a_index)
{/*{{{*/
  debug_message_3(fprintf(stderr,"http_server_s_tcp_conn_drop: %u\n",a_index));

  http_server_s *this = (http_server_s *)a_http_server;
  http_conn_s *conn = http_conns_s_at(&this->conns,a_index);

  http_conn_s_clear(conn);

  return 0;
}/*}}}*/

int http_server_s_tcp_conn_recv(void *a_http_server,unsigned a_index,bc_array_s *a_message)
{/*{{{*/
  http_server_s *this = (http_server_s *)a_http_server;
  http_conn_s *conn = http_conns_s_at(&this->conns,a_index);

  switch (conn->state)
  {
  case c_http_conn_state_REQUEST:
    {/*{{{*/

      // - retrieve method -
      conn->method = http_conn_s_recognize_request_terminal(conn,a_message);

      switch (conn->method)
      {
      case c_http_req_term_GET:
      case c_http_req_term_HEAD:
      case c_http_req_term_POST:
      case c_http_req_term_PUT:
        break;
      default:
        throw_error(HTTP_SERVER_HTTP_REQUEST_ERROR);
      }

      // - skip whitespace -
      if (http_conn_s_recognize_request_terminal(conn,a_message) != c_http_req_term_WHITESPACE)
      {
        throw_error(HTTP_SERVER_HTTP_REQUEST_ERROR);
      }

      // - retrieve components -
      unsigned old_input_idx = conn->input_idx;
      unsigned term = http_conn_s_recognize_request_terminal(conn,a_message);

      do {
        switch (term)
        {
        case c_http_req_term_COMPONENT:
          {/*{{{*/
            bc_buffers_s_push_blank(&conn->components);
            bc_buffer_s_set(bc_buffers_s_last(&conn->components),
                old_input_idx,conn->input_idx - old_input_idx);
          }/*}}}*/
          break;
        case c_http_req_term_ENC_COMPONENT:
          {/*{{{*/
            bc_buffers_s_push_blank(&conn->components);
            bc_buffer_s_set(bc_buffers_s_last(&conn->components),old_input_idx,
                http_conn_s_decode_url(a_message->data + old_input_idx,a_message->data + conn->input_idx));
          }/*}}}*/
          break;
        default:
          throw_error(HTTP_SERVER_HTTP_REQUEST_ERROR);
        }

        old_input_idx = conn->input_idx;
        term = http_conn_s_recognize_request_terminal(conn,a_message);

        if (term != c_idx_not_exist && (1 << term) & c_http_exit_mask_COMPONENTS)
        {
          break;
        }
      } while(1);

      if (term != c_http_req_term_WHITESPACE)
      {
        do {
          switch (term)
          {
          case c_http_req_term_ID:
            {/*{{{*/
              bc_buffer_pairs_s_push_blank(&conn->params);
              bc_buffer_s_set(&bc_buffer_pairs_s_last(&conn->params)->id,
                  old_input_idx + 1,conn->input_idx - old_input_idx - 1);
            }/*}}}*/
            break;
          case c_http_req_term_ENC_ID:
            {/*{{{*/
              bc_buffer_pairs_s_push_blank(&conn->params);
              bc_buffer_s_set(&bc_buffer_pairs_s_last(&conn->params)->id,old_input_idx + 1,
                  http_conn_s_decode_url(a_message->data + old_input_idx + 1,a_message->data + conn->input_idx));
            }/*}}}*/
            break;
          default:
            throw_error(HTTP_SERVER_HTTP_REQUEST_ERROR);
          }

          old_input_idx = conn->input_idx;
          term = http_conn_s_recognize_request_terminal(conn,a_message);

          switch (term)
          {
          case c_http_req_term_VALUE:
            {/*{{{*/
              bc_buffer_s_set(&bc_buffer_pairs_s_last(&conn->params)->value,
                  old_input_idx + 1,conn->input_idx - old_input_idx - 1);
            }/*}}}*/
            break;
          case c_http_req_term_ENC_VALUE:
            {/*{{{*/
              bc_buffer_s_set(&bc_buffer_pairs_s_last(&conn->params)->value,old_input_idx + 1,
                  http_conn_s_decode_url(a_message->data + old_input_idx + 1,a_message->data + conn->input_idx));
            }/*}}}*/
            break;
          default:
            throw_error(HTTP_SERVER_HTTP_REQUEST_ERROR);
          }

          old_input_idx = conn->input_idx;
          term = http_conn_s_recognize_request_terminal(conn,a_message);

        } while(term != c_http_req_term_WHITESPACE);
      }

      // - retrieve http version -
      old_input_idx = conn->input_idx;
      term = http_conn_s_recognize_request_terminal(conn,a_message);
      if (term != c_http_req_term_HTTP)
      {
        throw_error(HTTP_SERVER_HTTP_REQUEST_ERROR);
      }

      // - set http buffer -
      bc_buffer_s_set(&conn->http,old_input_idx,conn->input_idx - old_input_idx);

      // - skip end of line -
      conn->input_idx += 2;

      // - update connection state -
      conn->state = c_http_conn_state_HEADERS;
    }/*}}}*/

  /* fall through */
  case c_http_conn_state_HEADERS:
    {/*{{{*/
      do {
        unsigned header_idx = conn->input_idx;
        unsigned term = http_conn_s_recognize_header_terminal(conn,a_message);

        if (term == c_idx_not_exist)
        {
          // - header not recognized, wait on data -
          conn->input_idx = header_idx;
          break;
        }

        if (term == c_http_head_term_END)
        {
          // - update message length by header length -
          conn->headers_size = conn->input_idx;
          conn->message_size += conn->input_idx;

          // - update connection state -
          conn->state = c_http_conn_state_BODY;
          break;
        }

        // - invalid terminal recognized -
        if (term < c_http_head_term_HEADER)
        {
          throw_error(HTTP_SERVER_HTTP_HEADERS_ERROR);
        }

        unsigned header_length = conn->input_idx - header_idx;

        // - skip separator -
        if (http_conn_s_recognize_header_terminal(conn,a_message) != c_http_head_term_SEPARATOR)
        {
          throw_error(HTTP_SERVER_HTTP_HEADERS_ERROR);
        }

        unsigned value_idx = conn->input_idx;

        // - find end of line -
        char *ptr = a_message->data + value_idx;
        char *ptr_end = a_message->data + a_message->used;
        do {
          if (*ptr == '\r')
          {
            break;
          }
        } while(++ptr < ptr_end);

        // - end of line was found -
        if (ptr < ptr_end)
        {
          unsigned value_length = (ptr - a_message->data) - value_idx;

          bc_buffer_pairs_s_push_blank(&conn->headers);
          bc_buffer_pair_s *header = bc_buffer_pairs_s_last(&conn->headers);

          bc_buffer_s_set(&header->id,header_idx,header_length);
          bc_buffer_s_set(&header->value,value_idx,value_length);

          switch (term)
          {
          case c_http_head_term_CONTENT_LENGTH:
            {
              char *endptr;
              conn->message_size = strtoll(a_message->data + value_idx,&endptr,10);

              // - invalid value -
              if (endptr != a_message->data + value_idx + value_length)
              {
                throw_error(HTTP_SERVER_HTTP_HEADERS_ERROR);
              }
            }
            break;
          }

          // - update input index -
          conn->input_idx = (ptr - a_message->data) + 2;
        }
        else
        {
          // - value not complete, wait on data -
          conn->input_idx = header_idx;
          break;
        }

      } while(1);
    }/*}}}*/

    if (conn->state != c_http_conn_state_BODY)
    {
      break;
    }

  /* fall through */
  case c_http_conn_state_BODY:
    {/*{{{*/

      //// debug print components
      //if (conn->components.used != 0)
      //{
      //  bc_buffer_s *ptr = conn->components.data;
      //  bc_buffer_s *ptr_end = ptr + conn->components.used;
      //  do {
      //    fprintf(stderr,"comp '%.*s'\n",ptr->length,a_message->data + ptr->begin);
      //  } while(++ptr < ptr_end);
      //}

      //// debug print parameters
      //if (conn->params.used != 0)
      //{
      //  bc_buffer_pair_s *ptr = conn->params.data;
      //  bc_buffer_pair_s *ptr_end = ptr + conn->params.used;
      //  do {
      //    fprintf(stderr,"parameter: '%.*s' --> '%.*s'\n",
      //      ptr->id.length,a_message->data + ptr->id.begin,
      //      ptr->value.length,a_message->data + ptr->value.begin);
      //  } while(++ptr < ptr_end);
      //}

      //// debug print headers
      //if (conn->headers.used != 0)
      //{
      //  bc_buffer_pair_s *ptr = conn->headers.data;
      //  bc_buffer_pair_s *ptr_end = ptr + conn->headers.used;
      //  do {
      //    fprintf(stderr,"header: '%.*s' --> '%.*s'\n",
      //      ptr->id.length,a_message->data + ptr->id.begin,
      //      ptr->value.length,a_message->data + ptr->value.begin);
      //  } while(++ptr < ptr_end);
      //}

      // - receiving small message or all data are already available -
      if (conn->message_size < HTTP_MAX_CHUNK_SIZE || a_message->used >= conn->message_size)
      {
        if (a_message->used >= conn->message_size)
        {
          // - set headers data -
          conn->headers_data = a_message->data;

          // - call conn_request_callback -
          if (((http_conn_request_callback_t)this->conn_request_callback)(this->cb_object,a_index,1))
          {
            throw_error(HTTP_CONN_CALLBACK_ERROR);
          }
        }
      }

      // - receiving large message -
      else
      {
        // - copy headers data -
        if (conn->headers_buffer.used == 0)
        {
          bc_array_s_set(&conn->headers_buffer,conn->headers_size,a_message->data);
          bc_array_s_tail(a_message,a_message->used - conn->headers_size);

          // - set headers data -
          conn->headers_data = conn->headers_buffer.data;

          // - update receive size -
          conn->receive_size += conn->headers_size;
        }

        // - message is complete or chunk is big enought -
        int complete = conn->receive_size + a_message->used >= conn->message_size;
        if (complete || a_message->used >= HTTP_MAX_CHUNK_SIZE)
        {
          // - call conn_request_callback -
          if (((http_conn_request_callback_t)this->conn_request_callback)(this->cb_object,a_index,complete))
          {
            throw_error(HTTP_CONN_CALLBACK_ERROR);
          }

          // - update receive size -
          conn->receive_size += a_message->used;

          a_message->used = 0;
        }
      }
    }/*}}}*/
    break;
  }

  return 0;
}/*}}}*/

int http_server_s_tcp_conn_send(void *a_http_server,unsigned a_index)
{/*{{{*/
  http_server_s *this = (http_server_s *)a_http_server;

  // - conn response callback is set -
  if (this->conn_response_callback)
  {
    tcp_conn_s *tcp_conn = tcp_conn_list_s_at(&this->server.conn_list,a_index);

    // - empty output queue -
    if (tcp_conn->out_msg_queue.used == 0)
    {
      // - call conn_response_callback -
      if (((http_conn_response_callback_t)this->conn_response_callback)(this->cb_object,a_index))
      {
        throw_error(HTTP_CONN_CALLBACK_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

