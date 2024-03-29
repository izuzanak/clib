
@begin
include "sd_channel.h"
@end

// === constants and definitions ===============================================

#define MSG_REST() ((size_t)(ptr_end - ptr))

// === methods of generated structures =========================================

// -- sd_range_record_s --
@begin
methods sd_range_record_s
@end

// -- sd_range_records_s --
@begin
methods sd_range_records_s
@end

// -- sd_channel_s --
@begin
methods sd_channel_s
@end

int sd_channel_s_create(sd_channel_s *this,const char *a_ip,unsigned short a_port,
    sd_channel_cbreq_t a_channel_callback,
    void *a_cb_object)
{/*{{{*/
  sd_channel_s_clear(this);

  // - log message -
  log_info_2("channel server, create, ip: %s, port: %hu",a_ip,a_port);

  // - create channel server -
  if(channel_server_s_create(&this->server,a_ip,a_port,
        sd_channel_s_conn_new,
        sd_channel_s_conn_drop,
        sd_channel_s_conn_message,
        this))
  {
    throw_error(SD_CHANNEL_SERVER_CREATE_ERROR);
  }

  if(epoll_s_fd_callback(&this->server.epoll_fd,EPOLLIN | EPOLLPRI,sd_channel_s_fd_event,this,0))
  {
    throw_error(SD_CHANNEL_SERVER_EPOLL_ERROR);
  }

  this->channel_callback = a_channel_callback;
  this->cb_object = a_cb_object;

  return 0;
}/*}}}*/

int sd_channel_s_conn_new(void *a_sd_channel,unsigned a_index)
{/*{{{*/
  debug_message_5(fprintf(stderr,"sd_channel_s_conn_new: %u\n",a_index));

  // - log message -
  log_info_2("channel server %u, new connection",a_index);

  sd_channel_s *this = (sd_channel_s *)a_sd_channel;

  // - call callback -
  if (sd_channel_s_message_call(this,a_index,sd_channel_cbreq_NEW))
  {
    throw_error(SD_CHANNEL_SERVER_CALLBACK_ERROR);
  }

  return 0;
}/*}}}*/

int sd_channel_s_conn_drop(void *a_sd_channel,unsigned a_index)
{/*{{{*/
  debug_message_5(fprintf(stderr,"sd_channel_s_conn_drop: %u\n",a_index));

  // - log message -
  log_info_2("channel server %u, drop connection",a_index);

  sd_channel_s *this = (sd_channel_s *)a_sd_channel;

  // - call callback -
  if (sd_channel_s_message_call(this,a_index,sd_channel_cbreq_DROP))
  {
    throw_error(SD_CHANNEL_SERVER_CALLBACK_ERROR);
  }

  return 0;
}/*}}}*/

int sd_channel_s_send_multi_message_var(sd_channel_s *this,const ui_array_s *a_indexes,var_s a_data_var)
{/*{{{*/
  const bc_array_s *message = loc_s_channel_message_value(a_data_var);

  if (a_indexes->used != 0)
  {
    VAR_CLEAR(length_var,loc_s_channel_message_buffer_length(message));

    const unsigned *i_ptr = a_indexes->data;
    const unsigned *i_ptr_end = i_ptr + a_indexes->used;
    do
    {
      // - log message -
      log_info_2("channel server %u, --> %u bytes",*i_ptr,
          loc_s_channel_message_value(a_data_var)->used);

      // - schedule message to send -
      if (channel_conn_s_schedule_message_var(&this->server.conn_list.data[*i_ptr].object,length_var,a_data_var))
      {
        throw_error(SD_CHANNEL_SERVER_SCHEDULE_MESSAGE_ERROR);
      }
    } while(++i_ptr < i_ptr_end);
  }

  return 0;
}/*}}}*/

