
#ifndef __CL_VALIDATOR_H
#define __CL_VALIDATOR_H

@begin
include "cl_var.h"
include "cl_regex.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libvalidator_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libvalidator_cll_EXPORTS
#define libvalidator_cll_EXPORT __declspec(dllexport)
#else
#define libvalidator_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_VALIDATOR_INVALID_SCHEMA_TYPE 1
#define ERROR_VALIDATOR_INVALID_PROPERTY_TYPE 2
#define ERROR_VALIDATOR_INVALID_PROPERTIES_ARRAY_SIZE 3
#define ERROR_VALIDATOR_INVALID_VALUE_TYPE 4
#define ERROR_VALIDATOR_INVALID_PROPERTY_ID 5
#define ERROR_VALIDATOR_INVALID_REFERENCE_NAME_TYPE 6
#define ERROR_VALIDATOR_INVALID_VALIDATION_ENTRY 7
#define ERROR_VALIDATOR_VALIDATION_ERROR 8

// === constants and definitions ===============================================

enum {
  c_validator_prop_type = 0,
  c_validator_prop_equal,
  c_validator_prop_not_equal,
  c_validator_prop_lesser,
  c_validator_prop_greater,
  c_validator_prop_lesser_equal,
  c_validator_prop_greater_equal,
  c_validator_prop_length_equal,
  c_validator_prop_length_not_equal,
  c_validator_prop_length_lesser,
  c_validator_prop_length_greater,
  c_validator_prop_length_lesser_equal,
  c_validator_prop_length_greater_equal,
  c_validator_prop_reference,
  c_validator_prop_regex,
  c_validator_prop_test,
  c_validator_prop_items,
  c_validator_prop_all_items,
  c_validator_prop_all_keys,
  c_validator_prop_in,
};

// === definition of generated structures ======================================

// -- regex_array_s --
@begin
array<regex_s> regex_array_s;
@end

// -- validator_s --
@begin
struct
<
var_s:schema
var_array_s:value_stack
var_array_s:props_stack

string_tree_s:regex_map
regex_array_s:regex_list
>
validator_s;
@end

WUR libvalidator_cll_EXPORT unsigned validator_s_recognize_property(const char *a_string);
WUR static inline int validator_s_create(validator_s *this,var_s a_schema);
WUR libvalidator_cll_EXPORT int validator_s_validate_pair(validator_s *this,var_s a_value,var_s a_props);
WUR libvalidator_cll_EXPORT int validator_s_validate(validator_s *this,var_s a_ref_name,var_s a_value);

// === inline methods of generated structures ==================================

// -- regex_array_s --
@begin
inlines regex_array_s
@end

// -- validator_s --
@begin
inlines validator_s
@end

static inline int validator_s_create(validator_s *this,var_s a_schema)
{/*{{{*/
  validator_s_clear(this);

  if (a_schema->v_type != c_bi_type_dict)
  {
    throw_error(VALIDATOR_INVALID_SCHEMA_TYPE);
  }

  var_s_copy_loc(&this->schema,a_schema);

  return 0;
}/*}}}*/

#endif

