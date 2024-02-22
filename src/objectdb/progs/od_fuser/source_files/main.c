
@begin
include "main.h"
@end

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- ino_name_s --
@begin
methods ino_name_s
@end

// -- ino_name_tree_s --
@begin
methods ino_name_tree_s
@end

// -- file_list_s --
@begin
methods file_list_s
@end

// -- od_fuser_s --
@begin
methods od_fuser_s
@end

int od_fuser_s_create(od_fuser_s *this)
{/*{{{*/
  od_fuser_s_clear(this);

  // - reset configuration changed flag -
  this->config_changed_flag = 0;

  // - create object database -
  odb_database_s_create(&this->database);

  // - initialize root ino_name -
  ino_name_s ino_name = {0,{1,""}};
  ino_name_tree_s_insert(&this->ino_name_tree,&ino_name);

  return 0;
}/*}}}*/

int od_fuser_s_process_config(od_fuser_s *this)
{/*{{{*/

  // - log message -
  log_info_2("process configuration");

  // - if communication channel configuration changed -
  if (!od_conf_ip_port_s_compare(&this->config.channel,&this->last_config.channel))
  {
    od_conf_ip_port_s *channel_cfg = &this->config.channel;

    // - create communication channel -
    if (od_channel_client_s_create(&this->channel,
          channel_cfg->ip.data,
          channel_cfg->port,
          od_fuser_s_channel_callback,
          this,0))
    {
      throw_error(OD_FUSER_CHANNEL_CREATE_ERROR);
    }
  }

  // - if fuse configuration changed -
  if (!string_s_compare(
        &this->config.fuser.mountpoint,
        &this->last_config.fuser.mountpoint))
  {
    // - create mountpoint directory -
    this->buffer.used = 0;
    bc_array_s_append_format(&this->buffer,"mkdir -p '%s'",this->config.fuser.mountpoint.data);
    if (system(this->buffer.data)) // NOLINT
    {
      throw_error(OD_FUSER_MOUNTPOINT_CREATE_ERROR);
    }

    // - create fuse session -
    struct fuse_lowlevel_ops od_fuser_s_fuse_oper =
    {/*{{{*/
      .lookup     = od_fuser_s_lookup,
      .getattr    = od_fuser_s_getattr,
      .opendir    = od_fuser_s_opendir,
      .readdir    = od_fuser_s_readdir,
      .open       = od_fuser_s_open,
      .release    = od_fuser_s_release,
      .read       = od_fuser_s_read,
      .write      = od_fuser_s_write,
    };/*}}}*/

    CONT_INIT_CLEAR(string_array_s,fuse_args);
    string_array_s_push_ptr(&fuse_args,"od_fuser");
    string_array_s_push_ptr(&fuse_args,"-o");
    string_array_s_push_ptr(&fuse_args,"allow_other");

    if (fuse_session_s_create(&this->fuse_session,&fuse_args,&od_fuser_s_fuse_oper,this) ||
        fuse_session_s_mount(&this->fuse_session,this->config.fuser.mountpoint.data) ||
        (this->fuse_epoll_fd.fd = dup(fuse_session_s_fd(&this->fuse_session))) == -1 ||
        epoll_s_fd_callback(&this->fuse_epoll_fd,EPOLLIN | EPOLLPRI,od_fuser_s_fuse_fd_event,this,0))
    {
      throw_error(OD_FUSER_FUSE_SESSION_CREATE_ERROR);
    }
  }

  // - update last configuration -
  od_config_s_copy(&this->last_config,&this->config);

  return 0;
}/*}}}*/

int od_fuser_s_run(od_fuser_s *this)
{/*{{{*/

  // - log message -
  log_info_0("running");

  while(!g_terminate)
  {
    if (this->config_changed_flag)
    {
      if (od_fuser_s_process_config(this))
      {
        throw_error(OD_FUSER_CONFIG_DATA_ERROR);
      }

      if (g_terminate)
      {
        break;
      }

      // - reset configuration changed flag -
      this->config_changed_flag = 0;
    }

    // - wait on events -
    int err;
    if ((err = epoll_s_wait(g_epoll,-1)))
    {
      if (err != ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED)
      {
        throw_error(OD_FUSER_EPOLL_WAIT_ERROR);
      }
    }

    // - check fuse session state -
    if (this->fuse_session.se && fuse_session_exited(this->fuse_session.se))
    {
      g_terminate = 1;
    }
  }

  return 0;
}/*}}}*/

