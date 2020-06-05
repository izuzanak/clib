
@begin
include "main.h"
@end

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// === global functions ========================================================

int fuse_fs_stat(fuse_ino_t ino,struct stat *stbuf)
{/*{{{*/
  switch (ino)
  {
  case 1:
    stbuf->st_ino = ino;
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
    break;
  case 2:
    stbuf->st_ino = ino;
    stbuf->st_mode = S_IFREG | 0444;
    stbuf->st_nlink = 1;
    stbuf->st_atime = time(NULL);
    stbuf->st_mtime = stbuf->st_atime;
    stbuf->st_ctime = stbuf->st_atime;
    break;
  default:
    return 1;
  }

  return 0;
}/*}}}*/

void fuse_fs_lookup(fuse_req_t req,fuse_ino_t parent,const char *name)
{/*{{{*/
  fprintf(stderr,"fuse_fs_lookup\n");

  switch (parent)
  {
  case 1:
    if (strcmp(name,"numbers") == 0)
    {
      struct fuse_entry_param entry = {};
      entry.ino = 2;
      entry.attr_timeout = 1.0;
      entry.entry_timeout = 1.0;
      fuse_fs_stat(entry.ino,&entry.attr);
      fuse_reply_entry(req,&entry);
    }
    else
    {
      throw_fuse_error(req,ENOENT);
    }
    break;
  default:
    throw_fuse_error(req,ENOENT);
  }
}/*}}}*/

void fuse_fs_getattr(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi)
{/*{{{*/
  (void)fi;

  fprintf(stderr,"fuse_fs_getattr\n");

  struct stat stbuf = {};
  if (fuse_fs_stat(ino,&stbuf))
  {
    throw_fuse_error(req,ENOENT);
    return;
  }

  fuse_reply_attr(req,&stbuf,1.0);
}/*}}}*/

void fuse_fs_opendir(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi)
{/*{{{*/
  fprintf(stderr,"fuse_fs_opendir\n");

  switch (ino)
  {
  case 1:
    fuse_reply_open(req,fi);
    break;
  default:
    throw_fuse_error(req,ENOTDIR);
  }
}/*}}}*/

void fuse_fs_readdir(fuse_req_t req,fuse_ino_t ino,size_t size,off_t off,struct fuse_file_info *fi)
{/*{{{*/
  (void)off;
  (void)fi;

  fprintf(stderr,"fuse_fs_readdir\n");

  switch (ino)
  {
  case 1:
    {
      CONT_INIT_CLEAR(bc_array_s,dirbuf);

      fuse_dirbuff_s_add(&dirbuf,req,".",1);
      fuse_dirbuff_s_add(&dirbuf,req,"..",1);
      fuse_dirbuff_s_add(&dirbuf,req,"numbers",2);

      fuse_dirbuff_s_reply(&dirbuf,req,size,off);
    }
    break;
  default:
    throw_fuse_error(req,ENOTDIR);
  }
}/*}}}*/

void fuse_fs_open(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi)
{/*{{{*/
  fprintf(stderr,"fuse_fs_open\n");

  if ((fi->flags & O_ACCMODE) != O_RDONLY)
  {
    throw_fuse_error(req,EACCES);
    return;
  }

  switch (ino)
  {
  case 2:
    fi->direct_io = 1;
    fi->nonseekable = 1;

    unsigned *counter = cmalloc(sizeof(unsigned));
    *counter = 0;
    fi->fh = (uintptr_t)counter;

    fuse_reply_open(req,fi);
    break;
  default:
    throw_fuse_error(req,EACCES);
  }
}/*}}}*/

void fuse_fs_release(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi)
{/*{{{*/
  (void)req;
  (void)ino;
  (void)fi;

  fprintf(stderr,"fuse_fs_release\n");

  unsigned *counter = (unsigned *)(uintptr_t)fi->fh;
  cfree(counter);

  fuse_reply_err(req,0);
}/*}}}*/

void fuse_fs_read(fuse_req_t req,fuse_ino_t ino,size_t size,off_t off,struct fuse_file_info *fi)
{/*{{{*/
  (void)off;

  fprintf(stderr,"fuse_fs_read, off: %llu, size: %" HOST_LL_FORMAT "u\n",(ulli)off,(ulli)size);

  if (ino == 2)
  {
    CONT_INIT_CLEAR(bc_array_s,buffer);

    unsigned *counter = (unsigned *)(uintptr_t)fi->fh;

    while (*counter < 100000)
    {
      unsigned old_used = buffer.used;
      ui_to_string(counter,&buffer);
      bc_array_s_push(&buffer,'\n');

      if (buffer.used > size)
      {
        buffer.used = old_used;
        break;
      }

      ++*counter;
    }

    fuse_reply_buf(req,buffer.data,buffer.used);
  }
}/*}}}*/

int fuse_fd_event(void *a_fuse_session,unsigned a_index,epoll_event_s *a_event)
{/*{{{*/
  (void)a_index;
  (void)a_event;

  fuse_session_s *session = (fuse_session_s *)a_fuse_session;
  return fuse_session_s_process(session);
}/*}}}*/

void signal_handler(int a_signum)
{/*{{{*/
  switch (a_signum)
  {
  case SIGCHLD:
    break;
  default:

    // - terminate on all signals -
    __sync_add_and_fetch(&g_terminate,1);
  }
}/*}}}*/

int epoll_fd_update(int a_fd,unsigned a_evts,int a_update_cb,const epoll_callback_s *a_callback)
{/*{{{*/
  return epoll_s_fd_update(g_epoll,a_fd,a_evts,a_update_cb,a_callback);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    struct fuse_lowlevel_ops fuse_oper =
    {
      .lookup     = fuse_fs_lookup,
      .getattr    = fuse_fs_getattr,
      .opendir    = fuse_fs_opendir,
      .readdir    = fuse_fs_readdir,
      .open       = fuse_fs_open,
      .release    = fuse_fs_release,
      .read       = fuse_fs_read,
    };

    CONT_INIT_CLEAR(string_array_s,args);
    string_array_s_push_ptr(&args,"fuse_fs");

    CONT_INIT_CLEAR(fuse_session_s,session);
    cassert(fuse_session_s_create(&session,&args,&fuse_oper,NULL) == 0);
    //cassert(fuse_session_s_mount(&session,"tests/libfuse_cll_test/resources/fuse_dir") == 0);
    cassert(fuse_session_s_mount(&session,"/dev/shm/test") == 0);

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,0);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(epoll_fd_s,epoll_fd);
    cassert((epoll_fd.fd = dup(fuse_session_s_fd(&session))) != -1);
    cassert(epoll_s_fd_callback(&epoll_fd,EPOLLIN | EPOLLPRI,fuse_fd_event,&session,0) == 0);

    do {

      // - wait on events -
      int err;
      if ((err = epoll_s_wait(g_epoll,500)))
      {
        cassert(err == ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED);
      }

      if (fuse_session_exited(session.se))
      {
        g_terminate = 1;
      }

    } while(g_terminate == 0);
  }

  memcheck_release_assert();

  return 0;
}/*}}}*/

