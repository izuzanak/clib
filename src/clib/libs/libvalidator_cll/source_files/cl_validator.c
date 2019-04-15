
@begin
include "cl_validator.h"
@end

// === methods of generated structures =========================================

// -- regex_array_s --
@begin
methods regex_array_s
@end

// -- validator_s --
@begin
methods validator_s
@end

unsigned validator_s_recognize_property(const char *a_string)
{/*{{{*/
#define VALIDATOR_GET_NEXT_CHAR() \
{\
   in_char = (unsigned char)a_string[input_idx];\
}

#define VALIDATOR_CLOSE_CHAR(RET_TERM_IDX) \
{\
   if (in_char == '\0') {\
      return RET_TERM_IDX;\
   }\
\
   input_idx++;\
}

   unsigned input_idx = 0;
   unsigned char in_char;

// - STATE 0 -
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 33) {
      goto state_1_label;
   }
   if (in_char == 60) {
      goto state_2_label;
   }
   if (in_char == 61) {
      goto state_3_label;
   }
   if (in_char == 62) {
      goto state_4_label;
   }
   if (in_char == 97) {
      goto state_5_label;
   }
   if (in_char == 105) {
      goto state_6_label;
   }
   if (in_char == 108) {
      goto state_7_label;
   }
   if (in_char == 114) {
      goto state_8_label;
   }
   if (in_char == 116) {
      goto state_9_label;
   }
   return c_idx_not_exist;

// - STATE 1 -
state_1_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_10_label;
   }
   return c_idx_not_exist;

// - STATE 2 -
state_2_label:
   VALIDATOR_CLOSE_CHAR(3);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_11_label;
   }
   return 3;

// - STATE 3 -
state_3_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_12_label;
   }
   return c_idx_not_exist;

// - STATE 4 -
state_4_label:
   VALIDATOR_CLOSE_CHAR(4);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_13_label;
   }
   return 4;

// - STATE 5 -
state_5_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 108) {
      goto state_14_label;
   }
   return c_idx_not_exist;

// - STATE 6 -
state_6_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 110) {
      goto state_15_label;
   }
   if (in_char == 116) {
      goto state_16_label;
   }
   return c_idx_not_exist;

// - STATE 7 -
state_7_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_17_label;
   }
   return c_idx_not_exist;

// - STATE 8 -
state_8_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_18_label;
   }
   return c_idx_not_exist;

// - STATE 9 -
state_9_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_19_label;
   }
   if (in_char == 121) {
      goto state_20_label;
   }
   return c_idx_not_exist;

// - STATE 10 -
state_10_label:
   VALIDATOR_CLOSE_CHAR(2);
   return 2;

// - STATE 11 -
state_11_label:
   VALIDATOR_CLOSE_CHAR(5);
   return 5;

// - STATE 12 -
state_12_label:
   VALIDATOR_CLOSE_CHAR(1);
   return 1;

// - STATE 13 -
state_13_label:
   VALIDATOR_CLOSE_CHAR(6);
   return 6;

// - STATE 14 -
state_14_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 108) {
      goto state_21_label;
   }
   return c_idx_not_exist;

// - STATE 15 -
state_15_label:
   VALIDATOR_CLOSE_CHAR(19);
   return 19;

// - STATE 16 -
state_16_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_22_label;
   }
   return c_idx_not_exist;

// - STATE 17 -
state_17_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 110) {
      goto state_23_label;
   }
   return c_idx_not_exist;

// - STATE 18 -
state_18_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 102) {
      goto state_24_label;
   }
   if (in_char == 103) {
      goto state_25_label;
   }
   return c_idx_not_exist;

// - STATE 19 -
state_19_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 115) {
      goto state_26_label;
   }
   return c_idx_not_exist;

// - STATE 20 -
state_20_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 112) {
      goto state_27_label;
   }
   return c_idx_not_exist;

// - STATE 21 -
state_21_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 45) {
      goto state_28_label;
   }
   return c_idx_not_exist;