void od_fuser_s_buffer_ino_path(od_fuser_s *this,unsigned a_ino)
{/*{{{*/
  debug_assert(a_ino != 0);

  this->ino_stack.used = 0;
  this->path_buffer.used = 0;

  if (a_ino > 1)
  {
    // - construct ino stack -
    do {
      ino_name_s *ino_name = &this->ino_name_tree.data[a_ino].object;
      ui_array_s_push(&this->ino_stack,a_ino);
      a_ino = ino_name->parent;
    } while(a_ino > 1);

    // - construct database path -
    do {
      string_s *name = &this->ino_name_tree.data[ui_array_s_pop(&this->ino_stack)].object.name;
      bc_array_s_append(&this->path_buffer,name->size - 1,name->data);

      if (this->ino_stack.used == 0)
      {
        break;
      }

      bc_array_s_push(&this->path_buffer,'/');
    } while(1);
  }

  bc_array_s_push(&this->path_buffer,'\0');
}/*}}}*/

void od_fuser_s_set_ino_value(od_fuser_s *this,unsigned a_ino,var_s a_value_var)
{/*{{{*/
  od_fuser_s_buffer_ino_path(this,a_ino);

  int updated;
  odb_database_s_set_value(&this->database,this->path_buffer.data,a_value_var,&updated);
}/*}}}*/

void od_fuser_s_get_ino_value(od_fuser_s *this,unsigned a_ino,var_s *a_value_var)
{/*{{{*/
  od_fuser_s_buffer_ino_path(this,a_ino);
  odb_database_s_get_value(&this->database,this->path_buffer.data,a_value_var);
}/*}}}*/

void od_fuser_s_stat(od_fuser_s *this,unsigned ino,struct stat *stbuf)
{/*{{{*/
  CONT_INIT_CLEAR(var_s,data_var);
  od_fuser_s_get_ino_value(this,ino,&data_var);

  // - variable is dictionary -
  if (data_var != NULL && data_var->v_type == c_bi_type_dict)
  {
    stbuf->st_ino = ino;
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2 + loc_s_dict_value(data_var)->count;
    stbuf->st_uid = getuid();
    stbuf->st_gid = getgid();
    stbuf->st_atime = 0;
    stbuf->st_mtime = stbuf->st_atime;
    stbuf->st_ctime = stbuf->st_atime;
  }
  else
  {
    stbuf->st_size = 0;
    stbuf->st_ino = ino;
    stbuf->st_mode = S_IFREG | 0666;
    stbuf->st_nlink = 1;
    stbuf->st_uid = getuid();
    stbuf->st_gid = getgid();
    stbuf->st_atime = 0;
    stbuf->st_mtime = stbuf->st_atime;
    stbuf->st_ctime = stbuf->st_atime;
  }
}/*}}}*/

