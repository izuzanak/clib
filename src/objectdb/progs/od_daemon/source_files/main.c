
@begin
include "main.h"
@end

const char *g_name;
const char *g_conf;

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- od_daemon_s --
@begin
methods od_daemon_s
@end

int od_daemon_s_create(od_daemon_s *this)
{/*{{{*/
  od_daemon_s_clear(this);

  // - reset configuration changed flag -
  this->config_changed_flag = 0;

  // - create object database -
  odb_database_s_create(&this->database);

  return 0;
}/*}}}*/

int od_daemon_s_process_config(od_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_2("process configuration");

  // - if communication channel configuration changed -
  if (!od_conf_ip_port_s_compare(&this->config.channel,&this->last_config.channel))
  {
    od_conf_ip_port_s *channel_cfg = &this->config.channel;

    // - create communication channel -
    if (od_channel_s_create(&this->channel,channel_cfg->ip.data,channel_cfg->port,
          od_daemon_s_channel_callback,
          this))
    {
      throw_error(OD_DAEMON_CHANNEL_CREATE_ERROR);
    }
  }

  // - if storage configuration changed -
  if (!od_conf_storage_s_compare(&this->config.storage,&this->last_config.storage))
  {
    od_conf_storage_s *storage_cfg = &this->config.storage;

    if (storage_cfg->path.size > 1)
    {
      // - open storage file -
      if (file_s_open(&this->storage,storage_cfg->path.data,"r+b"))
      {
        throw_error(OD_DAEMON_STORAGE_FILE_OPEN_ERROR);
      }

      // - read database from storage -
      if (od_daemon_s_storage_read(this))
      {
        throw_error(OD_DAEMON_STORAGE_READ_ERROR);
      }
    }
    else
    {
      // - storage is not used -
      file_s_clear(&this->storage);
    }
  }

  // - if watchdog channel configuration changed -
  if (!od_conf_ip_port_s_compare(&this->config.watchdog,&this->last_config.watchdog))
  {
    od_conf_ip_port_s *watchdog_cfg = &this->config.watchdog;

    if (watchdog_cfg->ip.size > 1)
    {
      // - create watchdog channel -
      if (wd_channel_client_s_create(&this->watchdog_channel,
            watchdog_cfg->ip.data,watchdog_cfg->port,
            od_daemon_s_watchdog_channel_callback,
            this,0))
      {
        throw_error(OD_DAEMON_WATCHDOG_CHANNEL_CREATE_ERROR);
      }
    }
    else
    {
      wd_channel_client_s_clear(&this->watchdog_channel);
    }
  }

  // - update last configuration -
  od_config_s_copy(&this->last_config,&this->config);

  return 0;
}/*}}}*/

