
@begin
include "cl_linux.h"
@end

// === methods of structure fd_s ===============================================

int fd_s_write(const fd_s *this,const void *a_src,size_t a_size)
{/*{{{*/
  debug_assert(*this != -1);

  if (a_size > 0)
  {
    long int cnt;
    unsigned writed = 0;

    do
    {
      // - ERROR -
      if ((cnt = write(*this,(const char *)a_src + writed,a_size - writed)) == -1)
      {
        throw_error(FD_WRITE_ERROR);
      }
    }
    while((writed += cnt) < a_size);
  }

  return 0;
}/*}}}*/

int fd_s_read(const fd_s *this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(*this != -1);

  const long int c_buffer_add = 1024;

  int inq_cnt;
  long int read_cnt;
  do
  {
    bc_array_s_reserve(a_trg,c_buffer_add);
    read_cnt = read(*this,a_trg->data + a_trg->used,c_buffer_add);

    // - ERROR -
    if (read_cnt == -1)
    {
      throw_error(FD_READ_ERROR);
    }

    a_trg->used += read_cnt;

    // - ERROR -
    if (ioctl(*this,TIOCINQ,&inq_cnt) == -1)
    {
      throw_error(FD_READ_ERROR);
    }
  }
  while(inq_cnt > 0);

  return 0;
}/*}}}*/

int fd_s_read_cnt(const fd_s *this,size_t a_count,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(*this != -1);

  bc_array_s_push_blanks(a_trg,a_count);

  unsigned readed = 0;
  long int read_cnt;

  do
  {
    read_cnt = read(*this,a_trg->data + readed,a_count - readed);

    // - ERROR -
    if (read_cnt == -1)
    {
      throw_error(FD_READ_ERROR);
    }

    readed += read_cnt;
  }
  while(readed < a_count && read_cnt != 0);

  // - ERROR -
  if (readed < a_count)
  {
    throw_error(FD_READ_ERROR);
  }

  return 0;
}/*}}}*/

// === methods of structure socket_s ===========================================

int socket_s_bind(const socket_s *this,const socket_address_s *a_addr)
{/*{{{*/
  debug_assert(*this != -1);

#ifndef DISABLE_SO_DOMAIN
  // - retrieve socket domain -
  int domain;
  socklen_t domain_len = sizeof(int);
  getsockopt(*this,SOL_SOCKET,SO_DOMAIN,&domain,&domain_len);

  switch (domain)
  {
  case AF_INET:
  {
#endif
    // - ERROR -
    if (bind(*this,(struct sockaddr *)a_addr,sizeof(struct sockaddr_in)) != 0)
    {
      throw_error(SOCKET_BIND_ERROR);
    }
#ifndef DISABLE_SO_DOMAIN
  }
  break;

  // - ERROR -
  default:
    throw_error(SOCKET_UNKNOWN_DOMAIN);
  }
#endif

  return 0;
}/*}}}*/

