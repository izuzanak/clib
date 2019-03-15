
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_var.h"
@end

// === definition of generated structures ======================================

// -- person_s --
@begin
struct
<
string_s:name
string_s:surname
ui:age
>
options ( to_json to_json_nice )
person_s;
@end

static inline var_s loc_s_person(const char *a_name,const char *a_surname,unsigned a_age);
static inline void loc_s_person_clear(var_s this);
static inline int loc_s_person_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_person_to_string(var_s this,bc_array_s *a_trg);
#endif
static inline void loc_s_person_to_json(var_s this,bc_array_s *a_trg);
static inline void loc_s_person_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg);

// === inline methods of generated structures ==================================

// -- person_s --
@begin
inlines person_s
@end

extern unsigned g_type_person;

static inline var_s loc_s_person(const char *a_name,const char *a_surname,unsigned a_age)
{/*{{{*/
  person_s *person = (person_s *)cmalloc(sizeof(person_s));
  person_s_init(person);

  string_s_set_ptr(&person->name,a_name);
  string_s_set_ptr(&person->surname,a_surname);
  person->age = a_age;

  var_s var = loc_s___new();
  var->v_type = g_type_person;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.ptr = person;

  return var;
}/*}}}*/

static inline void loc_s_person_clear(var_s this)
{/*{{{*/
  debug_assert(this->v_type == g_type_person);

  person_s *person = (person_s *)this->v_data.ptr;
  person_s_clear(person);
  cfree(person);
}/*}}}*/

static inline int loc_s_person_order(var_s a_first,var_s a_second)
{/*{{{*/
  debug_assert(a_first->v_type == g_type_person && a_second->v_type == g_type_person);

  string_s *f_surname = &((person_s *)a_first->v_data.ptr)->surname;
  string_s *s_surname = &((person_s *)a_second->v_data.ptr)->surname;

  if (f_surname->size < s_surname->size) { return -1; }
  if (f_surname->size > s_surname->size) { return 1; }

  return memcmp(f_surname->data,s_surname->data,f_surname->size - 1);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void loc_s_person_to_string(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == g_type_person);

  person_s *person = (person_s *)this->v_data.ptr;
  person_s_to_string(person,a_trg);
}/*}}}*/
#endif

static inline void loc_s_person_to_json(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == g_type_person);

  person_s *person = (person_s *)this->v_data.ptr;
  person_s_to_json(person,a_trg);
}/*}}}*/

static inline void loc_s_person_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == g_type_person);

  person_s *person = (person_s *)this->v_data.ptr;
  person_s_to_json_nice(person,a_json_nice,a_trg);
}/*}}}*/

// === test execution functions ================================================

void test_var_blank();
void test_var_integer();
void test_var_float();
void test_var_string();
void test_var_array();
void test_var_dict();
void test_register_type();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

