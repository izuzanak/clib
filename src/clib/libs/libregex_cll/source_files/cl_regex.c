
@begin
include "cl_regex.h"
@end

// === methods of generated structures =========================================

// -- regmatch_array_s --
@begin
methods regmatch_array_s
@end

// === methods of structure regex_s ============================================

int regex_s_match_n(regex_s *this,const char *a_data,size_t a_match_cnt,regmatch_array_s *a_matches)
{/*{{{*/
  debug_assert(*this != NULL);

  // - reset matches array -
  a_matches->used = 0;

  if (a_match_cnt > 0)
  {
    regmatch_array_s_reserve(a_matches,a_match_cnt);

    // - execute regular expression -
    if (regexec(*this,a_data,a_match_cnt,a_matches->data,0) != 0)
    {
      return 0;
    }

    regmatch_s *m_ptr = a_matches->data;
    regmatch_s *m_ptr_end = m_ptr + a_match_cnt;
    do {

      // - first no match -
      if (m_ptr->rm_so == -1)
      {
        break;
      }
    } while(++m_ptr < m_ptr_end);

    a_matches->used = m_ptr - a_matches->data;
  }

  return 1;
}/*}}}*/

int regex_s_split(regex_s *this,const string_s *a_src,string_array_s *a_trg)
{/*{{{*/
  debug_assert(*this != NULL);

  a_trg->used = 0;

  // - create match target structure -
  regmatch_t match;

  // - split string to array -
  {
    unsigned idx = 0;
    do
    {
      unsigned old_idx = idx;

      // - if match was found -
      if (regexec(*this,a_src->data + idx,1,&match,0) == 0)
      {
        // - ERROR -
        if (match.rm_so == match.rm_eo)
        {
          throw_error(REGEX_EMPTY_MATCH);
        }

        idx += match.rm_so;
      }

      // - if match was not found -
      else
      {
        idx = a_src->size - 1;
      }

      string_array_s_push_blank(a_trg);
      string_s_set(string_array_s_last(a_trg),idx - old_idx,a_src->data + old_idx);

      if (idx >= a_src->size - 1)
      {
        break;
      }

      // - jump over matched string -
      idx += match.rm_eo - match.rm_so;
    }
    while(1);
  }

  return 0;
}/*}}}*/

int regex_s_replace(regex_s *this,const string_s *a_src,const string_s *a_new,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(*this != NULL);

  a_trg->used = 0;

  // - replace regular expression matches by new string -
  {
    regmatch_t match;

    unsigned pos = 0;
    do {
      unsigned old_pos = pos;

      // - search for substring -
      if (regexec(*this,a_src->data + pos,1,&match,0) == 0)
      {
        // - ERROR -
        if (match.rm_so == match.rm_eo)
        {
          throw_error(REGEX_EMPTY_MATCH);
        }

        pos += match.rm_so;
      }
      else
      {
        pos = a_src->size - 1;
      }

      // - append part of original string to result -
      bc_array_s_append(a_trg,pos - old_pos,a_src->data + old_pos);

      if (pos >= a_src->size - 1)
      {
        break;
      }

      // - append new string to result -
      bc_array_s_append(a_trg,a_new->size - 1,a_new->data);

      // - jump over substring -
      pos += match.rm_eo - match.rm_so;

    } while(1);
  }

  return 0;
}/*}}}*/