// - STATE 22 -
state_22_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 109) {
      goto state_29_label;
   }
   return c_idx_not_exist;

// - STATE 23 -
state_23_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 103) {
      goto state_30_label;
   }
   return c_idx_not_exist;

// - STATE 24 -
state_24_label:
   VALIDATOR_CLOSE_CHAR(13);
   return 13;

// - STATE 25 -
state_25_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_31_label;
   }
   return c_idx_not_exist;

// - STATE 26 -
state_26_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 116) {
      goto state_32_label;
   }
   return c_idx_not_exist;

// - STATE 27 -
state_27_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_33_label;
   }
   return c_idx_not_exist;

// - STATE 28 -
state_28_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 105) {
      goto state_34_label;
   }
   if (in_char == 107) {
      goto state_35_label;
   }
   return c_idx_not_exist;

// - STATE 29 -
state_29_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 115) {
      goto state_36_label;
   }
   return c_idx_not_exist;

// - STATE 30 -
state_30_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 116) {
      goto state_37_label;
   }
   return c_idx_not_exist;

// - STATE 31 -
state_31_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 120) {
      goto state_38_label;
   }
   return c_idx_not_exist;

// - STATE 32 -
state_32_label:
   VALIDATOR_CLOSE_CHAR(15);
   return 15;

// - STATE 33 -
state_33_label:
   VALIDATOR_CLOSE_CHAR(0);
   return 0;

// - STATE 34 -
state_34_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 116) {
      goto state_39_label;
   }
   return c_idx_not_exist;

// - STATE 35 -
state_35_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_40_label;
   }
   return c_idx_not_exist;

// - STATE 36 -
state_36_label:
   VALIDATOR_CLOSE_CHAR(16);
   return 16;

// - STATE 37 -
state_37_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 104) {
      goto state_41_label;
   }
   return c_idx_not_exist;

// - STATE 38 -
state_38_label:
   VALIDATOR_CLOSE_CHAR(14);
   return 14;

// - STATE 39 -
state_39_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_42_label;
   }
   return c_idx_not_exist;

// - STATE 40 -
state_40_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 121) {
      goto state_43_label;
   }
   return c_idx_not_exist;

// - STATE 41 -
state_41_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 32) {
      goto state_44_label;
   }
   return c_idx_not_exist;

// - STATE 42 -
state_42_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 109) {
      goto state_45_label;
   }
   return c_idx_not_exist;

// - STATE 43 -
state_43_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 115) {
      goto state_46_label;
   }
   return c_idx_not_exist;

// - STATE 44 -
state_44_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 33) {
      goto state_47_label;
   }
   if (in_char == 60) {
      goto state_48_label;
   }
   if (in_char == 61) {
      goto state_49_label;
   }
   if (in_char == 62) {
      goto state_50_label;
   }
   return c_idx_not_exist;

// - STATE 45 -
state_45_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 115) {
      goto state_51_label;
   }
   return c_idx_not_exist;

// - STATE 46 -
state_46_label:
   VALIDATOR_CLOSE_CHAR(18);
   return 18;

// - STATE 47 -
state_47_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_52_label;
   }
   return c_idx_not_exist;

// - STATE 48 -
state_48_label:
   VALIDATOR_CLOSE_CHAR(9);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_53_label;
   }
   return 9;

// - STATE 49 -
state_49_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_54_label;
   }
   return c_idx_not_exist;

// - STATE 50 -
state_50_label:
   VALIDATOR_CLOSE_CHAR(10);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_55_label;
   }
   return 10;

// - STATE 51 -
state_51_label:
   VALIDATOR_CLOSE_CHAR(17);
   return 17;

// - STATE 52 -
state_52_label:
   VALIDATOR_CLOSE_CHAR(8);
   return 8;

// - STATE 53 -
state_53_label:
   VALIDATOR_CLOSE_CHAR(11);
   return 11;

