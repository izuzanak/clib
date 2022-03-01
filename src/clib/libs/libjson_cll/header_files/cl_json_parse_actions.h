
#ifndef CL_LIB_JSON_PARSE_ACTIONS_H
#define CL_LIB_JSON_PARSE_ACTIONS_H

// === json parse actions access ===============================================

extern void (*json_pa_callers[])(json_parser_s *this);

// === json parse actions ======================================================

void pa_json_null(json_parser_s *this);
void pa_json_object_begin(json_parser_s *this);
void pa_json_object_pair(json_parser_s *this);
void pa_json_array_begin(json_parser_s *this);
void pa_json_array_value(json_parser_s *this);
void pa_json_val_string(json_parser_s *this);
void pa_json_val_integer(json_parser_s *this);
void pa_json_val_float(json_parser_s *this);
void pa_json_val_object(json_parser_s *this);
void pa_json_val_array(json_parser_s *this);
void pa_json_val_true(json_parser_s *this);
void pa_json_val_false(json_parser_s *this);
void pa_json_val_null(json_parser_s *this);
void pa_json_string(json_parser_s *this);

#endif