int od_fuser_s_channel_callback(void *a_od_fuser,unsigned a_index,unsigned a_type,va_list a_ap)
{/*{{{*/
  od_fuser_s *this = (od_fuser_s *)a_od_fuser;

  switch (a_type)
  {
  case od_channel_cbreq_NEW:
    {/*{{{*/
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,
          "{\"type\":\"watch\",\"id\":%" HOST_LL_FORMAT "d,\"path\":\"\",\"options\":1}",
          ++this->channel.message_id);

      if (od_channel_client_s_send_message(&this->channel,&this->buffer))
      {
        throw_error(OD_FUSER_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case od_channel_cbreq_DROP:
    break;
  case od_channel_cbresp_SET:
    break;
  case od_channel_cbresp_WATCH:
    break;
  case od_channel_cbevt_UPDATE:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *path = va_arg(a_ap,const string_s *);
      const string_s *mod = va_arg(a_ap,const string_s *);
      var_s data_var = va_arg(a_ap,var_s);

      // - ignore path, watching whole database -
      (void)id;
      (void)path;

      int updated;
      odb_database_s_set_value(
          &this->database,mod == NULL ? NULL : mod->data,data_var,&updated);
    }/*}}}*/
    break;
  default:
    {/*{{{*/

      // - log message -
      log_info_2("channel client %u, unknown request",a_index);
    }/*}}}*/
  }

  return 0;
}/*}}}*/

int od_fuser_s_fuse_fd_event(void *a_od_fuser,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;
  (void)a_epoll_event;

  od_fuser_s *this = (od_fuser_s *)a_od_fuser;
  return fuse_session_s_process(&this->fuse_session);
}/*}}}*/

void od_fuser_s_lookup(fuse_req_t req,fuse_ino_t parent,const char *name)
{/*{{{*/
  debug_message_6(fprintf(stderr,"od_fuser_s_lookup: parent: %" HOST_LL_FORMAT "u, name: %s\n",(ulli)parent,name));

  od_fuser_s *this = (od_fuser_s *)fuse_req_userdata(req);

  struct fuse_entry_param entry = {};
  entry.attr_timeout = 1.0;
  entry.entry_timeout = 1.0;

  // - retrieve or create ino -
  ino_name_s ino_name = {parent,{strlen(name) + 1,(char *)name}};
  entry.ino = ino_name_tree_s_unique_insert(&this->ino_name_tree,&ino_name);

  od_fuser_s_stat(this,entry.ino,&entry.attr);

  fuse_reply_entry(req,&entry);
}/*}}}*/

void od_fuser_s_getattr(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi)
{/*{{{*/
  (void)fi;

  debug_message_6(fprintf(stderr,"od_fuser_s_getattr, ino: %" HOST_LL_FORMAT "u\n",(ulli)ino));

  od_fuser_s *this = (od_fuser_s *)fuse_req_userdata(req);

  struct stat stbuf = {};
  od_fuser_s_stat(this,ino,&stbuf);

  fuse_reply_attr(req,&stbuf,1.0);
}/*}}}*/

void od_fuser_s_opendir(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi)
{/*{{{*/
  debug_message_6(fprintf(stderr,"od_fuser_s_opendir, ino: %" HOST_LL_FORMAT "u\n",(ulli)ino));

  fuse_reply_open(req,fi);
}/*}}}*/

void od_fuser_s_readdir(fuse_req_t req,fuse_ino_t ino,size_t size,off_t off,struct fuse_file_info *fi)
{/*{{{*/
  (void)fi;

  debug_message_6(fprintf(stderr,"od_fuser_s_readdir, ino: %" HOST_LL_FORMAT "u\n",(ulli)ino));

  od_fuser_s *this = (od_fuser_s *)fuse_req_userdata(req);

  CONT_INIT_CLEAR(var_s,data_var);
  od_fuser_s_get_ino_value(this,ino,&data_var);

  if (data_var == NULL || data_var->v_type != c_bi_type_dict)
  {
    throw_fuse_error(req,ENOTDIR);
  }

  // - prepare directory buffer -
  bc_array_s *dirbuff = &this->fuse_dirbuff;
  dirbuff->used = 0;

  fuse_dirbuff_s_add(dirbuff,req,".",ino);
  fuse_dirbuff_s_add(dirbuff,req,"..",ino);

  // - retrieve dictionary keys -
  var_map_tree_s *tree = (var_map_tree_s *)data_var->v_data.ptr;
  if (tree->count != 0)
  {
    var_map_tree_s_node *vmtn_ptr = tree->data;
    var_map_tree_s_node *vmtn_ptr_end = vmtn_ptr + tree->used;
    do {
      if (vmtn_ptr->valid)
      {
        fuse_dirbuff_s_add(dirbuff,req,loc_s_string_value(vmtn_ptr->object.key)->data,ino);
      }
    } while(++vmtn_ptr < vmtn_ptr_end);
  }

  fuse_dirbuff_s_reply(dirbuff,req,size,off);
}/*}}}*/

void od_fuser_s_open(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi)
{/*{{{*/
  debug_message_6(fprintf(stderr,"od_fuser_s_open, ino: %" HOST_LL_FORMAT "u\n",(ulli)ino));

  od_fuser_s *this = (od_fuser_s *)fuse_req_userdata(req);

  // - retrieve open ino name -
  fuse_ino_t open_ino = ino;

  ino_name_s *ino_name = &this->ino_name_tree.data[ino].object;
  string_s *name = &ino_name->name;

  // - check for .json extension -
  if (name->size >= 6 && strcmp(name->data + name->size - 6,".json") == 0)
  {
    // - retrieve or create ino -
    ino_name_s open_ino_name = {ino_name->parent,{name->size - 5,name->data}};
    open_ino = ino_name_tree_s_unique_insert(&this->ino_name_tree,&open_ino_name);
  }

  unsigned file_idx;

  switch (fi->flags & O_ACCMODE)
  {
    case O_RDONLY:
      {/*{{{*/
        CONT_INIT_CLEAR(var_s,data_var);
        od_fuser_s_get_ino_value(this,open_ino,&data_var);

        file_idx = file_list_s_append_blank(&this->open_files);
        bc_array_s *file_buffer = file_list_s_at(&this->open_files,file_idx);

        CONT_INIT_CLEAR(json_nice_s,json_nice);
        json_nice_s_create(&json_nice,"  ","",NULL);

        file_buffer->used = 0;
        var_s_to_json_nice(&data_var,&json_nice,file_buffer);
        bc_array_s_push(file_buffer,'\n');
      }/*}}}*/
      break;
    case O_WRONLY:
      {/*{{{*/
        
        // - write only .json files -
        if (open_ino == ino)
        {
          throw_fuse_error(req,EACCES);
        }

        file_idx = file_list_s_append_blank(&this->open_files);
        bc_array_s *file_buffer = file_list_s_at(&this->open_files,file_idx);
        file_buffer->used = 0;
      }/*}}}*/
      break;
    default:
      throw_fuse_error(req,EACCES);
  }

  // - update file info structure -
  fi->direct_io = 1;
  fi->nonseekable = 1;

  // - store open file index to file info -
  fi->fh = file_idx;

  fuse_reply_open(req,fi);
}/*}}}*/

void od_fuser_s_release(fuse_req_t req,fuse_ino_t ino,struct fuse_file_info *fi)
{/*{{{*/
  debug_message_6(fprintf(stderr,"od_fuser_s_release, ino: %" HOST_LL_FORMAT "u\n",(ulli)ino));

  od_fuser_s *this = (od_fuser_s *)fuse_req_userdata(req);

  unsigned file_idx = fi->fh;
  if (file_idx >= this->open_files.used)
  {
    throw_fuse_error(req,ENOENT);
  }

  if ((fi->flags & O_ACCMODE) == O_WRONLY)
  {
    bc_array_s *file_buffer = file_list_s_at(&this->open_files,file_idx);

    CONT_INIT_CLEAR(var_s,value_var);
    CONT_INIT_CLEAR(json_parser_s,json_parser);
    if (json_parser_s_parse(&json_parser,file_buffer,&value_var))
    {
      file_list_s_remove(&this->open_files,file_idx);

      throw_fuse_error(req,EIO);
    }

    ino_name_s *ino_name = &this->ino_name_tree.data[ino].object;
    string_s *name = &ino_name->name;

    // - retrieve or create ino -
    ino_name_s open_ino_name = {ino_name->parent,{name->size - 5,name->data}};
    fuse_ino_t open_ino = ino_name_tree_s_unique_insert(&this->ino_name_tree,&open_ino_name);

    od_fuser_s_set_ino_value(this,open_ino,value_var);

    // - update database -
    this->buffer.used = 0;
    bc_array_s_append_format(&this->buffer,
        "{\"type\":\"set\",\"id\":%" HOST_LL_FORMAT "d,\"path\":\"%s\",\"data\":%.*s}",
        ++this->channel.message_id,this->path_buffer.data,file_buffer->used,file_buffer->data);

    if (od_channel_client_s_send_message(&this->channel,&this->buffer))
    {
      file_list_s_remove(&this->open_files,file_idx);

      throw_fuse_error(req,EIO);
    }
  }

  file_list_s_remove(&this->open_files,file_idx);

  fuse_reply_err(req,0);
}/*}}}*/

void od_fuser_s_read(fuse_req_t req,fuse_ino_t ino,size_t size,off_t off,struct fuse_file_info *fi)
{/*{{{*/
  (void)ino;

  debug_message_7(fprintf(stderr,"od_fuser_s_read, ino: %" HOST_LL_FORMAT "u\n",(ulli)ino));

  od_fuser_s *this = (od_fuser_s *)fuse_req_userdata(req);

  unsigned file_idx = fi->fh;
  if (file_idx >= this->open_files.used)
  {
    throw_fuse_error(req,ENOENT);
  }

  bc_array_s *file_buffer = file_list_s_at(&this->open_files,file_idx);

  unsigned rest = file_buffer->used - off;
  fuse_reply_buf(req,file_buffer->data + off,rest > size ? size : rest);
}/*}}}*/

void od_fuser_s_write(fuse_req_t req,fuse_ino_t ino,const char *buf,size_t size,off_t off,struct fuse_file_info *fi)
{/*{{{*/
  (void)ino;

  debug_message_7(fprintf(stderr,"od_fuser_s_write, ino: %" HOST_LL_FORMAT "u\n",(ulli)ino));

  od_fuser_s *this = (od_fuser_s *)fuse_req_userdata(req);

  unsigned file_idx = fi->fh;
  if (file_idx >= this->open_files.used)
  {
    throw_fuse_error(req,ENOENT);
  }

  bc_array_s *file_buffer = file_list_s_at(&this->open_files,file_idx);

  if (file_buffer->used != off)
  {
    throw_fuse_error(req,EIO);
  }

  // - append data to file buffer -
  bc_array_s_append(file_buffer,size,buf);

  fuse_reply_write(req,size);
}/*}}}*/

// === global functions ========================================================

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
  libchannel_cll_init();
  libchannel_odl_init();
  libconf_odl_init();
  libodb_odl_init();

  char *name = "od_fuser";
  char *conf = "od_config.json";

  // - process command line arguments -
  if (argc > 1)
  {
    int arg_idx = 1;
    do {
      if (strncmp("--name=",argv[arg_idx],7) == 0)
      {
        name = argv[arg_idx] + 7;
      }
      else if (strncmp("--conf=",argv[arg_idx],7) == 0)
      {
        conf = argv[arg_idx] + 7;
      }
      else
      {
        cassert(0);
      }
    } while(++arg_idx < argc);
  }

  // - create process directories -
  cassert(system("mkdir -p -m 777 " PROCESS_RUN_DIR_PATH " " PROCESS_LOG_DIR_PATH) == 0); // NOLINT

  // - create process -
  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    CONT_INIT_CLEAR(epoll_s,epoll);
    epoll_s_create(&epoll,EPOLL_CLOEXEC);

    g_epoll_fd_update = epoll_fd_update;
    g_epoll = &epoll;

    CONT_INIT_CLEAR(process_s,process);
    cassert(process_s_create(&process,name) == 0);

    do {
      CONT_INIT_CLEAR(od_fuser_s,fuser);

      if (od_fuser_s_create(&fuser) ||
          od_config_s_read_file(&fuser.config,conf))
      {
        break;
      }

      // - set configuration changed flag -
      fuser.config_changed_flag = 1;

      if (od_fuser_s_run(&fuser))
      {
        // - ignore return value, always terminate -
      }

    } while(0);
  }

  libodb_odl_clear();
  libconf_odl_clear();
  libchannel_odl_clear();
  libchannel_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

