
@begin
include "cl_sys.h"
@end

// === methods of structure stream_s ===========================================

int stream_s_read(stream_s *this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(*this != NULL);

  const unsigned c_buffer_add = 1024;

  unsigned read_cnt;
  do
  {
    unsigned old_used = a_trg->used;
    bc_array_s_push_blanks(a_trg,c_buffer_add);
    read_cnt = fread(a_trg->data + old_used,1,c_buffer_add,*this);
  }
  while(read_cnt >= c_buffer_add);

  // - ERROR -
  if (ferror(*this))
  {
    throw_error(STREAM_READ_ERROR);
  }

  a_trg->used = (a_trg->used - c_buffer_add) + read_cnt;

  return 0;
}/*}}}*/

int stream_s_read_max(stream_s *this,bc_array_s *a_trg,size_t a_size)
{/*{{{*/
  debug_assert(*this != NULL);

  // - prepare target buffer -
  bc_array_s_reserve(a_trg,a_size);

  unsigned read_cnt = fread(a_trg->data + a_trg->used,1,a_size,*this);

  if (read_cnt < a_size)
  {
    // - ERROR -
    if (ferror(*this))
    {
      throw_error(STREAM_READ_ERROR);
    }
  }

  a_trg->used += read_cnt;

  return 0;
}/*}}}*/

// === methods of structure environment_s ======================================

unsigned environment_s_recognize_terminal(const char **a_src,const char *a_src_end) // NOLINT
{/*{{{*/
#define ENVIRONMENT_VARS_GET_NEXT_CHAR() \
{\
  in_char = (unsigned char)**a_src;\
}

#define ENVIRONMENT_VARS_CLOSE_CHAR(RET_TERM_IDX) \
{\
   if (*a_src >= a_src_end) {\
      return RET_TERM_IDX;\
   }\
\
   ++*a_src;\
}

   unsigned char in_char;

// - STATE 0 -
   ENVIRONMENT_VARS_GET_NEXT_CHAR();

   if (in_char < 36) {
      goto state_1_label;
   }
   if (in_char == 36) {
      goto state_2_label;
   }
   goto state_1_label;

// - STATE 1 -
state_1_label:
   ENVIRONMENT_VARS_CLOSE_CHAR(3);
   ENVIRONMENT_VARS_GET_NEXT_CHAR();

   if (in_char < 36) {
      goto state_1_label;
   }
   if (in_char >= 37) {
      goto state_1_label;
   }
   return 3;

// - STATE 2 -
state_2_label:
   ENVIRONMENT_VARS_CLOSE_CHAR(c_idx_not_exist);
   ENVIRONMENT_VARS_GET_NEXT_CHAR();

   if (in_char < 36) {
      goto state_1_label;
   }
   if (in_char == 36) {
      goto state_3_label;
   }
   if (in_char < 123) {
      goto state_1_label;
   }
   if (in_char == 123) {
      goto state_4_label;
   }
   goto state_1_label;

// - STATE 3 -
state_3_label:
   ENVIRONMENT_VARS_CLOSE_CHAR(2);
   return 2;

// - STATE 4 -
state_4_label:
   ENVIRONMENT_VARS_CLOSE_CHAR(c_idx_not_exist);
   ENVIRONMENT_VARS_GET_NEXT_CHAR();

   if (in_char < 58) {
      goto state_4_label;
   }
   if (in_char == 58) {
      goto state_5_label;
   }
   if (in_char < 125) {
      goto state_4_label;
   }
   if (in_char == 125) {
      goto state_6_label;
   }
   goto state_4_label;

// - STATE 5 -
state_5_label:
   ENVIRONMENT_VARS_CLOSE_CHAR(c_idx_not_exist);
   ENVIRONMENT_VARS_GET_NEXT_CHAR();

   if (in_char < 45) {
      goto state_7_label;
   }
   if (in_char == 45) {
      goto state_8_label;
   }
   if (in_char < 58) {
      goto state_7_label;
   }
   if (in_char == 58) {
      goto state_9_label;
   }
   if (in_char < 125) {
      goto state_7_label;
   }
   if (in_char == 125) {
      goto state_6_label;
   }
   goto state_7_label;

// - STATE 6 -
state_6_label:
   ENVIRONMENT_VARS_CLOSE_CHAR(0);
   return 0;

// - STATE 7 -
state_7_label:
   ENVIRONMENT_VARS_CLOSE_CHAR(c_idx_not_exist);
   ENVIRONMENT_VARS_GET_NEXT_CHAR();

   if (in_char < 58) {
      goto state_7_label;
   }
   if (in_char == 58) {
      goto state_9_label;
   }
   if (in_char < 125) {
      goto state_7_label;
   }
   if (in_char == 125) {
      goto state_6_label;
   }
   goto state_7_label;

// - STATE 8 -
state_8_label:
   ENVIRONMENT_VARS_CLOSE_CHAR(c_idx_not_exist);
   ENVIRONMENT_VARS_GET_NEXT_CHAR();

   if (in_char < 58) {
      goto state_8_label;
   }
   if (in_char == 58) {
      goto state_10_label;
   }
   if (in_char < 125) {
      goto state_8_label;
   }
   if (in_char == 125) {
      goto state_11_label;
   }
   goto state_8_label;

// - STATE 9 -
state_9_label:
   ENVIRONMENT_VARS_CLOSE_CHAR(c_idx_not_exist);
   ENVIRONMENT_VARS_GET_NEXT_CHAR();

   if (in_char < 58) {
      goto state_7_label;
   }
   if (in_char == 58) {
      goto state_9_label;
   }
   if (in_char < 125) {
      goto state_7_label;
   }
   if (in_char == 125) {
      goto state_6_label;
   }
   goto state_7_label;

// - STATE 10 -
state_10_label:
   ENVIRONMENT_VARS_CLOSE_CHAR(c_idx_not_exist);
   ENVIRONMENT_VARS_GET_NEXT_CHAR();

   if (in_char < 58) {
      goto state_8_label;
   }
   if (in_char == 58) {
      goto state_10_label;
   }
   if (in_char < 125) {
      goto state_8_label;
   }
   if (in_char == 125) {
      goto state_11_label;
   }
   goto state_8_label;

// - STATE 11 -
state_11_label:
   ENVIRONMENT_VARS_CLOSE_CHAR(1);
   return 1;

}/*}}}*/

