
@begin
include "main.h"
@end

volatile int g_terminate = 0;

// === global functions ========================================================

void signal_handler(int a_signum)
{/*{{{*/
  switch (a_signum)
  {
  case SIGCHLD:
    {
      int status;
      wait(&status);
    }
    break;
  default:

    // - terminate on all signals -
    __sync_add_and_fetch(&g_terminate,1);
  }
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  cassert(signal_s_simple_handler(signal_handler) == 0);

  {
    struct fuse_lowlevel_ops fuse_oper = {};

    CONT_INIT_CLEAR(string_array_s,args);
    string_array_s_push_ptr(&args,"create");

    CONT_INIT_CLEAR(fuse_session_s,session);
    cassert(fuse_session_s_create(&session,&args,&fuse_oper,NULL) == 0);
    //cassert(fuse_session_s_mount(&session,"tests/libfuse_cll_test/resources/fuse_dir") == 0);
    cassert(fuse_session_s_mount(&session,"/dev/shm/test") == 0);

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,0);

    do {

      // - wait on events -
      int err;
      if ((err = epoll_s_wait(&epoll,16,500)))
      {
        cassert(err == ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED);
      }

      fprintf(stderr,"epoll looping ...\n");
    } while(g_terminate == 0);
  }

  memcheck_release_assert();

  return 0;
}/*}}}*/

