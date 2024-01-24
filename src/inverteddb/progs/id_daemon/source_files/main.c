
@begin
include "main.h"
@end

const char *g_name;
const char *g_conf;

volatile int g_terminate = 0;
epoll_s *g_epoll;

// === methods of generated structures =========================================

// -- id_daemon_s --
@begin
methods id_daemon_s
@end

int id_daemon_s_create(id_daemon_s *this)
{/*{{{*/
  id_daemon_s_clear(this);

  // - reset configuration changed flag -
  this->config_changed_flag = 0;

  return 0;
}/*}}}*/

int id_daemon_s_process_config(id_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_2("process configuration");

  // - if communication channel configuration changed -
  if (!id_conf_ip_port_s_compare(&this->config.channel,&this->last_config.channel))
  {
    id_conf_ip_port_s *channel_cfg = &this->config.channel;

    // - create communication channel -
    if (id_channel_s_create(&this->channel,channel_cfg->ip.data,channel_cfg->port,
          id_daemon_s_channel_callback,
          this))
    {
      throw_error(ID_DAEMON_CHANNEL_CREATE_ERROR);
    }
  }

  // - if watchdog channel configuration changed -
  if (!id_conf_ip_port_s_compare(&this->config.watchdog,&this->last_config.watchdog))
  {
    id_conf_ip_port_s *watchdog_cfg = &this->config.watchdog;

    if (watchdog_cfg->ip.size > 1)
    {
      // - create watchdog channel -
      if (wd_channel_client_s_create(&this->watchdog_channel,
            watchdog_cfg->ip.data,watchdog_cfg->port,
            id_daemon_s_watchdog_channel_callback,
            this,0))
      {
        throw_error(ID_DAEMON_WATCHDOG_CHANNEL_CREATE_ERROR);
      }
    }
    else
    {
      wd_channel_client_s_clear(&this->watchdog_channel);
    }
  }

  // - if databases configuration changed -
  if (!id_conf_database_tree_s_compare(&this->config.databases,&this->last_config.databases))
  {
    // - update databases -
    if (id_daemon_s_update_databases(this))
    {
      throw_error(ID_DAEMON_CONFIG_DATA_ERROR);
    }
  }

  // - update last configuration -
  id_config_s_copy(&this->last_config,&this->config);

  return 0;
}/*}}}*/

int id_daemon_s_update_databases(id_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_2("id_daemon_s_update_databases");

  // - remove databases not present in configuration -
  if (this->databases.root_idx != c_idx_not_exist)
  {
    idb_database_tree_s_node *idtn_ptr = this->databases.data;
    idb_database_tree_s_node *idtn_ptr_end = idtn_ptr + this->databases.used;
    do
    {
      if (idtn_ptr->valid)
      {
        id_conf_database_s search_database_conf = {idtn_ptr->object.name,};
        unsigned database_conf_idx = id_conf_database_tree_s_get_idx(&this->config.databases,&search_database_conf);

        // - database not present in configuration -
        if (database_conf_idx == c_idx_not_exist)
        {
          // - log message -
          log_info_2("remove database %s",idtn_ptr->object.name.data);

          idb_database_s *database = &idtn_ptr->object;
          idb_database_tree_s_remove(&this->databases,idtn_ptr - this->databases.data);
          idb_database_s_clear(database);
        }
      }
    } while(++idtn_ptr < idtn_ptr_end);
  }

  // - add or update databases according to configuration -
  if (this->config.databases.root_idx != c_idx_not_exist)
  {
    id_conf_database_tree_s_node *cdtn_ptr = this->config.databases.data;
    id_conf_database_tree_s_node *cdtn_ptr_end = cdtn_ptr + this->config.databases.used;
    do {
      if (cdtn_ptr->valid)
      {
        idb_database_s search_database = {cdtn_ptr->object.name,};
        unsigned database_idx = idb_database_tree_s_get_idx(&this->databases,&search_database);

        if (database_idx == c_idx_not_exist)
        {
          id_conf_database_s *database_conf = &cdtn_ptr->object;

          // - log message -
          log_info_2("add database %s",database_conf->name.data);

          // - add database from configuration -
          CONT_INIT_CLEAR(idb_database_s,database);
          if (idb_database_s_create(&database,
                database_conf->name.data,
                database_conf->storage.data,
                database_conf->max_indexes,
                database_conf->merge_cnt))
          {
            throw_error(ID_DAEMON_DATABASE_CREATE_ERROR);
          }

          idb_database_tree_s_swap_insert(&this->databases,&database);
        }
        else
        {
          // - not supported runtime configuration change -
          cassert(0);
        }
      }
    } while(++cdtn_ptr < cdtn_ptr_end);
  }

  return 0;
}/*}}}*/

