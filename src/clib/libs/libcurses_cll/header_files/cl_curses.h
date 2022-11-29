
#ifndef CL_LIB_CURSES_H
#define CL_LIB_CURSES_H

@begin
include "cl_struct.h"
@end

#include <curses.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libcurses_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libcurses_cll_EXPORTS
#define libcurses_cll_EXPORT __declspec(dllexport)
#else
#define libcurses_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_CURSES_CBREAK_ERROR 1
#define ERROR_CURSES_ECHO_ERROR 2
#define ERROR_CURSES_KEYPAD_ERROR 3
#define ERROR_CURSES_NL_ERROR 4
#define ERROR_CURSES_RAW_ERROR 5
#define ERROR_CURSES_MOVEXY_ERROR 6
#define ERROR_CURSES_ADDCH_ERROR 7
#define ERROR_CURSES_ADDSTR_ERROR 8
#define ERROR_CURSES_ATTR_ERROR 9
#define ERROR_CURSES_ATTRSET_ERROR 10
#define ERROR_CURSES_GETCH_ERROR 11
#define ERROR_CURSES_GETSTR_ERROR 12
#define ERROR_CURSES_CLEAR_ERROR 13
#define ERROR_CURSES_REFRESH_ERROR 14

// === definition of structure curses_s ========================================

WUR static inline int curses_s_cbreak(int a_value);
WUR static inline int curses_s_echo(int a_value);
WUR static inline int curses_s_keypad(int a_value);
WUR static inline int curses_s_nl(int a_value);
WUR static inline int curses_s_raw(int a_value);
WUR static inline int curses_s_movexy(int a_x,int a_y);
static inline void curses_s_curxy(int *a_x,int *a_y);
static inline void curses_s_sizexy(int *a_x,int *a_y);
WUR static inline int curses_s_addch(chtype a_char);
WUR static inline int curses_s_addstr(const char *a_data,int a_length);
WUR static inline int curses_s_attr(int a_offon,int a_attrs);
WUR static inline int curses_s_attrset(int a_attrs);
WUR static inline int curses_s_getch(int *a_char);
WUR static inline int curses_s_getstr(unsigned a_length,bc_array_s *a_trg);
WUR static inline int curses_s_clear();
WUR static inline int curses_s_clrtobot();
WUR static inline int curses_s_clrtoeol();
WUR static inline int curses_s_refresh();

// === definition of global functions ==========================================

void libcurses_cll_init();
void libcurses_cll_clear();

// === inline methods of structure curses_s ====================================

static inline int curses_s_cbreak(int a_value)
{/*{{{*/

  // - ERROR -
  if ((a_value ? cbreak() : nocbreak()) == ERR)
  {
    throw_error(CURSES_CBREAK_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_echo(int a_value)
{/*{{{*/

  // - ERROR -
  if ((a_value ? echo() : noecho()) == ERR)
  {
    throw_error(CURSES_ECHO_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_keypad(int a_value)
{/*{{{*/

  // - ERROR -
  if (keypad(stdscr,a_value != 0) == ERR)
  {
    throw_error(CURSES_KEYPAD_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_nl(int a_value)
{/*{{{*/

  // - ERROR -
  if ((a_value ? nl() : nonl()) == ERR)
  {
    throw_error(CURSES_NL_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_raw(int a_value)
{/*{{{*/

  // - ERROR -
  if ((a_value ? raw() : noraw()) == ERR)
  {
    throw_error(CURSES_RAW_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_movexy(int a_x,int a_y)
{/*{{{*/

  // - ERROR -
  if (a_x < 0 || a_y < 0 || move(a_y,a_x) == ERR)
  {
    throw_error(CURSES_MOVEXY_ERROR);
  }

  return 0;
}/*}}}*/

static inline void curses_s_curxy(int *a_x,int *a_y)
{/*{{{*/
  getyx(stdscr,(*a_y),(*a_x));
}/*}}}*/

static inline void curses_s_sizexy(int *a_x,int *a_y)
{/*{{{*/
  getmaxyx(stdscr,(*a_y),(*a_x));
}/*}}}*/

static inline int curses_s_addch(chtype a_char)
{/*{{{*/

  // - ERROR -
  if (addch(a_char) == ERR)
  {
    throw_error(CURSES_ADDCH_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_addstr(const char *a_data,int a_length)
{/*{{{*/

  // - ERROR -
  if (addnstr(a_data,a_length) == ERR)
  {
    throw_error(CURSES_ADDSTR_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_attr(int a_offon,int a_attrs)
{/*{{{*/

  // - ERROR -
  if ((a_offon ? attron(a_attrs) : attroff(a_attrs)) == ERR)
  {
    throw_error(CURSES_ATTR_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_attrset(int a_attrs)
{/*{{{*/

  // - ERROR -
  if (attrset(a_attrs) == ERR)
  {
    throw_error(CURSES_ATTRSET_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_getch(int *a_char)
{/*{{{*/

  // - ERROR -
  if ((*a_char = getch()) == ERR)
  {
    throw_error(CURSES_GETCH_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_getstr(unsigned a_length,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_reserve(a_trg,a_length + 1);

  // - ERROR -
  if (getnstr(a_trg->data + a_trg->used,a_length) == ERR)
  {
    throw_error(CURSES_GETSTR_ERROR);
  }

  // - adjust buffer size -
  a_trg->used += strlen(a_trg->data + a_trg->used);

  return 0;
}/*}}}*/

static inline int curses_s_clear()
{/*{{{*/

  // - ERROR -
  if (clear() == ERR)
  {
    throw_error(CURSES_CLEAR_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_clrtobot()
{/*{{{*/

  // - ERROR -
  if (clrtobot() == ERR)
  {
    throw_error(CURSES_CLEAR_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_clrtoeol()
{/*{{{*/

  // - ERROR -
  if (clrtoeol() == ERR)
  {
    throw_error(CURSES_CLEAR_ERROR);
  }

  return 0;
}/*}}}*/

static inline int curses_s_refresh()
{/*{{{*/

  // - ERROR -
  if (refresh() == ERR)
  {
    throw_error(CURSES_REFRESH_ERROR);
  }

  return 0;
}/*}}}*/

#endif

