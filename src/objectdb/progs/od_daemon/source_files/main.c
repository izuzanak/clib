
@begin
include "main.h"
@end

volatile int g_terminate = 0;

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

  epoll_s_create(&this->epoll,0);

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
    if (od_channel_s_create(&this->channel,channel_cfg->ip.data,channel_cfg->port,&this->epoll,
          od_daemon_s_channel_callback,
          this))
    {
      throw_error(OD_DAEMON_CHANNEL_CREATE_ERROR);
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
    if ((err = epoll_s_wait(&this->epoll,1,-1)))
    {
      if (err != ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED)
      {
        throw_error(OD_DAEMON_EPOLL_WAIT_ERROR);
      }
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

int od_daemon_s_process_updates(od_daemon_s *this,const string_s *a_path)
{/*{{{*/

  // - search for matching node -
  CONT_INIT_CLEAR(var_s,node_var);
  odb_database_s_find_node(&this->database,a_path->data,&node_var);

  // - find info nodes in path -
  this->nodes.used = 0;
  odb_database_s_nodes_path(&this->database,&this->nodes);

  if (node_var != NULL)
  {
    // - find info nodes in subtree -
    odb_database_s_nodes_tree(node_var,&this->nodes);
  }

  if (this->nodes.used != 0)
  {
    var_s *n_ptr = this->nodes.data;
    var_s *n_ptr_end = n_ptr + this->nodes.used;
    do {
      odb_node_s *node = loc_s_odb_node_value(*n_ptr);
      var_map_tree_s *info = loc_s_dict_value(node->info);
      const string_s *path = loc_s_string_value(node->path);

      CONT_INIT_CLEAR(var_s,data_var);
      odb_database_s_get_value(&this->database,path->data,&data_var);

      // - create watch event message -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"type\":\"update\",\"id\":0,\"path\":");
      string_s_to_json(path,&this->buffer);
      bc_array_s_append_ptr(&this->buffer,",\"data\":");
      var_s_to_json(&data_var,&this->buffer);
      bc_array_s_push(&this->buffer,'}');

      // - retrieve channel indexes -
      this->indexes.used = 0;

      if (info->root_idx != c_idx_not_exist)
      {
        var_map_tree_s_node *ptr = info->data;
        var_map_tree_s_node *ptr_end = ptr + info->used;
        do {
          if (ptr->valid)
          {
            ui_array_s_push(&this->indexes,loc_s_int_value(ptr->object.key));
          }
        } while(++ptr < ptr_end);
      }

      // - send watch event message -
      if (od_channel_s_send_multi_message(&this->channel,&this->indexes,&this->buffer))
      {
        throw_error(OD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }

    } while(++n_ptr < n_ptr_end);
  }

  return 0;
}/*}}}*/

int od_daemon_s_channel_callback(void *a_sd_daemon,unsigned a_index,unsigned a_type,va_list a_ap)
{/*{{{*/
  od_daemon_s *this = (od_daemon_s *)a_sd_daemon;

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
        if (od_daemon_s_process_updates(this,path))
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
      if (od_daemon_s_process_updates(this,path))
      {
        throw_error(OD_DAEMON_PROCESS_UPDATES_ERROR);
      }

      // - reset command data -
      odb_database_s_set_value(&this->database,path->data,NULL,&updated);
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

      CONT_INIT_CLEAR(var_s,node_var);
      odb_database_s_add_node(&this->database,path->data,&node_var);

      odb_node_s *node = loc_s_odb_node_value(node_var);

      if (node->info == NULL)
      {
        // - create info dict -
        var_s_copy_loc(&node->info,loc_s_dict());
      }

      VAR_CLEAR(index_var,loc_s_int(a_index));
      loc_s_dict_set(node->info,index_var,NULL);

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

// === global functions ========================================================

void signal_handler(int a_signum)
{/*{{{*/
  (void)a_signum;

  // - terminate on all signals -
  __sync_add_and_fetch(&g_terminate,1);
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

  {
    cassert(signal_s_simple_handler(signal_handler) == 0);

    CONT_INIT_CLEAR(process_s,process);
    cassert(process_s_create(&process,"od_daemon") == 0);

    do {
      CONT_INIT_CLEAR(od_daemon_s,daemon);

      if (od_daemon_s_create(&daemon) ||
          od_config_s_read_file(&daemon.config,OD_JSON_CONFIG_FILE))
      {
        break;
      }

      // - set configuration changed flag -
      daemon.config_changed_flag = 1;

      if (od_daemon_s_run(&daemon)) {}
    } while(0);
  }

  libodb_odl_clear();
  libconf_odl_clear();
  libchannel_odl_clear();
  libchannel_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