int id_daemon_s_run(id_daemon_s *this)
{/*{{{*/

  // - log message -
  log_info_0("running");

  while(!g_terminate)
  {
    if (this->config_changed_flag)
    {
      if (id_daemon_s_process_config(this))
      {
        throw_error(ID_DAEMON_CONFIG_DATA_ERROR);
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
        throw_error(ID_DAEMON_EPOLL_WAIT_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

void id_daemon_bits_result_to_range_offsets(idb_bits_tree_s *a_bits_res,ui_array_s *a_ranges)
{/*{{{*/
  if (a_bits_res->root_idx != c_idx_not_exist)
  {
    unsigned stack[RB_TREE_STACK_SIZE(idb_bits_tree_s,a_bits_res)];
    unsigned *stack_ptr = stack;

    unsigned br_idx = idb_bits_tree_s_get_stack_min_value_idx(a_bits_res,a_bits_res->root_idx,&stack_ptr);
    unsigned last_value = c_idx_not_exist;
    unsigned start_value = 0;

    do {
      idb_bits_s *idb_bits = idb_bits_tree_s_at(a_bits_res,br_idx);
      unsigned value = idb_bits->pos;

      ulli *bits_ptr = &idb_bits->bits0;
      ulli *bits_ptr_end = bits_ptr + IDB_BITS_SLOT_COUNT;
      do {
        ulli bit_mask = 1;
        do {
          if (*bits_ptr & bit_mask)
          {
            if (last_value == c_idx_not_exist)
            {
              // - range start -
              ui_array_s_push(a_ranges,value);
              start_value = value;
            }
            else
            {
              if (value != last_value + 1)
              {
                // - range end -
                ui_array_s_push(a_ranges,last_value - start_value);

                // - range start -
                ui_array_s_push(a_ranges,value - last_value);
                start_value = value;
              }
            }

            last_value = value;
          }
        } while(++value,bit_mask <<= 1);
      } while(++bits_ptr < bits_ptr_end);

      br_idx = idb_bits_tree_s_get_stack_next_idx(a_bits_res,br_idx,&stack_ptr,stack);
    } while(br_idx != c_idx_not_exist);

    // - range end -
    ui_array_s_push(a_ranges,last_value - start_value);
  }
}/*}}}*/

int id_daemon_s_channel_callback(void *a_id_daemon,unsigned a_index,unsigned a_type,va_list a_ap)
{/*{{{*/
  id_daemon_s *this = (id_daemon_s *)a_id_daemon;

  switch (a_type)
  {
  case id_channel_cbreq_NEW:
    {/*{{{*/
      while (a_index >= this->channel_watches.used)
      {
        var_array_s_push_loc(&this->channel_watches,loc_s_dict());
      }
    }/*}}}*/
    break;
  case id_channel_cbreq_DROP:
    {/*{{{*/
      var_map_tree_s *vm_tree = loc_s_dict_value(*var_array_s_at(&this->channel_watches,a_index));
      if (vm_tree->root_idx != c_idx_not_exist)
      {
        // - clear dictionary -
        var_map_tree_s_clear(vm_tree);
      }
    }/*}}}*/
    break;
  case id_channel_cbreq_EXTRACT:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *db_name = va_arg(a_ap,const string_s *);
      var_s data_var = va_arg(a_ap,var_s);

      idb_database_s search_db = {*db_name,};
      unsigned db_idx = idb_database_tree_s_get_idx(&this->databases,&search_db);
      if (db_idx == c_idx_not_exist)
      {
        throw_error(ID_DAEMON_DATABASE_NOT_EXIST);
      }

      idb_database_s *database = idb_database_tree_s_at(&this->databases,db_idx);

      // - update extractor of existing database -
      if (idb_database_s_update_extractor(database,data_var))
      {
        throw_error(ID_DAEMON_DATABASE_UPDATE_EXTRACTOR_ERROR);
      }

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"extract\",\"id\":%" HOST_LL_FORMAT "d",id);
      bc_array_s_push(&this->buffer,'}');

      if (id_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(ID_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case id_channel_cbreq_INDEX:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *db_name = va_arg(a_ap,const string_s *);
      const ui_array_s *doc_ids = va_arg(a_ap,const ui_array_s *);
      var_array_s *docs = va_arg(a_ap,var_array_s *);

      idb_database_s search_db = {*db_name,};
      unsigned db_idx = idb_database_tree_s_get_idx(&this->databases,&search_db);
      if (db_idx == c_idx_not_exist)
      {
        throw_error(ID_DAEMON_DATABASE_NOT_EXIST);
      }

      idb_database_s *database = idb_database_tree_s_at(&this->databases,db_idx);

      // - update index of database -
      if (idb_database_s_update_indexes(database,docs,doc_ids) ||
          idb_database_s_merge_indexes(database))
      {
        throw_error(ID_DAEMON_DATABASE_UPDATE_INDEX_ERROR);
      }

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"index\",\"id\":%" HOST_LL_FORMAT "d",id);
      bc_array_s_push(&this->buffer,'}');

      if (id_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(ID_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case id_channel_cbreq_REINDEX:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *db_name = va_arg(a_ap,const string_s *);
      const ui_array_s *doc_ids = va_arg(a_ap,const ui_array_s *);
      var_array_s *docs = va_arg(a_ap,var_array_s *);

      idb_database_s search_db = {*db_name,};
      unsigned db_idx = idb_database_tree_s_get_idx(&this->databases,&search_db);
      if (db_idx == c_idx_not_exist)
      {
        throw_error(ID_DAEMON_DATABASE_NOT_EXIST);
      }

      idb_database_s *database = idb_database_tree_s_at(&this->databases,db_idx);

      // - remove old document index from database -
      if (idb_database_s_remove_docs(database,doc_ids))
      {
        throw_error(ID_DAEMON_DATABASE_REMOVE_DOCS_ERROR);
      }

      // - update index of database -
      if (idb_database_s_update_indexes(database,docs,doc_ids) ||
          idb_database_s_merge_indexes(database))
      {
        throw_error(ID_DAEMON_DATABASE_UPDATE_INDEX_ERROR);
      }

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"reindex\",\"id\":%" HOST_LL_FORMAT "d",id);
      bc_array_s_push(&this->buffer,'}');

      if (id_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(ID_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case id_channel_cbreq_REMOVE:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *db_name = va_arg(a_ap,const string_s *);
      const ui_array_s *doc_ids = va_arg(a_ap,const ui_array_s *);

      idb_database_s search_db = {*db_name,};
      unsigned db_idx = idb_database_tree_s_get_idx(&this->databases,&search_db);
      if (db_idx == c_idx_not_exist)
      {
        throw_error(ID_DAEMON_DATABASE_NOT_EXIST);
      }

      idb_database_s *database = idb_database_tree_s_at(&this->databases,db_idx);

      // - remove document indexes from database -
      if (idb_database_s_remove_docs(database,doc_ids))
      {
        throw_error(ID_DAEMON_DATABASE_REMOVE_DOCS_ERROR);
      }

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"remove\",\"id\":%" HOST_LL_FORMAT "d",id);
      bc_array_s_push(&this->buffer,'}');

      if (id_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(ID_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case id_channel_cbreq_QUERY:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *db_name = va_arg(a_ap,const string_s *);
      const string_s *query = va_arg(a_ap,const string_s *);

      idb_database_s search_db = {*db_name,};
      unsigned db_idx = idb_database_tree_s_get_idx(&this->databases,&search_db);
      if (db_idx == c_idx_not_exist)
      {
        throw_error(ID_DAEMON_DATABASE_NOT_EXIST);
      }

      idb_database_s *database = idb_database_tree_s_at(&this->databases,db_idx);

      // - query database -
      if (idb_database_s_query(database,query))
      {
        throw_error(ID_DAEMON_DATABASE_QUERY_ERROR);
      }

      CONT_INIT_CLEAR(ui_array_s,query_res);
      idb_bits_tree_s_to_query_res(&database->bits_res,&query_res);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"query\",\"id\":%" HOST_LL_FORMAT "d,\"data\":",id);
      ui_array_s_to_json(&query_res,&this->buffer);
      bc_array_s_push(&this->buffer,'}');

      if (id_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(ID_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case id_channel_cbreq_QUERY_RANGES:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *db_name = va_arg(a_ap,const string_s *);
      const string_s *query = va_arg(a_ap,const string_s *);

      idb_database_s search_db = {*db_name,};
      unsigned db_idx = idb_database_tree_s_get_idx(&this->databases,&search_db);
      if (db_idx == c_idx_not_exist)
      {
        throw_error(ID_DAEMON_DATABASE_NOT_EXIST);
      }

      idb_database_s *database = idb_database_tree_s_at(&this->databases,db_idx);

      // - query database -
      if (idb_database_s_query(database,query))
      {
        throw_error(ID_DAEMON_DATABASE_QUERY_ERROR);
      }

      CONT_INIT_CLEAR(ui_array_s,ranges);
      id_daemon_bits_result_to_range_offsets(&database->bits_res,&ranges);

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"query_ranges\",\"id\":%" HOST_LL_FORMAT "d,\"data\":",id);
      ui_array_s_to_json(&ranges,&this->buffer);
      bc_array_s_push(&this->buffer,'}');

      if (id_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(ID_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
  case id_channel_cbreq_QUERY_RANGES_GZIP:
    {/*{{{*/
      lli id = va_arg(a_ap,lli);
      const string_s *db_name = va_arg(a_ap,const string_s *);
      const string_s *query = va_arg(a_ap,const string_s *);

      idb_database_s search_db = {*db_name,};
      unsigned db_idx = idb_database_tree_s_get_idx(&this->databases,&search_db);
      if (db_idx == c_idx_not_exist)
      {
        throw_error(ID_DAEMON_DATABASE_NOT_EXIST);
      }

      idb_database_s *database = idb_database_tree_s_at(&this->databases,db_idx);

      // - query database -
      if (idb_database_s_query(database,query))
      {
        throw_error(ID_DAEMON_DATABASE_QUERY_ERROR);
      }

      CONT_INIT_CLEAR(ui_array_s,ranges);
      id_daemon_bits_result_to_range_offsets(&database->bits_res,&ranges);

      CONT_INIT_CLEAR(file_s,tmp_file);
      CONT_INIT_CLEAR(gz_file_s,gz_file);
      this->compressed.used = 0;
      int tmp_fd;

      // - gzip compression through temporary file -
      if ((tmp_file = tmpfile()) == NULL ||
          (tmp_fd = dup(stream_s_fd(&tmp_file))) == -1 ||
          gz_file_s_fd_open(&gz_file,tmp_fd,"w") ||
          gz_file_s_write_close(&gz_file,ranges.data,ranges.used*sizeof(unsigned)) ||
          file_s_seek(&tmp_file,SEEK_SET,0) ||
          file_s_read_close(&tmp_file,&this->compressed))
      {
        throw_error(ID_DAEMON_GZIP_COMPRESSION_ERROR);
      }

      // - send response -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,"{\"resp\":\"query_ranges_gzip\",\"id\":%" HOST_LL_FORMAT "d,\"data\":\"",id);
      crypto_encode_base64(this->compressed.data,this->compressed.used,&this->buffer);
      bc_array_s_append_ptr(&this->buffer,"\"}");

      if (id_channel_s_send_message(&this->channel,a_index,&this->buffer))
      {
        throw_error(ID_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
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

int id_daemon_s_watchdog_channel_callback(void *a_id_daemon,unsigned a_index,unsigned a_type,va_list a_ap)
{/*{{{*/
  id_daemon_s *this = (id_daemon_s *)a_id_daemon;

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
        throw_error(ID_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }

      // - register watchdog update -
      this->buffer.used = 0;
      bc_array_s_append_format(&this->buffer,
          "{\"type\":\"watch\",\"id\":%" HOST_LL_FORMAT "d}",
          ++this->watchdog_channel.message_id);

      if (wd_channel_client_s_send_message(&this->watchdog_channel,&this->buffer))
      {
        throw_error(ID_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
      }

      // - schedule watchdog timer -
      struct itimerspec its_watchdog = {{30,0},{30,0}};
      if (epoll_s_timer_callback(&this->watchdog_timer,&its_watchdog,0,
            id_daemon_s_watchdog_time_event,this,0))
      {
        throw_error(ID_DAEMON_EPOLL_ERROR);
      }
    }/*}}}*/
    break;
  case wd_channel_cbreq_DROP:
    {/*{{{*/

      // - disable watchdog timer -
      struct itimerspec its_watchdog = {{0,0},{0,0}};
      if (epoll_timer_s_settime(&this->watchdog_timer,&its_watchdog,0))
      {
        throw_error(ID_DAEMON_TIMER_SETTIME_ERROR);
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

int id_daemon_s_watchdog_time_event(void *a_id_daemon,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(ID_DAEMON_TIMER_READ_ERROR);
  }

  id_daemon_s *this = (id_daemon_s *)a_id_daemon;

  this->buffer.used = 0;
  bc_array_s_append_format(&this->buffer,
      "{\"type\":\"keepalive\",\"id\":%" HOST_LL_FORMAT "d,\"name\":\"%s\"}",
      ++this->watchdog_channel.message_id,g_name);

  if (wd_channel_client_s_send_message(&this->watchdog_channel,&this->buffer))
  {
    throw_error(ID_DAEMON_CHANNEL_SEND_MESSAGE_ERROR);
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
  libcrypto_cll_init();
  libchannel_cll_init();
  libchannel_wdl_init();
  libchannel_idl_init();
  libconf_idl_init();
  libidb_idl_init();

  g_name = "id_daemon";
  g_conf = "id_config.json";

  // - process command line arguments -
  if (argc > 1)
  {
    int arg_idx = 1;
    do
    {
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
      CONT_INIT_CLEAR(id_daemon_s,daemon);

      if (id_daemon_s_create(&daemon) ||
          id_config_s_read_file(&daemon.config,g_conf))
      {
        break;
      }

      // - set configuration changed flag -
      daemon.config_changed_flag = 1;

      if (id_daemon_s_run(&daemon))
      {
        // - ignore return value, always terminate -
      }

    } while(0);
  }

  libidb_idl_clear();
  libconf_idl_clear();
  libchannel_idl_clear();
  libchannel_wdl_clear();
  libchannel_cll_clear();
  libcrypto_cll_clear();
  memcheck_release_assert();

  return 0;
}/*}}}*/