int environment_s_resolve_vars(unsigned a_size,const char *a_src,bc_array_s *a_trg)
{/*{{{*/
  CONT_INIT_CLEAR(bc_array_s,name);
  char *value = NULL;

  const char *a_src_end = a_src + a_size;
  while (a_src < a_src_end)
  {
    const char *begin = a_src;
    unsigned terminal = environment_s_recognize_terminal(&a_src,a_src_end);

    switch (terminal)
    {
      case c_env_term_VAR:
        {/*{{{*/
          name.used = 0;
          bc_array_s_append(&name,a_src - begin - 3,begin + 2);
          bc_array_s_push(&name,'\0');

          // - retrieve environment value -
          if ((value = getenv(name.data)) == NULL)
          {
            throw_error(EVIRONMENT_RESOLVE_VARS_ERROR);
          }

          // - append variable value -
          bc_array_s_append_ptr(a_trg,value);
        }/*}}}*/
        break;
      case c_env_term_VAR_DEF:
        {/*{{{*/
          const char *def_begin = strstr(begin,":-");

          name.used = 0;
          bc_array_s_append(&name,def_begin - begin - 2,begin + 2);
          bc_array_s_push(&name,'\0');

          // - retrieve environment value -
          if ((value = getenv(name.data)) != NULL)
          {
            // - append variable value -
            bc_array_s_append_ptr(a_trg,value);
          }
          else
          {
            // - append default value -
            bc_array_s_append(a_trg,a_src - def_begin  - 3,def_begin + 2);
          }
        }/*}}}*/
        break;
      case c_env_term_ESCAPE:
        bc_array_s_push(a_trg,'$');
        break;
      case c_env_term_TEXT:
        bc_array_s_append(a_trg,a_src - begin,begin);
        break;
      default:
        throw_error(EVIRONMENT_RESOLVE_VARS_ERROR);
        break;
    }
  }

  return 0;
}/*}}}*/

// === methods of generated structures =========================================

// -- pollfd_array_s --
@begin
methods pollfd_array_s
@end