// - STATE 54 -
state_54_label:
   VALIDATOR_CLOSE_CHAR(7);
   return 7;

// - STATE 55 -
state_55_label:
   VALIDATOR_CLOSE_CHAR(12);
   return 12;

}/*}}}*/

int validator_s_validate_pair(validator_s *this,var_s a_value,var_s a_props)
{/*{{{*/

  // - ERROR -
  if (a_props->v_type != c_bi_type_array)
  {
    throw_error(VALIDATOR_INVALID_PROPERTY_TYPE);
  }

  var_array_s *props_array = loc_s_array_value(a_props);

  // - ERROR -
  if (props_array->used & 0x01)
  {
    throw_error(VALIDATOR_INVALID_PROPERTIES_ARRAY_SIZE);
  }

  if (props_array->used != 0)
  {
    var_s *prop_ptr = props_array->data;
    var_s *prop_ptr_end = prop_ptr + props_array->used;

    do {

      // - retrieve property key and value -
      var_s prop_key = prop_ptr[0];
      var_s prop_value = prop_ptr[1];

#define VALIDATE_STACKS_PUSH_PROP_KEY() \
/*{{{*/\
  var_array_s_push(&this->props_stack,&prop_key);
/*}}}*/

      // - ERROR -
      if (prop_key->v_type != c_bi_type_string)
      {
        throw_error(VALIDATOR_INVALID_PROPERTY_TYPE);
      }

      unsigned prop_id = validator_s_recognize_property(loc_s_string_value(prop_key)->data);

      switch (prop_id)
      {
      case c_validator_prop_type:
      {/*{{{*/

        // - ERROR -
        if (prop_value->v_type != c_bi_type_integer)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          throw_error(VALIDATOR_INVALID_PROPERTY_TYPE);
        }

        // - ERROR -
        if (a_value->v_type != loc_s_int_value(prop_value))
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          throw_error(VALIDATOR_INVALID_VALUE_TYPE);
        }
      }/*}}}*/
      break;

      // - ERROR -
      default:
        VALIDATE_STACKS_PUSH_PROP_KEY();

        throw_error(VALIDATOR_INVALID_PROPERTY_ID);
      }
    } while((prop_ptr += 2) < prop_ptr_end);
  }

  return 0;
}/*}}}*/

int validator_s_validate(validator_s *this,var_s a_ref_name,var_s a_value)
{/*{{{*/

  // - ERROR -
  if (a_ref_name->v_type != c_bi_type_string)
  {
    throw_error(VALIDATOR_INVALID_REFERENCE_NAME_TYPE);
  }

  var_s props = loc_s_dict_get(this->schema,a_ref_name);

  // - ERROR -
  if (props == NULL)
  {
    throw_error(VALIDATOR_INVALID_VALIDATION_ENTRY);
  }

  this->value_stack.used = 0;
  this->props_stack.used = 0;

  // - ERROR -
  if (validator_s_validate_pair(this,a_value,props))
  {
    var_array_s_push(&this->props_stack,&a_ref_name);

    CONT_INIT_CLEAR(var_array_s,tmp_array);

#define VALIDATOR_S_VALIDATE_REVERSE_STACK(VALUE) \
{/*{{{*/\
  if ((VALUE).used > 1)\
  {\
    tmp_array.used = 0;\
\
    var_s *ptr = (VALUE).data + (VALUE).used;\
    var_s *ptr_end = (VALUE).data;\
    do {\
      var_array_s_push(&tmp_array,--ptr);\
    } while(ptr > ptr_end);\
\
    var_array_s_swap(&(VALUE),&tmp_array);\
  }\
}/*}}}*/

    VALIDATOR_S_VALIDATE_REVERSE_STACK(this->value_stack);
    VALIDATOR_S_VALIDATE_REVERSE_STACK(this->props_stack);

    throw_error(VALIDATOR_VALIDATION_ERROR);
  }

  return 0;
}/*}}}*/