int socket_s_listen(const socket_s *this,const socket_address_s *a_addr,int a_backlog)
{/*{{{*/
  debug_assert(*this != -1);

  // - set server socket as reusable -
  int yes = 1;
  setsockopt(*this,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

#ifndef DISABLE_SO_DOMAIN
  // - retrieve socket domain -
  int domain;
  socklen_t domain_len = sizeof(int);
  getsockopt(*this,SOL_SOCKET,SO_DOMAIN,&domain,&domain_len);

  switch (domain)
  {
  case AF_INET:
  {
#endif
    // - ERROR -
    if (bind(*this,(struct sockaddr *)a_addr,sizeof(struct sockaddr_in)) != 0)
    {
      throw_error(SOCKET_BIND_ERROR);
    }

    // - ERROR -
    if (listen(*this,a_backlog) != 0)
    {
      throw_error(SOCKET_LISTEN_ERROR);
    }
#ifndef DISABLE_SO_DOMAIN
  }
  break;

  // - ERROR -
  default:
    throw_error(SOCKET_UNKNOWN_DOMAIN);
  }
#endif

  return 0;
}/*}}}*/

int socket_s_accept(const socket_s *this,socket_s *a_socket,socket_address_s *a_addr)
{/*{{{*/
  debug_assert(*this != -1);

  // - clear target socket -
  socket_s_clear(a_socket);

#ifndef DISABLE_SO_DOMAIN
  // - retrieve socket domain -
  int domain;
  socklen_t domain_len = sizeof(int);
  getsockopt(*this,SOL_SOCKET,SO_DOMAIN,&domain,&domain_len);

  switch (domain)
  {
  case AF_INET:
  {
#endif
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // - ERROR -
    *a_socket = accept(*this,(struct sockaddr *)a_addr,&addr_len);
    if (*a_socket < 0)
    {
      throw_error(SOCKET_ACCEPT_ERROR);
    }

    // - ERROR -
    if (addr_len != sizeof(struct sockaddr_in))
    {
      socket_s_clear(a_socket);

      throw_error(SOCKET_ADDRESS_UNKNOWN_FORMAT);
    }
#ifndef DISABLE_SO_DOMAIN
  }
  break;

  // - ERROR -
  default:
    throw_error(SOCKET_UNKNOWN_DOMAIN);
  }
#endif

  return 0;
}/*}}}*/

int socket_s_connect(const socket_s *this,const socket_address_s *a_addr)
{/*{{{*/
  debug_assert(*this != -1);

#ifndef DISABLE_SO_DOMAIN
  // - retrieve socket domain -
  int domain;
  socklen_t domain_len = sizeof(int);
  getsockopt(*this,SOL_SOCKET,SO_DOMAIN,&domain,&domain_len);

  switch (domain)
  {
  case AF_INET:
  {
#endif
    // - ERROR -
    if (connect(*this,(struct sockaddr *)a_addr,sizeof(struct sockaddr_in)) != 0)
    {
      throw_error(SOCKET_CONNECT_ERROR);
    }
#ifndef DISABLE_SO_DOMAIN
  }
  break;

  // - ERROR -
  default:
    throw_error(SOCKET_UNKNOWN_DOMAIN);
  }
#endif

  return 0;
}/*}}}*/

int socket_s_sendto(const socket_s *this,const socket_address_s *a_addr,
    const void *a_src,size_t a_size)
{/*{{{*/
  debug_assert(*this != -1);

  if (a_size > 0)
  {
    long int cnt;
    unsigned writed = 0;

    do
    {
      // - ERROR -
      if ((cnt = sendto(*this,(const char *)a_src + writed,a_size - writed,0,
              (struct sockaddr *)a_addr,sizeof(struct sockaddr_in))) == -1)
      {
        throw_error(SOCKET_SENDTO_ERROR);
      }
    }
    while((writed += cnt) < a_size);
  }

  return 0;
}/*}}}*/

int socket_s_recvfrom(const socket_s *this,bc_array_s *a_trg,socket_address_s *a_addr)
{/*{{{*/
  debug_assert(*this != -1);

  // - max count of udp message bytes -
  const ssize_t c_udp_max_cnt = 4096;

  // - prepare target buffer -
  a_trg->used = 0;
  bc_array_s_reserve(a_trg,c_udp_max_cnt);

  // - source address length -
  socklen_t addr_len = sizeof(struct sockaddr_in);

  // - ERROR -
  ssize_t cnt;
  if ((cnt = recvfrom(*this,a_trg->data,a_trg->size,0,(struct sockaddr *)a_addr,&addr_len)) == -1)
  {
    throw_error(SOCKET_RECVFROM_ERROR);
  }

  // - ERROR -
  if (cnt >= c_udp_max_cnt)
  {
    throw_error(SOCKET_UDP_MAX_MSG_SIZE_EXCEEDED);
  }

  // - update target buffer -
  a_trg->used = cnt;

  // - ERROR -
  if (addr_len != sizeof(struct sockaddr_in))
  {
    throw_error(SOCKET_ADDRESS_UNKNOWN_FORMAT);
  }

  return 0;
}/*}}}*/

// === methods of structure pid_s ==============================================

int pid_s_execute(pid_s *this,const string_array_s *a_arguments)
{/*{{{*/
  pid_s_clear(this);

  // - ERROR -
  if (a_arguments->used == 0)
  {
    throw_error(PID_MISSING_PROGRAM_NAME);
  }

  pid_t pid = fork();

  // - ERROR -
  if (pid == -1)
  {
    throw_error(PID_CANNOT_CREATE_NEW_PROCESS);
  }

  // - process is child -
  if (pid == 0)
  {
    char *arguments[a_arguments->used + 1];
    char **a_ptr = arguments;

    // - initialize argument list -
    const string_s *s_ptr = a_arguments->data;
    const string_s *s_ptr_end = s_ptr + a_arguments->used;
    do {
      *a_ptr++ = s_ptr->data;
    } while(++s_ptr < s_ptr_end);

    // - add terminating argument -
    *a_ptr = NULL;

    // - execute target process -
    if (execvp(a_arguments->data[0].data,arguments) == -1)
    {
      exit(127);
    }
  }

  *this = pid;

  return 0;
}/*}}}*/

// === methods of structure signal_s ===========================================

int signal_s_simple_handler(signal_callback_t a_handler)
{/*{{{*/

  // - create signal ignore action -
  struct sigaction ignore_action;
  ignore_action.sa_handler = SIG_IGN;
  if (sigemptyset(&ignore_action.sa_mask))
  {
    throw_error(SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR);
  }
  ignore_action.sa_flags = 0;

  // - ignore SIGPIPE signal -
  if (sigaction(SIGPIPE,&ignore_action,NULL))
  {
    throw_error(SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR);
  }

  // - create signal handler action -
  struct sigaction signal_action;
  signal_action.sa_handler = a_handler;
  if (sigemptyset(&signal_action.sa_mask))
  {
    throw_error(SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR);
  }
  signal_action.sa_flags = 0;

  struct sigaction old_action;
  const int signals[] = {SIGINT,SIGTERM,SIGHUP,SIGCHLD,SIGUSR1,SIGALRM};

  const int *s_ptr = signals;
  const int *s_ptr_end = s_ptr + sizeof(signals)/sizeof(signals[0]);
  do {
    if (sigaction(*s_ptr,NULL,&old_action))
    {
      throw_error(SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR);
    }

    if (old_action.sa_handler != SIG_IGN)
    {
      if (sigaction(*s_ptr,&signal_action,NULL))
      {
        throw_error(SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR);
      }
    }
  } while(++s_ptr < s_ptr_end);

  return 0;
}/*}}}*/

// === methods of generated structures =========================================

// -- epoll_callback_s --
@begin
methods epoll_callback_s
@end

// -- epoll_fd_event_s --
@begin
methods epoll_fd_event_s
@end

// -- epoll_fd_events_s --
@begin
methods epoll_fd_events_s
@end

// -- epoll_time_map_s --
@begin
methods epoll_time_map_s
@end

// -- epoll_time_events_s --
@begin
methods epoll_time_events_s
@end

// -- epoll_s --
@begin
methods epoll_s
@end

int epoll_s_fd_update(epoll_s *this,
    epoll_fd_s *a_epoll_fd,unsigned a_evts,int a_update_cb,const epoll_callback_s *a_callback)
{/*{{{*/
  epoll_fd_events_s *fd_events = &this->fd_events;

  // - resize fd events if needed -
  if (fd_events->used <= a_epoll_fd->fd)
  {
    unsigned old_used = fd_events->used;
    epoll_fd_events_s_push_blanks(fd_events,a_epoll_fd->fd + 1 - fd_events->used);
    memset(fd_events->data + old_used,0,(fd_events->used - old_used)*sizeof(epoll_fd_event_s));
  }

  epoll_fd_event_s *fd_event = fd_events->data + a_epoll_fd->fd;

  // - update callback if requested -
  if (a_update_cb)
  {
    fd_event->callback = *a_callback;
  }

  unsigned evts = fd_event->evts;

  // - adjust epoll fd events -
  if (evts != a_evts)
  {
    struct epoll_event epoll_event = {
      .events = a_evts,
      .data.fd = a_epoll_fd->fd
    };

    if (evts == 0)
    {
      // - ERROR -
      if (epoll_ctl(this->fd,EPOLL_CTL_ADD,a_epoll_fd->fd,&epoll_event) == -1)
      {
        throw_error(EPOLL_CONTROL_ADD_ERROR);
      }

      a_epoll_fd->epoll = this;
    }
    else if (a_evts == 0)
    {
      // - ERROR -
      if (epoll_ctl(this->fd,EPOLL_CTL_DEL,a_epoll_fd->fd,&epoll_event) == -1)
      {
        throw_error(EPOLL_CONTROL_DELETE_ERROR);
      }

      a_epoll_fd->epoll = this;
    }
    else
    {
      // - ERROR -
      if (epoll_ctl(this->fd,EPOLL_CTL_MOD,a_epoll_fd->fd,&epoll_event) == -1)
      {
        throw_error(EPOLL_CONTROL_MODIFY_ERROR);
      }
    }

    // - update fd events -
    fd_events->data[a_epoll_fd->fd].evts = a_evts;
  }

  return 0;
}/*}}}*/

int epoll_s_wait(epoll_s *this,int a_max_events,int a_timeout)
{/*{{{*/
  epoll_time_events_s *time_events = &this->time_events;

  // - adjust timeout -
  if (this->time_events.root_idx != c_idx_not_exist)
  {
    unsigned time_first_idx = epoll_time_events_s_get_min_value_idx(time_events,time_events->root_idx);
    ulli time_first = time_events->data[time_first_idx].object.time;

    ulli time_now;
    if (clock_s_gettime(CLOCK_MONOTONIC,&time_now))
    {
      throw_error(EPOLL_GET_TIME_ERROR);
    }

    if (time_first <= time_now)
    {
      a_timeout = 0;
    }
    else
    {
      int timeout = (time_first - time_now)/1000000;

      if (a_timeout == -1 || timeout < a_timeout)
      {
        a_timeout = timeout;
      }
    }
  }

  // - call epoll_wait -
  struct epoll_event stack_events[32];
  struct epoll_event *events = a_max_events <= 32 ? stack_events :
    (struct epoll_event *)cmalloc(a_max_events*sizeof(struct epoll_event));

  int count = epoll_wait(this->fd,events,a_max_events,a_timeout);

  // - ERROR -
  if (count == -1)
  {
    if (events != stack_events)
    {
      cfree(events);
    }

    // - interrupted by signal -
    if (errno == EINTR)
    {
      return ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED;
    }

    throw_error(EPOLL_WAIT_ERROR);
  }

  // - process fd events -
  if (count > 0)
  {
    struct epoll_event *e_ptr = events;
    struct epoll_event *e_ptr_end = e_ptr + count;
    do {
      unsigned fd = e_ptr->data.fd;

      // - check event callback -
      epoll_callback_s *callback = &this->fd_events.data[fd].callback;
      if (callback->function != NULL)
      {
        // - call event callback -
        ((epoll_fd_callback_t)callback->function)(callback->object,callback->index,e_ptr,this);
      }

    } while(++e_ptr < e_ptr_end);
  }

  if (events != stack_events)
  {
    cfree(events);
  }

  // - process time events -
  if (time_events->root_idx != c_idx_not_exist)
  {
    ulli time_now;
    if (clock_s_gettime(CLOCK_MONOTONIC,&time_now))
    {
      throw_error(EPOLL_GET_TIME_ERROR);
    }

    unsigned time_idx = epoll_time_events_s_get_min_value_idx(time_events,time_events->root_idx);
    do {
      epoll_time_map_s *time_map = &time_events->data[time_idx].object;

      // - if time is greater than actual time -
      if (time_map->time > time_now)
      {
        break;
      }

      unsigned next_idx = epoll_time_events_s_get_next_idx(time_events,time_idx);

      // - check event callback -
      epoll_callback_s *callback = &time_map->callback;
      if (callback->function != NULL)
      {
        // - call event callback -
        ((epoll_time_callback_t)callback->function)(callback->object,callback->index,time_idx,this);
      }

      epoll_time_events_s_remove(time_events,time_idx);

      if (time_map->period != 0)
      {
        epoll_time_map_s time_map_copy = *time_map;

        // - reschedule timer -
        time_map_copy.time += time_map_copy.period;
        epoll_time_events_s_insert(time_events,&time_map_copy);
      }

      time_idx = next_idx;
    } while(time_idx != c_idx_not_exist);
  }

  return 0;
}/*}}}*/

