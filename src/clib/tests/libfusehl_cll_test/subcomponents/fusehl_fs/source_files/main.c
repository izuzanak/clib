
@begin
include "main.h"
@end

const size_t c_file_size = 1024;

// === methods of generated structures =========================================

// === global functions ========================================================

static void *fusehl_fs_init(struct fuse_conn_info *conn,struct fuse_config *cfg)
{/*{{{*/
  (void)conn;

  fprintf(stderr,"fusehl_fs_init\n");

  cfg->direct_io = 1;

  return NULL;
}/*}}}*/

static int fusehl_fs_getattr(const char *path,struct stat *stbuf,struct fuse_file_info *fi)
{/*{{{*/
  fprintf(stderr,"fusehl_fs_getattr, path: %s, fi: %p\n",path,fi);

  int res = 0;

  memset(stbuf,0,sizeof(struct stat));

  if (strcmp(path,"/") == 0)
  {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
  }
  else if (strcmp(path,"/file.txt") == 0)
  {
    stbuf->st_mode = S_IFREG | 0444;
    stbuf->st_nlink = 1;
    stbuf->st_size = 0;
  }
  else
  {
    res = -ENOENT;
  }

  return res;
}/*}}}*/

static int fusehl_fs_readdir(const char *path,void *buf,fuse_fill_dir_t filler,
    off_t offset,struct fuse_file_info *fi,enum fuse_readdir_flags flags)
{/*{{{*/
  (void)offset;
  (void)flags;

  fprintf(stderr,"fusehl_fs_readdir, path: %s, fi: %p\n",path,fi);

  if (strcmp(path,"/") != 0)
    return -ENOENT;

  filler(buf,".",NULL,0,0);
  filler(buf,"..",NULL,0,0);
  filler(buf,"file.txt",NULL,0,0);

  return 0;
}/*}}}*/

static int fusehl_fs_open(const char *path,struct fuse_file_info *fi)
{/*{{{*/
  fprintf(stderr,"fusehl_fs_open, path: %s, fi: %p\n",path,fi);

  if (strcmp(path,"/file.txt") != 0)
  {
    return -ENOENT;
  }

  if ((fi->flags & O_ACCMODE) != O_RDONLY)
  {
    return -EACCES;
  }

  fi->direct_io = 1;
  fi->nonseekable = 1;

  return 0;
}/*}}}*/

static int fusehl_fs_release(const char *path,struct fuse_file_info *fi)
{/*{{{*/
  fprintf(stderr,"fusehl_fs_release, path: %s, fi: %p\n",path,fi);

  return 0;
}/*}}}*/

static int fusehl_fs_read(const char *path,char *buf,size_t size,off_t offset,
    struct fuse_file_info *fi)
{/*{{{*/
  fprintf(stderr,"fusehl_fs_read, path: %s, size: %zu, offset: %zd, fi: %p\n",
      path,size,offset,fi);

  if(strcmp(path,"/file.txt") != 0)
  {
    return -ENOENT;
  }

  if (offset < c_file_size)
  {
    if (offset + size > c_file_size)
    {
      size = c_file_size - offset;
    }

    memset(buf,'0',size);
  }
  else
  {
    size = 0;
  }

  return size;
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  {
    struct fuse_operations fuse_oper =
    {
      .init    = fusehl_fs_init,
      .getattr = fusehl_fs_getattr,
      .readdir = fusehl_fs_readdir,
      .open    = fusehl_fs_open,
      .release = fusehl_fs_release,
      .read    = fusehl_fs_read,
    };

    CONT_INIT_CLEAR(string_array_s,args);
    string_array_s_push_ptr(&args,"fusehl_fs");
    string_array_s_push_ptr(&args,"-f");
    string_array_s_push_ptr(&args,"-s");
    string_array_s_push_ptr(&args,"-o");
    string_array_s_push_ptr(&args,"allow_other");
    string_array_s_push_ptr(&args,"/dev/shm/test");

    char *fuse_args[args.used + 1];
    char **fa_ptr = fuse_args;

    // - process arguments -
    if (args.used != 0)
    {
      string_s *a_ptr = args.data;
      string_s *a_ptr_end = a_ptr + args.used;
      do
      {
        // - set argument -
        *fa_ptr++ = a_ptr->data;
      } while(++a_ptr < a_ptr_end);
    }

    // - set last argument to null -
    *fa_ptr = NULL;

    fuse_main(args.used,fuse_args,&fuse_oper,NULL);
  }

  memcheck_release_assert();

  return 0;
}/*}}}*/