int sd_channel_s_conn_message(void *a_sd_channel,unsigned a_index,const bc_array_s *a_message)
{/*{{{*/
  debug_message_5(fprintf(stderr,"conn_message: %u - %u bytes\n",a_index,a_message->used));

  // - log message -
  log_info_2("channel server %u, <-- %u bytes",a_index,a_message->used);

  sd_channel_s *this = (sd_channel_s *)a_sd_channel;

  char *ptr = a_message->data;
  char *ptr_end = ptr + a_message->used;

  if (MSG_REST() >= sizeof(ulli) + sizeof(usi))
  {
    ulli id;
    memcpy(&id,ptr,sizeof(ulli)); ptr += sizeof(ulli);
    id = be64toh(id);

    usi type;
    memcpy(&type,ptr,sizeof(usi)); ptr += sizeof(usi);
    type = be16toh(type);

    switch (type)
    {
      case sd_channel_msg_type_REQUEST:
        {/*{{{*/
          if (MSG_REST() >= sizeof(usi))
          {
            usi request;
            memcpy(&request,ptr,sizeof(usi)); ptr += sizeof(usi);
            request = be16toh(request);

            switch (request)
            {
              case sd_channel_cbreq_SEGMENT_INFO:
              case sd_channel_cbreq_SEGMENT_WRITE:
              case sd_channel_cbreq_SEGMENT_READ:
              case sd_channel_cbreq_SEGMENT_WATCH:
              case sd_channel_cbreq_SEGMENT_IGNORE:
                {/*{{{*/
                  unsigned rest_length = a_message->used - (ptr - a_message->data);
                  unsigned segment_id_length = strnlen(ptr,rest_length);
                  if (segment_id_length < rest_length)
                  {
                    const string_s segment_id = {segment_id_length + 1,ptr};
                    ptr += segment_id.size;

                    switch (request)
                    {
                      case sd_channel_cbreq_SEGMENT_INFO:
                      case sd_channel_cbreq_SEGMENT_READ:
                      case sd_channel_cbreq_SEGMENT_WATCH:
                      case sd_channel_cbreq_SEGMENT_IGNORE:
                        {/*{{{*/

                          // - call callback -
                          if (sd_channel_s_message_call(
                                this,a_index,request,id,
                                &segment_id))
                          {
                            throw_error(SD_CHANNEL_SERVER_CALLBACK_ERROR);
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_SEGMENT_WRITE:
                        {/*{{{*/
                          if (MSG_REST() >= sizeof(ulli))
                          {
                            ulli time;
                            memcpy(&time,ptr,sizeof(ulli)); ptr += sizeof(ulli);
                            time = be64toh(time);

                            const bc_array_s data = {MSG_REST(),MSG_REST(),ptr};

                            // - call callback -
                            if (sd_channel_s_message_call(
                                  this,a_index,request,id,
                                  &segment_id,time,&data))
                            {
                              throw_error(SD_CHANNEL_SERVER_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                    }
                  }
                }/*}}}*/
                break;
              case sd_channel_cbreq_TRACE_INFO:
              case sd_channel_cbreq_TRACE_WRITE:
              case sd_channel_cbreq_TRACE_READ:
              case sd_channel_cbreq_TRACE_HEAD:
              case sd_channel_cbreq_TRACE_TAIL:
              case sd_channel_cbreq_TRACE_RANGE:
              case sd_channel_cbreq_TRACE_STEP_RANGE:
              case sd_channel_cbreq_TRACE_TIME_RANGE:
              case sd_channel_cbreq_TRACE_LEE_TIME:
              case sd_channel_cbreq_TRACE_GRE_TIME:
              case sd_channel_cbreq_TRACE_WATCH:
              case sd_channel_cbreq_TRACE_IGNORE:
                {/*{{{*/
                  unsigned rest_length = a_message->used - (ptr - a_message->data);
                  unsigned trace_id_length = strnlen(ptr,rest_length);
                  if (trace_id_length < rest_length)
                  {
                    const string_s trace_id = {trace_id_length + 1,ptr};
                    ptr += trace_id.size;

                    switch (request)
                    {
                      case sd_channel_cbreq_TRACE_INFO:
                      case sd_channel_cbreq_TRACE_HEAD:
                      case sd_channel_cbreq_TRACE_TAIL:
                      case sd_channel_cbreq_TRACE_WATCH:
                      case sd_channel_cbreq_TRACE_IGNORE:
                        {/*{{{*/

                          // - call callback -
                          if (sd_channel_s_message_call(
                                this,a_index,request,id,
                                &trace_id))
                          {
                            throw_error(SD_CHANNEL_SERVER_CALLBACK_ERROR);
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_TRACE_WRITE:
                        {/*{{{*/
                          if (MSG_REST() >= sizeof(ulli))
                          {
                            ulli time;
                            memcpy(&time,ptr,sizeof(ulli)); ptr += sizeof(ulli);
                            time = be64toh(time);

                            const bc_array_s data = {MSG_REST(),MSG_REST(),ptr};

                            // - call callback -
                            if (sd_channel_s_message_call(
                                  this,a_index,request,id,
                                  &trace_id,time,&data))
                            {
                              throw_error(SD_CHANNEL_SERVER_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_TRACE_READ:
                        {/*{{{*/
                          if (MSG_REST() >= sizeof(lli))
                          {
                            lli record_id;
                            memcpy(&record_id,ptr,sizeof(lli));
                            record_id = be64toh(record_id);

                            // - call callback -
                            if (sd_channel_s_message_call(
                                  this,a_index,request,id,
                                  &trace_id,record_id))
                            {
                              throw_error(SD_CHANNEL_SERVER_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_TRACE_RANGE:
                        {/*{{{*/
                          if (MSG_REST() >= 3*sizeof(lli))
                          {
                            lli first_id;
                            memcpy(&first_id,ptr,sizeof(lli)); ptr += sizeof(lli);
                            first_id = be64toh(first_id);

                            lli last_id;
                            memcpy(&last_id,ptr,sizeof(lli)); ptr += sizeof(lli);
                            last_id = be64toh(last_id);

                            lli count;
                            memcpy(&count,ptr,sizeof(lli));
                            count = be64toh(count);

                            // - call callback -
                            if (sd_channel_s_message_call(
                                  this,a_index,request,id,
                                  &trace_id,first_id,last_id,count))
                            {
                              throw_error(SD_CHANNEL_SERVER_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_TRACE_STEP_RANGE:
                        {/*{{{*/
                          if (MSG_REST() >= 3*sizeof(lli))
                          {
                            lli first_id;
                            memcpy(&first_id,ptr,sizeof(lli)); ptr += sizeof(lli);
                            first_id = be64toh(first_id);

                            lli last_id;
                            memcpy(&last_id,ptr,sizeof(lli)); ptr += sizeof(lli);
                            last_id = be64toh(last_id);

                            ulli step;
                            memcpy(&step,ptr,sizeof(ulli));
                            step = be64toh(step);

                            // - call callback -
                            if (sd_channel_s_message_call(
                                  this,a_index,request,id,
                                  &trace_id,first_id,last_id,step))
                            {
                              throw_error(SD_CHANNEL_SERVER_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_TRACE_TIME_RANGE:
                        {/*{{{*/
                          if (MSG_REST() >= 3*sizeof(lli))
                          {
                            ulli first_time;
                            memcpy(&first_time,ptr,sizeof(ulli)); ptr += sizeof(ulli);
                            first_time = be64toh(first_time);

                            ulli last_time;
                            memcpy(&last_time,ptr,sizeof(ulli)); ptr += sizeof(ulli);
                            last_time = be64toh(last_time);

                            lli count;
                            memcpy(&count,ptr,sizeof(lli));
                            count = be64toh(count);

                            // - call callback -
                            if (sd_channel_s_message_call(
                                  this,a_index,request,id,
                                  &trace_id,first_time,last_time,count))
                            {
                              throw_error(SD_CHANNEL_SERVER_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_TRACE_LEE_TIME:
                      case sd_channel_cbreq_TRACE_GRE_TIME:
                        {/*{{{*/
                          if (MSG_REST() >= sizeof(ulli))
                          {
                            ulli time;
                            memcpy(&time,ptr,sizeof(ulli));
                            time = be64toh(time);

                            // - call callback -
                            if (sd_channel_s_message_call(
                                  this,a_index,request,id,
                                  &trace_id,time))
                            {
                              throw_error(SD_CHANNEL_SERVER_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                    }
                  }
                }/*}}}*/
                break;
              case sd_channel_cbreq_CONFIG:
                {/*{{{*/
                  const bc_array_s data = {MSG_REST(),MSG_REST(),ptr};

                  // - call callback -
                  if (sd_channel_s_message_call(
                        this,a_index,request,id,
                        &data))
                  {
                    throw_error(SD_CHANNEL_SERVER_CALLBACK_ERROR);
                  }
                }/*}}}*/
                break;
              case sd_channel_cbreq_PING:
                {/*{{{*/

                  // - send response -
                  this->buffer.used = 0;
                  bc_array_s_append_be_ulli(&this->buffer,id);
                  bc_array_s_append_be_usi(&this->buffer,sd_channel_msg_type_RESPONSE);
                  bc_array_s_append_be_usi(&this->buffer,request);

                  if (sd_channel_s_send_message(this,a_index,&this->buffer))
                  {
                    throw_error(SD_CHANNEL_SERVER_SEND_MESSAGE_ERROR);
                  }
                }/*}}}*/
                break;
            }
          }
        }/*}}}*/
        break;
      case sd_channel_msg_type_RESPONSE:
        break;
      case sd_channel_msg_type_EVENT:
        break;
    }
  }

  return 0;
}/*}}}*/

int sd_channel_s_fd_event(void *a_sd_channel,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  sd_channel_s *this = (sd_channel_s *)a_sd_channel;

  if(channel_server_s_fd_event(&this->server,a_index,a_epoll_event))
  {
    throw_error(SD_CHANNEL_SERVER_FD_EVENT_ERROR);
  }

  return 0;
}/*}}}*/

// -- sd_channel_client_s --
@begin
methods sd_channel_client_s
@end

int sd_channel_client_s_create(sd_channel_client_s *this,
  const char *a_server_ip,unsigned short a_server_port,
  sd_channel_cbreq_t a_channel_callback,
  void *a_cb_object,unsigned a_cb_index)
{/*{{{*/
  sd_channel_client_s_clear(this);

  // - log message -
  log_info_2("channel client, create, server_ip: %s, server_port: %hu",
      a_server_ip,a_server_port);

  string_s_set_ptr(&this->server_ip,a_server_ip);
  this->server_port = a_server_port;
  this->message_id = 0;

  this->channel_callback = a_channel_callback;
  this->cb_object = a_cb_object;
  this->cb_index = a_cb_index;

  // - register epoll timer -
  struct itimerspec itimerspec = {{0,0},{0,1}};
  if (epoll_s_timer_callback(&this->epoll_timer,&itimerspec,0,sd_channel_client_s_connect_time_event,this,0))
  {
    throw_error(SD_CHANNEL_CLIENT_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/

int sd_channel_client_s_conn_message(void *a_sd_channel_client,unsigned a_index,const bc_array_s *a_message)
{/*{{{*/
  (void)a_sd_channel_client;
  (void)a_index;

  debug_message_6(fprintf(stderr,"sd_channel_client_s_conn_message\n"));

  // - log message -
  log_info_2("channel client, <-- %u bytes",a_message->used);

  sd_channel_client_s *this = (sd_channel_client_s *)a_sd_channel_client;

  char *ptr = a_message->data;
  char *ptr_end = ptr + a_message->used;

  if (MSG_REST() >= sizeof(ulli) + sizeof(usi))
  {
    ulli id;
    memcpy(&id,ptr,sizeof(ulli)); ptr += sizeof(ulli);
    id = be64toh(id);

    usi type;
    memcpy(&type,ptr,sizeof(usi)); ptr += sizeof(usi);
    type = be16toh(type);

    switch (type)
    {
      case sd_channel_msg_type_REQUEST:
        break;
      case sd_channel_msg_type_RESPONSE:
        {/*{{{*/
          if (MSG_REST() >= sizeof(usi))
          {
            usi request;
            memcpy(&request,ptr,sizeof(usi)); ptr += sizeof(usi);
            request = be16toh(request);

            switch (request)
            {
              case sd_channel_cbreq_SEGMENT_INFO:
              case sd_channel_cbreq_SEGMENT_WRITE:
              case sd_channel_cbreq_SEGMENT_READ:
              case sd_channel_cbreq_SEGMENT_WATCH:
              case sd_channel_cbreq_SEGMENT_IGNORE:
                {/*{{{*/
                  unsigned rest_length = a_message->used - (ptr - a_message->data);
                  unsigned segment_id_length = strnlen(ptr,rest_length);
                  if (segment_id_length < rest_length)
                  {
                    const string_s segment_id = {segment_id_length + 1,ptr};
                    ptr += segment_id.size;

                    switch (request)
                    {
                      case sd_channel_cbreq_SEGMENT_INFO:
                        {/*{{{*/
                          if (MSG_REST() >= sizeof(unsigned))
                          {
                            unsigned rec_size;
                            memcpy(&rec_size,ptr,sizeof(unsigned));
                            rec_size = be32toh(rec_size);

                            // - call callback -
                            if (sd_channel_client_s_message_call(
                                  this,request,id,
                                  &segment_id,rec_size))
                            {
                              throw_error(SD_CHANNEL_CLIENT_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_SEGMENT_READ:
                        {/*{{{*/
                          if (MSG_REST() >= sizeof(ulli))
                          {
                            ulli time;
                            memcpy(&time,ptr,sizeof(ulli)); ptr += sizeof(ulli);
                            time = be64toh(time);

                            const bc_array_s data = {MSG_REST(),MSG_REST(),ptr};

                            // - call callback -
                            if (sd_channel_client_s_message_call(
                                  this,request,id,
                                  &segment_id,time,&data))
                            {
                              throw_error(SD_CHANNEL_CLIENT_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_SEGMENT_WRITE:
                      case sd_channel_cbreq_SEGMENT_WATCH:
                      case sd_channel_cbreq_SEGMENT_IGNORE:
                        {/*{{{*/

                          // - call callback -
                          if (sd_channel_client_s_message_call(
                                this,request,id,
                                &segment_id))
                          {
                            throw_error(SD_CHANNEL_CLIENT_CALLBACK_ERROR);
                          }
                        }/*}}}*/
                        break;
                    }
                  }
                }/*}}}*/
                break;
              case sd_channel_cbreq_TRACE_INFO:
              case sd_channel_cbreq_TRACE_WRITE:
              case sd_channel_cbreq_TRACE_READ:
              case sd_channel_cbreq_TRACE_HEAD:
              case sd_channel_cbreq_TRACE_TAIL:
              case sd_channel_cbreq_TRACE_RANGE:
              case sd_channel_cbreq_TRACE_STEP_RANGE:
              case sd_channel_cbreq_TRACE_TIME_RANGE:
              case sd_channel_cbreq_TRACE_LEE_TIME:
              case sd_channel_cbreq_TRACE_GRE_TIME:
              case sd_channel_cbreq_TRACE_WATCH:
              case sd_channel_cbreq_TRACE_IGNORE:
                {/*{{{*/
                  unsigned rest_length = a_message->used - (ptr - a_message->data);
                  unsigned trace_id_length = strnlen(ptr,rest_length);
                  if (trace_id_length < rest_length)
                  {
                    const string_s trace_id = {trace_id_length + 1,ptr};
                    ptr += trace_id.size;

                    switch (request)
                    {
                      case sd_channel_cbreq_TRACE_INFO:
                        {/*{{{*/
                          if (MSG_REST() >= sizeof(unsigned) + 2*sizeof(lli))
                          {
                            unsigned rec_size;
                            memcpy(&rec_size,ptr,sizeof(unsigned)); ptr += sizeof(unsigned);
                            rec_size = be32toh(rec_size);

                            lli head_id;
                            memcpy(&head_id,ptr,sizeof(lli)); ptr += sizeof(lli);
                            head_id = be64toh(head_id);

                            lli tail_id;
                            memcpy(&tail_id,ptr,sizeof(lli));
                            tail_id = be64toh(tail_id);

                            // - call callback -
                            if (sd_channel_client_s_message_call(
                                  this,request,id,
                                  &trace_id,rec_size,head_id,tail_id))
                            {
                              throw_error(SD_CHANNEL_CLIENT_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_TRACE_HEAD:
                      case sd_channel_cbreq_TRACE_TAIL:
                      case sd_channel_cbreq_TRACE_LEE_TIME:
                      case sd_channel_cbreq_TRACE_GRE_TIME:
                      case sd_channel_cbreq_TRACE_READ:
                        {/*{{{*/
                          if (MSG_REST() >= sizeof(lli) + sizeof(ulli))
                          {
                            lli record_id;
                            memcpy(&record_id,ptr,sizeof(lli)); ptr += sizeof(lli);
                            record_id = be64toh(record_id);

                            ulli time;
                            memcpy(&time,ptr,sizeof(ulli)); ptr += sizeof(ulli);
                            time = be64toh(time);

                            const bc_array_s data = {MSG_REST(),MSG_REST(),ptr};

                            // - call callback -
                            if (sd_channel_client_s_message_call(
                                  this,request,id,
                                  &trace_id,record_id,time,&data))
                            {
                              throw_error(SD_CHANNEL_CLIENT_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_TRACE_RANGE:
                      case sd_channel_cbreq_TRACE_STEP_RANGE:
                      case sd_channel_cbreq_TRACE_TIME_RANGE:
                        {/*{{{*/
                          if (MSG_REST() < sizeof(ulli))
                          {
                            break;
                          }

                          ulli resp_count;
                          memcpy(&resp_count,ptr,sizeof(ulli)); ptr += sizeof(ulli);
                          resp_count = be64toh(resp_count);

                          CONT_INIT(sd_range_records_s,range_records);

                          do
                          {
                            if (resp_count > 0)
                            {
                              sd_range_records_s_push_blanks(&range_records,resp_count);

                              sd_range_record_s *rr_ptr = range_records.data;
                              sd_range_record_s *rr_ptr_end = rr_ptr + resp_count;
                              do
                              {
                                if (MSG_REST() < sizeof(lli) + sizeof(ulli) + sizeof(ui))
                                {
                                  break;
                                }

                                memcpy(&rr_ptr->record_id,ptr,sizeof(lli)); ptr += sizeof(lli);
                                rr_ptr->record_id = be64toh(rr_ptr->record_id);

                                memcpy(&rr_ptr->time,ptr,sizeof(ulli)); ptr += sizeof(ulli);
                                rr_ptr->time = be64toh(rr_ptr->time);

                                unsigned record_size;
                                memcpy(&record_size,ptr,sizeof(ui)); ptr += sizeof(unsigned);
                                record_size = be32toh(record_size);

                                if (MSG_REST() < record_size)
                                {
                                  break;
                                }

                                rr_ptr->record.size = record_size;
                                rr_ptr->record.used = record_size;
                                rr_ptr->record.data = ptr; ptr += record_size;

                              } while(++rr_ptr < rr_ptr_end);

                              // - all records were retrieved -
                              if (rr_ptr < rr_ptr_end)
                              {
                                break;
                              }
                            }

                            // - call callback -
                            if (sd_channel_client_s_message_call(
                                  this,request,id,
                                  &trace_id,&range_records))
                            {
                              // - release range records -
                              if (range_records.data != NULL)
                              {
                                cfree(range_records.data);
                              }

                              throw_error(SD_CHANNEL_CLIENT_CALLBACK_ERROR);
                            }
                          } while(0);

                          // - release range records -
                          if (range_records.data != NULL)
                          {
                            cfree(range_records.data);
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_TRACE_WRITE:
                        {/*{{{*/
                          if (MSG_REST() >= sizeof(lli))
                          {
                            lli record_id;
                            memcpy(&record_id,ptr,sizeof(lli));
                            record_id = be64toh(record_id);

                            // - call callback -
                            if (sd_channel_client_s_message_call(
                                  this,request,id,
                                  &trace_id,record_id))
                            {
                              throw_error(SD_CHANNEL_CLIENT_CALLBACK_ERROR);
                            }
                          }
                        }/*}}}*/
                        break;
                      case sd_channel_cbreq_TRACE_WATCH:
                      case sd_channel_cbreq_TRACE_IGNORE:
                        {/*{{{*/

                          // - call callback -
                          if (sd_channel_client_s_message_call(
                                this,request,id,
                                &trace_id))
                          {
                            throw_error(SD_CHANNEL_CLIENT_CALLBACK_ERROR);
                          }
                        }/*}}}*/
                        break;
                    }
                  }
                }/*}}}*/
                break;
            }
          }
        }/*}}}*/
        break;
      case sd_channel_msg_type_EVENT:
        {/*{{{*/
          if (MSG_REST() >= sizeof(usi))
          {
            usi request;
            memcpy(&request,ptr,sizeof(usi)); ptr += sizeof(usi);
            request = be16toh(request);

            switch (request)
            {
              case sd_channel_cbreq_SEGMENT_UPDATE:
                {/*{{{*/
                  unsigned rest_length = a_message->used - (ptr - a_message->data);
                  unsigned segment_id_length = strnlen(ptr,rest_length);
                  if (segment_id_length < rest_length)
                  {
                    const string_s segment_id = {segment_id_length + 1,ptr};
                    ptr += segment_id.size;

                    if (MSG_REST() >= sizeof(ulli))
                    {
                      ulli time;
                      memcpy(&time,ptr,sizeof(ulli)); ptr += sizeof(ulli);
                      time = be64toh(time);

                      const bc_array_s data = {MSG_REST(),MSG_REST(),ptr};

                      // - call callback -
                      if (sd_channel_client_s_message_call(
                            this,request,id,
                            &segment_id,time,&data))
                      {
                        throw_error(SD_CHANNEL_CLIENT_CALLBACK_ERROR);
                      }
                    }
                  }
                }/*}}}*/
                break;
              case sd_channel_cbreq_TRACE_UPDATE:
                {/*{{{*/
                  unsigned rest_length = a_message->used - (ptr - a_message->data);
                  unsigned trace_id_length = strnlen(ptr,rest_length);
                  if (trace_id_length < rest_length)
                  {
                    const string_s trace_id = {trace_id_length + 1,ptr};
                    ptr += trace_id.size;

                    if (MSG_REST() >= sizeof(lli) + sizeof(ulli))
                    {
                      lli record_id;
                      memcpy(&record_id,ptr,sizeof(lli)); ptr += sizeof(lli);
                      record_id = be64toh(record_id);

                      ulli time;
                      memcpy(&time,ptr,sizeof(ulli)); ptr += sizeof(ulli);
                      time = be64toh(time);

                      const bc_array_s data = {MSG_REST(),MSG_REST(),ptr};

                      // - call callback -
                      if (sd_channel_client_s_message_call(
                            this,request,id,
                            &trace_id,record_id,time,&data))
                      {
                        throw_error(SD_CHANNEL_CLIENT_CALLBACK_ERROR);
                      }
                    }
                  }
                }/*}}}*/
                break;
            }
          }
        }/*}}}*/
        break;
    }
  }

  return 0;
}/*}}}*/

int sd_channel_client_s_fd_event(void *a_sd_channel_client,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  sd_channel_client_s *this = (sd_channel_client_s *)a_sd_channel_client;

  // - store connecting state -
  int connecting = this->connection.connecting;

  if (channel_conn_s_fd_event(&this->connection,0,a_epoll_event))
  {
    channel_conn_s_clear(&this->connection);

    // - register epoll timer -
    struct itimerspec itimerspec = {{0,0},{0,500000000}};
    if (epoll_s_timer_callback(&this->epoll_timer,&itimerspec,0,sd_channel_client_s_connect_time_event,this,0))
    {
      throw_error(SD_CHANNEL_CLIENT_EPOLL_ERROR);
    }

    if (!connecting)
    {
      // - call callback -
      if (sd_channel_client_s_message_call(this,sd_channel_cbreq_DROP))
      {
        throw_error(SD_CHANNEL_CLIENT_CALLBACK_ERROR);
      }
    }
  }
  else
  {
    if (connecting)
    {
      // - call callback -
      if (sd_channel_client_s_message_call(this,sd_channel_cbreq_NEW))
      {
        throw_error(SD_CHANNEL_CLIENT_CALLBACK_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int sd_channel_client_s_connect_time_event(void *a_sd_channel_client,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(SD_CHANNEL_CLIENT_TIMER_READ_ERROR);
  }

  sd_channel_client_s *this = (sd_channel_client_s *)a_sd_channel_client;

  if (channel_conn_s_create_client(&this->connection,
        this->server_ip.data,this->server_port,sd_channel_client_s_conn_message,this,0) ||
      epoll_s_fd_callback(&this->connection.epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI,sd_channel_client_s_fd_event,this,0))
  {
    // - register epoll timer -
    struct itimerspec itimerspec = {{0,0},{0,500000000}};
    if (epoll_s_timer_callback(&this->epoll_timer,&itimerspec,0,sd_channel_client_s_connect_time_event,this,0))
    {
      throw_error(SD_CHANNEL_CLIENT_EPOLL_ERROR);
    }
  }

  return 0;
}/*}}}*/