int od_daemon_s_run(od_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_0("running");

  while(!g_terminate)
  {
    if (this->config_changed_flag)
    {
      if (od_daemon_s_process_config(this))
      {
        throw_error(OD_DAEMON_CONFIG_DATA_ERROR);
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
        throw_error(OD_DAEMON_EPOLL_WAIT_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

void od_daemon_s_storage_record(od_daemon_s *this,const string_s *a_path,var_s a_data_var)
{/*{{{*/

  // - prepare record buffer -
  this->buffer.used = 0;
  bc_array_s_push_blanks(&this->buffer,ODB_RECORD_HEADER_SIZE);
  bc_array_s_push(&this->buffer,'[');
  string_s_to_json(a_path,&this->buffer);
  bc_array_s_push(&this->buffer,',');
  var_s_to_json(&a_data_var,&this->buffer);
  bc_array_s_push(&this->buffer,']');

  // - compute record crc -
  crc16_s rec_crc = 0x5555;
  crc16_s_update(&rec_crc,this->buffer.used - ODB_RECORD_HEADER_SIZE,this->buffer.data + ODB_RECORD_HEADER_SIZE);

  // - update record buffer -
  *((unsigned *)this->buffer.data) = this->buffer.used - ODB_RECORD_HEADER_SIZE;
  *((usi *)(this->buffer.data + sizeof(unsigned))) = rec_crc;
}/*}}}*/

int od_daemon_s_storage_read(od_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_2("storage, reading");

  odb_database_s_create(&this->database);

  struct stat st;
  if (fstat(fileno(this->storage),&st))
  {
    throw_error(OD_STORAGE_FILE_READ_ERROR);
  }

  CONT_INIT_CLEAR(bc_array_s,buffer);
  CONT_INIT_CLEAR(json_parser_s,json_parser);

  size_t rest_size = st.st_size;
  do {

    // - read record header -
    if (rest_size < ODB_RECORD_HEADER_SIZE)
    {
      break;
    }

    rest_size -= ODB_RECORD_HEADER_SIZE;

    buffer.used = 0;
    if (stream_s_read_cnt(&this->storage,&buffer,ODB_RECORD_HEADER_SIZE))
    {
      throw_error(OD_STORAGE_FILE_READ_ERROR);
    }

    unsigned rec_size = *((unsigned *)buffer.data);
    crc16_s rec_crc = *((usi *)(buffer.data + sizeof(unsigned)));

    // - read record data -
    if (rest_size < rec_size)
    {
      if (file_s_seek(&this->storage,-ODB_RECORD_HEADER_SIZE,SEEK_CUR))
      {
        throw_error(OD_STORAGE_FILE_READ_ERROR);
      }

      break;
    }

    buffer.used = 0;
    if (stream_s_read_cnt(&this->storage,&buffer,rec_size))
    {
      throw_error(OD_STORAGE_FILE_READ_ERROR);
    }

    rest_size -= rec_size;

    // - check record crc -
    if (!crc16_s_valid(&rec_crc,0x5555,buffer.used,buffer.data))
    {
      // - last record corrupted (no problem) -
      if (rest_size <= 0)
      {
        if (file_s_seek(&this->storage,-(ODB_RECORD_HEADER_SIZE + rec_size),SEEK_CUR))
        {
          throw_error(OD_STORAGE_FILE_READ_ERROR);
        }

        break;
      }

      throw_error(OD_STORAGE_FILE_READ_ERROR);
    }

    // - parse record data -
    CONT_INIT_CLEAR(var_s,record_var);
    if (json_parser_s_parse(&json_parser,&buffer,&record_var))
    {
      throw_error(OD_STORAGE_FILE_READ_ERROR);
    }

    // - set value in database -
    var_array_s *record_arr = loc_s_array_value(record_var);

    int updated;
    odb_database_s_set_value(&this->database,
        loc_s_string_value(record_arr->data[0])->data,record_arr->data[1],&updated);

  } while(1);

  // - log message -
  log_info_2("storage, reading done");

  return 0;
}/*}}}*/

int od_daemon_s_storage_write(od_daemon_s *this,const string_s *a_path,var_s a_data_var)
{/*{{{*/

  // - prepare record buffer -
  od_daemon_s_storage_record(this,a_path,a_data_var);

  long offset;
  if (file_s_tell(&this->storage,&offset))
  {
    throw_error(OD_STORAGE_FILE_WRITE_ERROR);
  }

  // - maximal storage size exceeded -
  if (offset + this->buffer.used > this->config.storage.max_size)
  {
    // - close storage file -
    file_s_clear(&this->storage);

    // - prepare record buffer -
    CONT_INIT_CLEAR(string_s,root_path);
    od_daemon_s_storage_record(this,&root_path,this->database.data_var);

    CONT_INIT_CLEAR(bc_array_s,tmp_path);
    bc_array_s_append_format(&tmp_path,"%s_tmp",this->config.storage.path.data);

    CONT_INIT_CLEAR(file_s,tmp_storage);

    // - create temporary storage file -
    if (file_s_open(&tmp_storage,tmp_path.data,"wb") ||
        stream_s_write(&tmp_storage,this->buffer.data,this->buffer.used) ||
        stream_s_fflush(&tmp_storage)
#ifndef CLIB_OBJECTDB_NOSYNC
        || fsync(fileno(tmp_storage))
#endif
        )
    {
      throw_error(OD_STORAGE_FILE_WRITE_ERROR);
    }

    file_s_clear(&tmp_storage);

    // - rename temporary storage file to storage file -
    if (rename(tmp_path.data,this->config.storage.path.data) ||
        file_s_open(&this->storage,this->config.storage.path.data,"ab"))
    {
      throw_error(OD_STORAGE_FILE_WRITE_ERROR);
    }
  }
  else
  {
    // - write record to file -
    if (stream_s_write(&this->storage,this->buffer.data,this->buffer.used) ||
        stream_s_fflush(&this->storage)
#ifndef CLIB_OBJECTDB_NOSYNC
        || fsync(fileno(this->storage))
#endif
       )
    {
      throw_error(OD_STORAGE_FILE_WRITE_ERROR);
    }
  }

  return 0;
}/*}}}*/

#define OD_DAEMON_REMOVE_CHANNEL_WATCH() \
{/*{{{*/\
\
  /* - remove channel index from info dict - */\
  loc_s_dict_remove_key(node->info,index_var);\
\
  /* - info dict is empty - */\
  if (loc_s_dict_length(node->info) == 0)\
  {\
    /* - replace empty dict by empty value - */\
    var_s_copy_loc(&node->info,NULL);\
\
    /* - nodes dict is empty - */\
    if (loc_s_dict_length(node->nodes) == 0)\
    {\
      /* - remove node - */\
      odb_database_s_remove_node(&this->database);\
    }\
  }\
}/*}}}*/

int od_daemon_s_process_updates(od_daemon_s *this,const string_s *a_path,var_s a_data_var)
{/*{{{*/

  // - search for matching node -
  CONT_INIT_CLEAR(var_s,node_var);
  odb_database_s_find_node(&this->database,a_path->data,&node_var);

  // - find info nodes in path -
  this->nodes.used = 0;
  odb_database_s_nodes_path(&this->database,&this->nodes);

  // - retrieve send modification info nodes -
  if (this->nodes.used != 0)
  {
    var_s *n_ptr = this->nodes.data;
    var_s *n_ptr_end = n_ptr + this->nodes.used;
    do {
      odb_node_s *node = loc_s_odb_node_value(*n_ptr);
      var_map_tree_s *info = loc_s_dict_value(node->info);
      const string_s *path = loc_s_string_value(node->path);

      // - retrieve channel indexes -
      this->indexes.used = 0;

      if (info->root_idx != c_idx_not_exist)
      {
        var_map_tree_s_node *ptr = info->data;
        var_map_tree_s_node *ptr_end = ptr + info->used;
        do {
          if (ptr->valid)
          {
            if (loc_s_int_value(ptr->object.value) & od_watch_SEND_MODIFICATIONS)
            {
              ui_array_s_push(&this->indexes,loc_s_int_value(ptr->object.key));
            }
          }
        } while(++ptr < ptr_end);
      }

      if (this->indexes.used != 0)
      {
        // - create watch modifications event message -
        this->buffer.used = 0;
        bc_array_s_append_format(&this->buffer,"{\"type\":\"update\",\"id\":0,\"path\":");
        string_s_to_json(path,&this->buffer);

        // - do not send empty mod -
        if (a_path->size > path->size)
        {
          bc_array_s_append_ptr(&this->buffer,",\"mod\":");
          string_s_buffer_to_json(a_path->data + path->size - 1 + (path->size > 1),a_path->data + a_path->size - 1,&this->buffer);
        }

        bc_array_s_append_ptr(&this->buffer,",\"data\":");
        var_s_to_json(&a_data_var,&this->buffer);
        bc_array_s_push(&this->buffer,'}');

        // - send watch modifications event message -
        if (od_channel_s_send_multi_message(&this->channel,&this->indexes,&this->buffer))
        {
          throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
        }
      }

    } while(++n_ptr < n_ptr_end);
  }

  // - store tree nodes base -
  unsigned tree_nodes_base = this->nodes.used;

  if (node_var != NULL)
  {
    odb_database_s_nodes_tree(node_var,&this->nodes);
  }

  if (this->nodes.used != 0)
  {
    var_s *n_ptr = this->nodes.data;
    var_s *n_ptr_end = n_ptr + this->nodes.used;
    var_s *n_ptr_tree_base = this->nodes.data + tree_nodes_base;
    do {
      odb_node_s *node = loc_s_odb_node_value(*n_ptr);
      var_map_tree_s *info = loc_s_dict_value(node->info);
      const string_s *path = loc_s_string_value(node->path);

      CONT_INIT_CLEAR(var_s,data_var);
      odb_database_s_get_value(&this->database,path->data,&data_var);

      this->indexes.used = 0;

      // - retrieve channel indexes -
      if (info->root_idx != c_idx_not_exist)
      {
        var_map_tree_s_node *ptr = info->data;
        var_map_tree_s_node *ptr_end = ptr + info->used;
        do {
          if (ptr->valid)
          {
            if (loc_s_int_value(ptr->object.value) & od_watch_SEND_MODIFICATIONS)
            {
              // - node in update tree -
              if (n_ptr >= n_ptr_tree_base)
              {
                ui_array_s_push(&this->indexes,loc_s_int_value(ptr->object.key));
              }
            }
            else
            {
              ui_array_s_push(&this->indexes,loc_s_int_value(ptr->object.key));
            }
          }
        } while(++ptr < ptr_end);
      }

      if (this->indexes.used != 0)
      {
        // - create watch event message -
        this->buffer.used = 0;
        bc_array_s_append_format(&this->buffer,"{\"type\":\"update\",\"id\":0,\"path\":");
        string_s_to_json(path,&this->buffer);
        bc_array_s_append_ptr(&this->buffer,",\"data\":");
        var_s_to_json(&data_var,&this->buffer);
        bc_array_s_push(&this->buffer,'}');

        // - send watch event message -
        if (od_channel_s_send_multi_message(&this->channel,&this->indexes,&this->buffer))
        {
          throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
        }
      }
    } while(++n_ptr < n_ptr_end);
  }

  return 0;
}/*}}}*/

int od_daemon_s_channel_callback(void *a_od_daemon,unsigned a_index,unsigned a_type,va_list a_ap)
{/*{{{*/
  od_daemon_s *this = (od_daemon_s *)a_od_daemon;

  switch (a_type)
  {
  case od_channel_cbreq_NEW:
    {/*{{{*/
      while (a_index >= this->channel_watches.used)
      {
        var_array_s_push_loc(&this->channel_watches,loc_s_dict());
      }
    }/*}}}*/
    break;
  case od_channel_cbreq_DROP:
    {/*{{{*/
      var_map_tree_s *vm_tree = loc_s_dict_value(*var_array_s_at(&this->channel_watches,a_index));
      if (vm_tree->root_idx != c_idx_not_exist)
      {
        VAR_CLEAR(index_var,loc_s_int(a_index));

        var_map_tree_s_node *ptr = vm_tree->data;
        var_map_tree_s_node *ptr_end = ptr + vm_tree->used;
        do {
          if (ptr->valid)
          {
            // - find node -
            CONT_INIT_CLEAR(var_s,node_var);
            odb_database_s_find_node(&this->database,
                loc_s_string_value(ptr->object.key)->data,&node_var);

            debug_assert(node_var != NULL);

            odb_node_s *node = loc_s_odb_node_value(node_var);
            debug_assert(node->info->v_type == c_bi_type_dict);

            OD_DAEMON_REMOVE_CHANNEL_WATCH();
          }
        } while(++ptr < ptr_end);

        // - clear dictionary -
        var_map_tree_s_clear(vm_tree);
      }
    }/*}}}*/
    break;
  case od_channel_cbreq_SET:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *path = va_arg(a_ap,const string_s *);
      var_s data_var = va_arg(a_ap,var_s);

      int updated;
      odb_database_s_set_value(&this->database,path->data,data_var,&updated);

      if (updated && this->storage != NULL)
      {
        // - write record to storage -
        if (od_daemon_s_storage_write(this,path,data_var))
        {
          throw_error(OD_DAEMON_STORAGE_WRITE_ERROR);
        }
      }

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"set\",\"id\":%" HOST_LL_FORMAT "d,\"path\":",id);
      string_s_to_json(path,&this->buffer);
      bc_array_s_push(&this->buffer,'}');

      if (od_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }

      if (updated)
      {
        // - process watch updates -
        if (od_daemon_s_process_updates(this,path,data_var))
        {
          throw_error(OD_DAEMON_PROCESS_UPDATES_ERROR);
        }
      }
    }/*}}}*/
    break;
  case od_channel_cbreq_CMD:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *path = va_arg(a_ap,const string_s *);
      var_s data_var = va_arg(a_ap,var_s);

      int updated;
      odb_database_s_set_value(&this->database,path->data,data_var,&updated);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"cmd\",\"id\":%" HOST_LL_FORMAT "d,\"path\":",id);
      string_s_to_json(path,&this->buffer);
      bc_array_s_push(&this->buffer,'}');

      if (od_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }

      // - process watch updates -
      if (od_daemon_s_process_updates(this,path,data_var))
      {
        throw_error(OD_DAEMON_PROCESS_UPDATES_ERROR);
      }

      // - reset command data -
      odb_database_s_set_value(&this->database,path->data,NULL,&updated);
    }/*}}}*/
    break;
  case od_channel_cbreq_LIST:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *path = va_arg(a_ap,const string_s *);

      CONT_INIT_CLEAR(var_s,data_var);
      odb_database_s_get_value(&this->database,path->data,&data_var);

      // - variable is dictionary -
      if (data_var != NULL && data_var->v_type == c_bi_type_dict)
      {
        VAR_CLEAR(keys_var,loc_s_array());
        var_array_s *keys_array = (var_array_s *)keys_var->v_data.ptr;

        // - retrieve dictionary keys -
        var_map_tree_s *tree = (var_map_tree_s *)data_var->v_data.ptr;
        if (tree->count != 0)
        {
          var_array_s_reserve(keys_array,tree->count);

          var_map_tree_s_node *vmtn_ptr = tree->data;
          var_map_tree_s_node *vmtn_ptr_end = vmtn_ptr + tree->used;
          do {
            if (vmtn_ptr->valid)
            {
              var_array_s_push_blank(keys_array);
              var_s_copy(var_array_s_last(keys_array),&vmtn_ptr->object.key);
            }
          } while(++vmtn_ptr < vmtn_ptr_end);
        }

        var_s_copy_loc(&data_var,keys_var);
      }

      // - variable is not dictionary -
      else
      {
        var_s_copy_loc(&data_var,NULL);
      }

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"list\",\"id\":%" HOST_LL_FORMAT "d,\"path\":",id);
      string_s_to_json(path,&this->buffer);
      bc_array_s_append_ptr(&this->buffer,",\"data\":");
      var_s_to_json(&data_var,&this->buffer);
      bc_array_s_push(&this->buffer,'}');

      if (od_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case od_channel_cbreq_GET:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *path = va_arg(a_ap,const string_s *);

      CONT_INIT_CLEAR(var_s,data_var);
      odb_database_s_get_value(&this->database,path->data,&data_var);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"get\",\"id\":%" HOST_LL_FORMAT "d,\"path\":",id);
      string_s_to_json(path,&this->buffer);
      bc_array_s_append_ptr(&this->buffer,",\"data\":");
      var_s_to_json(&data_var,&this->buffer);
      bc_array_s_push(&this->buffer,'}');

      if (od_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case od_channel_cbreq_WATCH:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *path = va_arg(a_ap,const string_s *);
      int options = va_arg(a_ap,int);

      CONT_INIT_CLEAR(var_s,node_var);
      odb_database_s_add_node(&this->database,path->data,&node_var);

      odb_node_s *node = loc_s_odb_node_value(node_var);

      if (node->info == NULL)
      {
        // - create info dict -
        var_s_copy_loc(&node->info,loc_s_dict());
      }

      VAR_CLEAR(index_var,loc_s_int(a_index));
      loc_s_dict_set(node->info,index_var,loc_s_int(options));

      // - update channel watches -
      loc_s_dict_set(*var_array_s_at(&this->channel_watches,a_index),
          loc_s_odb_node_value(node_var)->path,NULL);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"watch\",\"id\":%" HOST_LL_FORMAT "d,\"path\":",id);
      string_s_to_json(path,&this->buffer);
      bc_array_s_push(&this->buffer,'}');

      if (od_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }

      CONT_INIT_CLEAR(var_s,data_var);
      odb_database_s_get_value(&this->database,path->data,&data_var);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"type\":\"update\",\"id\":0,\"path\":");
      string_s_to_json(path,&this->buffer);
      bc_array_s_append_ptr(&this->buffer,",\"data\":");
      var_s_to_json(&data_var,&this->buffer);
      bc_array_s_push(&this->buffer,'}');

      if (od_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case od_channel_cbreq_IGNORE:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *path = va_arg(a_ap,const string_s *);

      CONT_INIT_CLEAR(var_s,node_var);
      odb_database_s_find_node(&this->database,path->data,&node_var);

      if (node_var != NULL)
      {
        odb_node_s *node = loc_s_odb_node_value(node_var);
        debug_assert(node->info->v_type == c_bi_type_dict);

        VAR_CLEAR(index_var,loc_s_int(a_index));
        if (loc_s_dict_has_key(node->info,index_var))
        {
          OD_DAEMON_REMOVE_CHANNEL_WATCH();
        }

        // - update channel watches -
        loc_s_dict_remove_key(*var_array_s_at(&this->channel_watches,a_index),node->path);
      }

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"ignore\",\"id\":%" HOST_LL_FORMAT "d,\"path\":",id);
      string_s_to_json(path,&this->buffer);
      bc_array_s_push(&this->buffer,'}');

      if (od_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  default:
    {/*{{{*/

      // - log message -
      log_info_2("channel server %u, unknown request",a_index);
    }/*}}}*/
  }

  return 0;
}/*}}}*/

