
#ifndef OD_PROG_MAIN_H
#define OD_PROG_MAIN_H

@begin
include "cl_process.h"
include "cl_fuse.h"
include "od_conf.h"
include "od_channel.h"
include "od_odb.h"
@end

// - error codes -
#define ERROR_OD_FUSER_CONFIG_DATA_ERROR 1
#define ERROR_OD_FUSER_EPOLL_WAIT_ERROR 2

#define ERROR_OD_FUSER_CHANNEL_CREATE_ERROR 1
#define ERROR_OD_FUSER_CHANNEL_SEND_MESSAGE_ERROR 2
#define ERROR_OD_FUSER_MOUNTPOINT_CREATE_ERROR 3
#define ERROR_OD_FUSER_FUSE_SESSION_CREATE_ERROR 4

// === constants and definitions ===============================================

#define ODB_RECORD_HEADER_SIZE (sizeof(unsigned) + sizeof(usi))

// === definition of generated structures ======================================

// -- ino_name_s --
@begin
struct
<
ulli:parent
string_s:name
>
ino_name_s;
@end

// -- ino_name_tree_s --
@begin
rb_tree<ino_name_s> ino_name_tree_s;
@end

// -- file_list_s --
@begin
list<bc_array_s> file_list_s;
@end

// -- od_fuser_s --
@begin
struct
<
bi:config_changed_flag
od_config_s:config
od_config_s:last_config

odb_database_s:database
od_channel_client_s:channel

ino_name_tree_s:ino_name_tree
file_list_s:open_files

$// -- fuser --
fuse_session_s:fuse_session
epoll_fd_s:fuse_epoll_fd
bc_array_s:fuse_dirbuff

$// - temporary buffers -
ui_array_s:ino_stack
bc_array_s:path_buffer
bc_array_s:buffer
>
od_fuser_s;
@end

WUR int od_fuser_s_create(od_fuser_s *this);
WUR int od_fuser_s_process_config(od_fuser_s *this);
WUR int od_fuser_s_run(od_fuser_s *this);

void od_fuser_s_buffer_ino_path(od_fuser_s *this,unsigned a_ino);
void od_fuser_s_set_ino_value(od_fuser_s *this,unsigned a_ino,var_s a_value_var);
void od_fuser_s_get_ino_value(od_fuser_s *this,unsigned a_ino,var_s *a_value_var);
void od_fuser_s_stat(od_fuser_s *this,unsigned ino,struct stat *stbuf);

WUR int od_fuser_s_channel_callback(void *a_od_fuser,unsigned a_index,unsigned a_type,va_list a_ap);

// -- fuser --
WUR int od_fuser_s_fuse_fd_event(void *a_od_fuser,unsigned a_index,epoll_event_s *a_epoll_event);

void od_fuser_s_lookup(fuse_req_t req,fuse_ino_t parent,const char *name);
void od_fuser_s_getattr(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi);
void od_fuser_s_opendir(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi);
void od_fuser_s_releasedir(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi);
void od_fuser_s_readdir(fuse_req_t req,fuse_ino_t ino,size_t size,off_t off,struct fuse_file_info *fi);
void od_fuser_s_open(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi);
void od_fuser_s_release(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi);
void od_fuser_s_read(fuse_req_t req,fuse_ino_t ino,size_t size,off_t off,struct fuse_file_info *fi);
void od_fuser_s_write(fuse_req_t req,fuse_ino_t ino,const char *buf,size_t size,off_t off,struct fuse_file_info *fi);

// === inline methods of generated structures ==================================

// -- ino_name_s --
@begin
inlines ino_name_s
@end

// -- ino_name_tree_s --
@begin
inlines ino_name_tree_s
@end

static inline int ino_name_tree_s___compare_value(const ino_name_tree_s *this,
    const ino_name_s *a_first,const ino_name_s *a_second)
{/*{{{*/
  (void)this;

  if (a_first->parent != a_second->parent)
  {
    return a_first->parent < a_second->parent ? -1 : 1;
  }

  const string_s *f_name = &a_first->name;
  const string_s *s_name = &a_second->name;

  if (f_name->size < s_name->size) { return -1; }
  if (f_name->size > s_name->size) { return 1; }
  return memcmp(f_name->data,s_name->data,f_name->size - 1);
}/*}}}*/

// -- file_list_s --
@begin
inlines file_list_s
@end

// -- od_fuser_s --
@begin
inlines od_fuser_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

