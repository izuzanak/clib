
@begin
include "main.h"
@end

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  // TODO continue ...

  memcheck_release_assert();

  return 0;
}/*}}}*/

