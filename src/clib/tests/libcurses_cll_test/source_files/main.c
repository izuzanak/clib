
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libcurses_cll_test";

const char *test_names[] =
{/*{{{*/
  "create",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_create,
};/*}}}*/

// === test execution functions ================================================

void test_create()
{/*{{{*/
  CONT_INIT_CLEAR(bc_array_s,buffer);

  cassert(!(
        curses_s_cbreak(0) ||
        curses_s_echo(0)   ||
        curses_s_raw(1)    ||
        curses_s_keypad(1)));

  int max_x;
  int max_y;
  curses_s_maxxy(&max_x,&max_y);

  int step = 10;

  int cur_y = 0;
  do {
    int cur_x = 0;
    do {
      buffer.used = 0;
      bc_array_s_append_format(&buffer,"[%3.3d,%3.3d] ",cur_x,cur_y);

      cassert(!(
            curses_s_movexy(cur_x,cur_y) ||
            curses_s_addstr(buffer.data,buffer.used)));

    } while((cur_x += step) < (max_x - step));
  } while(++cur_y < max_y);

  cassert(curses_s_movexy(0,0) == 0);

  int ch;
  cassert(curses_s_getch(&ch) == 0);

}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libcurses_cll_init(),libcurses_cll_clear());

  return 0;
}/*}}}*/