int od_daemon_s_watchdog_channel_callback(void *a_od_daemon,unsigned a_index,unsigned a_type,va_list a_ap)
{/*{{{*/
  od_daemon_s *this = (od_daemon_s *)a_od_daemon;

  switch (a_type)
  {
  case wd_channel_cbreq_NEW:
    {/*{{{*/

      // - enable watchdog monitor -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,
          "{\"type\":\"enable\",\"id\":%" HOST_LL_FORMAT "d,\"name\":\"%s\",\"timeout\":60}",
          ++this->watchdog_channel.message_id,g_name);

      if (wd_channel_client_s_send_message(&this->watchdog_channel,&this->buffer))
      {
        throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }

      // - register watchdog update -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,
          "{\"type\":\"watch\",\"id\":%" HOST_LL_FORMAT "d}",
          ++this->watchdog_channel.message_id);

      if (wd_channel_client_s_send_message(&this->watchdog_channel,&this->buffer))
      {
        throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }

      // - schedule watchdog timer -
      struct itimerspec its_watchdog = {{30,0},{30,0}};
      if (epoll_s_timer_callback(&this->watchdog_timer,&its_watchdog,0,
            od_daemon_s_watchdog_time_event,this,0))
      {
        throw_error(OD_DAEMON_EPOLL_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_cbreq_DROP:
    {/*{{{*/

      // - disable watchdog timer -
      struct itimerspec its_watchdog = {{0,0},{0,0}};
      if (epoll_timer_s_settime(&this->watchdog_timer,&its_watchdog,0))
      {
        throw_error(OD_DAEMON_TIMER_SETTIME_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_cbresp_ENABLE:
  case wd_channel_cbresp_DISABLE:
  case wd_channel_cbresp_WATCH:
  case wd_channel_cbresp_IGNORE:
  case wd_channel_cbresp_STATUS:
    break;
  case wd_channel_cbevt_UPDATE:
    {/*{{{*/
      (void)va_arg(a_ap,lli);
      const string_s *reason = va_arg(a_ap,const string_s *);

      if (strcmp(reason->data,"timeout") == 0)
      {
        // - terminate daemon -
        __sync_add_and_fetch(&g_terminate,1);
      }
    }/*}}}*/
    break;
  default:
    {/*{{{*/

      // - log message -
      log_info_2("channel watchdog %u, unknown request",a_index);
    }/*}}}*/
  }

  return 0;
}/*}}}*/

int od_daemon_s_watchdog_time_event(void *a_od_daemon,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(OD_DAEMON_TIMER_READ_ERROR);
  }

  od_daemon_s *this = (od_daemon_s *)a_od_daemon;

  this->buffer.used = 0;
  bc_array_s_append_format(&this->buffer,
      "{\"type\":\"keepalive\",\"id\":%" HOST_LL_FORMAT "d,\"name\":\"%s\"}",
      ++this->watchdog_channel.message_id,g_name);

  if (wd_channel_client_s_send_message(&this->watchdog_channel,&this->buffer))
  {
    throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

// === global functions ========================================================

void signal_handler(int a_signum)
{/*{{{*/
  (void)a_signum;

  // - terminate on all signals -
  __sync_add_and_fetch(&g_terminate,1);
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
  libchannel_wdl_init();
  libchannel_odl_init();
  libconf_odl_init();
  libodb_odl_init();

  g_name = "od_daemon";
  g_conf = "od_config.json";

  // - process command line arguments -
  if (argc > 1)
  {
    int arg_idx = 1;
    do {
      if (strncmp("--name=",argv[arg_idx],7) == 0)
      {
        g_name = argv[arg_idx] + 7;
      }
      else if (strncmp("--conf=",argv[arg_idx],7) == 0)
      {
        g_conf = argv[arg_idx] + 7;
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
    cassert(process_s_create(&process,g_name) == 0);

    do {
      CONT_INIT_CLEAR(od_daemon_s,daemon);

      if (od_daemon_s_create(&daemon) ||
          od_config_s_read_file(&daemon.config,g_conf))
      {
        break;
      }

      // - set configuration changed flag -
      daemon.config_changed_flag = 1;

      if (od_daemon_s_run(&daemon))
      {
        // - ignore return value, always terminate -
      }

    } while(0);
  }

  libodb_odl_clear();
  libconf_odl_clear();
  libchannel_odl_clear();
  libchannel_wdl_clear();
  libchannel_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

