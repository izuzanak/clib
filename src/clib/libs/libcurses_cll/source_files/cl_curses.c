
@begin
include "cl_curses.h"
@end

// === global functions ========================================================

void libcurses_cll_init()
{/*{{{*/
  initscr();
}/*}}}*/

void libcurses_cll_clear()
{/*{{{*/
  endwin();
}/*}}}*/

