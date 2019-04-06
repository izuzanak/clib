
#ifndef __CL_LINUX_H
#define __CL_LINUX_H

@begin
include "cl_time.h"
@end

#include <errno.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <netdb.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define liblinux_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef liblinux_cll_EXPORTS
#define liblinux_cll_EXPORT __declspec(dllexport)
#else
#define liblinux_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_LINUX_DUMMY_ERROR 1

#define ERROR_SOCKET_ADDRESS_RESOLVE_ERROR 1
#define ERROR_SOCKET_ADDRESS_NAME_ERROR 2

#define ERROR_SOCKET_CREATE_ERROR 1
#define ERROR_SOCKET_UNKNOWN_DOMAIN 2
#define ERROR_SOCKET_BIND_ERROR 3
#define ERROR_SOCKET_LISTEN_ERROR 4
#define ERROR_SOCKET_ACCEPT_ERROR 5
#define ERROR_SOCKET_CONNECT_ERROR 6
#define ERROR_SOCKET_SENDTO_ERROR 7
#define ERROR_SOCKET_RECVFROM_ERROR 8
#define ERROR_SOCKET_GETSOCKNAME_ERROR 9
#define ERROR_SOCKET_UDP_MAX_MSG_SIZE_EXCEEDED 10
#define ERROR_SOCKET_ADDRESS_UNKNOWN_FORMAT 11

#define ERROR_FD_WRITE_ERROR 1
#define ERROR_FD_READ_ERROR 2

#define ERROR_PID_MISSING_PROGRAM_NAME 1
#define ERROR_PID_CANNOT_CREATE_NEW_PROCESS 2
#define ERROR_PID_KILL_ERROR 3

#define ERROR_SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR 1

#define ERROR_EPOLL_CREATE_ERROR 1
#define ERROR_EPOLL_CONTROL_ADD_ERROR 2
#define ERROR_EPOLL_CONTROL_MODIFY_ERROR 3
#define ERROR_EPOLL_CONTROL_DELETE_ERROR 4
#define ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED 5
#define ERROR_EPOLL_WAIT_ERROR 6
#define ERROR_EPOLL_GET_TIME_ERROR 7
#define ERROR_EPOLL_FD_CALLBACK_ERROR 8
#define ERROR_EPOLL_TIMER_CALLBACK_ERROR 9

// === constants and definitions ===============================================

enum
{/*{{{*/
  EPOLL_FD_UNUSED  = -1,
  EPOLL_FD_REMOVED = -2,
};/*}}}*/

// === definition of structure fd_s ============================================

typedef int fd_s;
@begin
define fd_s dynamic
@end

static inline void fd_s_init(fd_s *this);
static inline void fd_s_clear(fd_s *this);
static inline void fd_s_flush_all(fd_s *this);
static inline void fd_s_swap(fd_s *this,fd_s *a_second);
static inline void fd_s_copy(const fd_s *this,const fd_s *a_src);
static inline int fd_s_compare(const fd_s *this,const fd_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void fd_s_to_string(const fd_s *this,bc_array_s *a_trg);
#endif

WUR liblinux_cll_EXPORT int fd_s_write(const fd_s *this,const void *a_src,size_t a_size);
WUR liblinux_cll_EXPORT int fd_s_read(const fd_s *this,bc_array_s *a_trg);
WUR liblinux_cll_EXPORT int fd_s_read_cnt(const fd_s *this,size_t a_count,bc_array_s *a_trg);

// === definition of structure socket_address_s ================================

typedef struct sockaddr_in socket_address_s;
@begin
define socket_address_s static
@end

static inline int socket_address_s_compare(const socket_address_s *this,const socket_address_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void socket_address_s_to_string(const socket_address_s *this,bc_array_s *a_trg);
#endif

WUR static inline int socket_address_s_create(socket_address_s *this,const char *a_address,unsigned short a_port);
WUR static inline int socket_address_s_name(socket_address_s *this,bc_array_s *a_trg);
static inline unsigned short socket_address_s_port(socket_address_s *this);

// === definition of structure socket_s ========================================

typedef int socket_s;
@begin
define socket_s dynamic
@end

static inline void socket_s_init(socket_s *this);
static inline void socket_s_clear(socket_s *this);
static inline void socket_s_flush_all(socket_s *this);
static inline void socket_s_swap(socket_s *this,socket_s *a_second);
static inline void socket_s_copy(const socket_s *this,const socket_s *a_src);
static inline int socket_s_compare(const socket_s *this,const socket_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void socket_s_to_string(const socket_s *this,bc_array_s *a_trg);
#endif

WUR static inline int socket_s_create(socket_s *this,int a_domain,int a_type);
WUR liblinux_cll_EXPORT int socket_s_bind(const socket_s *this,const socket_address_s *a_addr);
WUR liblinux_cll_EXPORT int socket_s_listen(const socket_s *this,const socket_address_s *a_addr,int a_backlog);
WUR liblinux_cll_EXPORT int socket_s_accept(const socket_s *this,socket_s *a_socket,socket_address_s *a_addr);
WUR liblinux_cll_EXPORT int socket_s_connect(const socket_s *this,const socket_address_s *a_addr);
WUR liblinux_cll_EXPORT int socket_s_sendto(const socket_s *this,const socket_address_s *a_addr,const void *a_src,size_t a_size);
WUR liblinux_cll_EXPORT int socket_s_recvfrom(const socket_s *this,bc_array_s *a_trg,socket_address_s *a_addr);
WUR static inline int socket_s_address(const socket_s *this,socket_address_s *a_addr);

// === definition of structure pid_s ===========================================

typedef pid_t pid_s;
@begin
define pid_s dynamic
@end

static inline void pid_s_init(pid_s *this);
static inline void pid_s_clear(pid_s *this);
static inline void pid_s_flush_all(pid_s *this);
static inline void pid_s_swap(pid_s *this,pid_s *a_second);
static inline void pid_s_copy(const pid_s *this,const pid_s *a_src);
static inline int pid_s_compare(const pid_s *this,const pid_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void pid_s_to_string(const pid_s *this,bc_array_s *a_trg);
#endif

WUR liblinux_cll_EXPORT int pid_s_execute(pid_s *this,const string_array_s *a_arguments);
WUR static inline int pid_s_kill(const pid_s *this,int a_signal);

// === definition of structure signal_s ========================================

typedef void (*signal_callback_t)(int);
WUR liblinux_cll_EXPORT int signal_s_simple_handler(signal_callback_t a_handler);

// === definition of generated structures ======================================

typedef struct epoll_s epoll_s;
typedef struct epoll_event epoll_event_s;
typedef int (*epoll_fd_callback_t)(void *a_object,unsigned a_index,epoll_event_s *a_event,epoll_s *a_epoll);
typedef int (*epoll_time_callback_t)(void *a_object,unsigned a_index,unsigned a_timer,epoll_s *a_epoll);

// -- epoll_callback_s --
@begin
struct
<
pointer:function
pointer:object
ui:index
>
epoll_callback_s;
@end

// -- epoll_fd_event_s --
@begin
struct
<
ui:evts
epoll_callback_s:callback
>
epoll_fd_event_s;
@end

// -- epoll_fd_events_s --
@begin
array<epoll_fd_event_s> epoll_fd_events_s;
@end

// -- epoll_time_map_s --
@begin
struct
<
ulli:time
ulli:period
epoll_callback_s:callback
>
epoll_time_map_s;
@end

// -- epoll_time_events_s --
@begin
safe_rb_tree<epoll_time_map_s> epoll_time_events_s;
@end

// -- epoll_s --
typedef struct epoll_fd_s epoll_fd_s;
typedef struct epoll_timer_s epoll_timer_s;

@begin
struct
<
fd_s:fd
epoll_fd_events_s:fd_events
epoll_time_events_s:time_events
>
epoll_s;
@end

static inline int epoll_s_create(epoll_s *this,int a_flags);
WUR static inline int epoll_s_fd(epoll_s *this,epoll_fd_s *a_epoll_fd,unsigned a_evts);
WUR static inline int epoll_s_fd_callback(epoll_s *this,
    epoll_fd_s *a_epoll_fd,unsigned a_evts,epoll_fd_callback_t a_callback,void *a_object,unsigned a_index);
WUR liblinux_cll_EXPORT int epoll_s_fd_update(epoll_s *this,
    epoll_fd_s *a_epoll_fd,unsigned a_evts,int a_update_cb,const epoll_callback_s *a_callback);
static inline void epoll_s_timer_stamp(epoll_s *this,
    ulli a_time,epoll_time_callback_t a_callback,void *a_object,unsigned a_index,epoll_timer_s *a_epoll_timer);
WUR static inline int epoll_s_timer_delay(epoll_s *this,
    ulli a_delay,epoll_time_callback_t a_callback,void *a_object,unsigned a_index,epoll_timer_s *a_epoll_timer);
WUR static inline int epoll_s_timer_period(epoll_s *this,
    ulli a_period,epoll_time_callback_t a_callback,void *a_object,unsigned a_index,epoll_timer_s *a_epoll_timer);
WUR static inline int epoll_s_timer_period_now(epoll_s *this,
    ulli a_period,epoll_time_callback_t a_callback,void *a_object,unsigned a_index,epoll_timer_s *a_epoll_timer);
static inline void epoll_s_timer_remove(epoll_s *this,epoll_timer_s *a_epoll_timer);
WUR liblinux_cll_EXPORT int epoll_s_wait(epoll_s *this,int a_max_events,int a_timeout);

// === definition of structure epoll_fd_s ======================================

typedef struct epoll_fd_s
{
  epoll_s *epoll;
  fd_s fd;
} epoll_fd_s;

@begin
define epoll_fd_s dynamic
@end

static inline void epoll_fd_s_init(epoll_fd_s *this);
static inline void epoll_fd_s_clear(epoll_fd_s *this);
static inline void epoll_fd_s_flush_all(epoll_fd_s *this);
static inline void epoll_fd_s_swap(epoll_fd_s *this,epoll_fd_s *a_second);
static inline void epoll_fd_s_copy(const epoll_fd_s *this,const epoll_fd_s *a_src);
static inline int epoll_fd_s_compare(const epoll_fd_s *this,const epoll_fd_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void epoll_fd_s_to_string(const epoll_fd_s *this,bc_array_s *a_trg);
#endif

WUR static inline int epoll_fd_s_modify_events(epoll_fd_s *this,unsigned a_evts);

// === definition of structure epoll_timer_s ===================================

typedef struct epoll_timer_s
{
  epoll_s *epoll;
  ui timer;
} epoll_timer_s;

@begin
define epoll_timer_s dynamic
@end

static inline void epoll_timer_s_init(epoll_timer_s *this);
static inline void epoll_timer_s_clear(epoll_timer_s *this);
static inline void epoll_timer_s_flush_all(epoll_timer_s *this);
static inline void epoll_timer_s_swap(epoll_timer_s *this,epoll_timer_s *a_second);
static inline void epoll_timer_s_copy(const epoll_timer_s *this,const epoll_timer_s *a_src);
static inline int epoll_timer_s_compare(const epoll_timer_s *this,const epoll_timer_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void epoll_timer_s_to_string(const epoll_timer_s *this,bc_array_s *a_trg);
#endif

// === inline methods of structure socket_address_s ============================

static inline int socket_address_s_compare(const socket_address_s *this,const socket_address_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void socket_address_s_to_string(const socket_address_s *this,bc_array_s *a_trg)
{/*{{{*/
  (void)this;

  bc_array_s_append_ptr(a_trg,"socket_address_s{}");
}/*}}}*/
#endif

static inline int socket_address_s_create(socket_address_s *this,const char *a_address,unsigned short a_port)
{/*{{{*/

  // - retrieve host by name address -
  struct hostent *host = gethostbyname(a_address);

  // - ERROR -
  if (host == NULL)
  {
    throw_error(SOCKET_ADDRESS_RESOLVE_ERROR);
  }

  memcpy(&this->sin_addr.s_addr,host->h_addr,host->h_length);
  this->sin_port = htons(a_port);
  this->sin_family = AF_INET;

  return 0;
}/*}}}*/

static inline int socket_address_s_name(socket_address_s *this,bc_array_s *a_trg)
{/*{{{*/
  a_trg->used = 0;
  bc_array_s_reserve(a_trg,256);

  // - ERROR -
  if (getnameinfo((struct sockaddr *)this,sizeof(struct sockaddr_in),
        a_trg->data,256,NULL,0,NI_NUMERICHOST | NI_NUMERICSERV) != 0)
  {
    throw_error(SOCKET_ADDRESS_NAME_ERROR);
  }

  a_trg->used = strlen(a_trg->data);

  return 0;
}/*}}}*/

static inline unsigned short socket_address_s_port(socket_address_s *this)
{/*{{{*/
  return ntohs(this->sin_port);
}/*}}}*/

// === inline methods of structure fd_s ========================================

static inline void fd_s_init(fd_s *this)
{/*{{{*/
  *this = -1;
}/*}}}*/

static inline void fd_s_clear(fd_s *this)
{/*{{{*/
  if (*this != -1)
  {
    close(*this);
  }

  fd_s_init(this);
}/*}}}*/

static inline void fd_s_flush_all(fd_s *this)
{/*{{{*/
}/*}}}*/

static inline void fd_s_swap(fd_s *this,fd_s *a_second)
{/*{{{*/
  fd_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void fd_s_copy(const fd_s *this,const fd_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int fd_s_compare(const fd_s *this,const fd_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void fd_s_to_string(const fd_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"fd_s{%d}",*this);
}/*}}}*/
#endif

// === inline methods of structure socket_s ====================================

static inline void socket_s_init(socket_s *this)
{/*{{{*/
  *this = -1;
}/*}}}*/

static inline void socket_s_clear(socket_s *this)
{/*{{{*/
  if (*this != -1)
  {
    close(*this);
  }

  socket_s_init(this);
}/*}}}*/

static inline void socket_s_flush_all(socket_s *this)
{/*{{{*/
}/*}}}*/

static inline void socket_s_swap(socket_s *this,socket_s *a_second)
{/*{{{*/
  socket_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void socket_s_copy(const socket_s *this,const socket_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int socket_s_compare(const socket_s *this,const socket_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void socket_s_to_string(const socket_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"socket_s{%d}",*this);
}/*}}}*/
#endif

static inline int socket_s_create(socket_s *this,int a_domain,int a_type)
{/*{{{*/
  socket_s_clear(this);

  // - create socket -
  *this = socket(a_domain,a_type,0);

  // - ERROR -
  if (*this == -1)
  {
    throw_error(SOCKET_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int socket_s_address(const socket_s *this,socket_address_s *a_addr)
{/*{{{*/
  debug_assert(*this != -1);

  socklen_t addr_len = sizeof(struct sockaddr_in);

  // - ERROR -
  if (getsockname(*this,(struct sockaddr *)a_addr,&addr_len))
  {
    throw_error(SOCKET_GETSOCKNAME_ERROR);
  }

  // - ERROR -
  if (addr_len != sizeof(struct sockaddr_in))
  {
    throw_error(SOCKET_ADDRESS_UNKNOWN_FORMAT);
  }

  return 0;
}/*}}}*/

// === inline methods of structure pid_s =======================================

static inline void pid_s_init(pid_s *this)
{/*{{{*/
  *this = -1;
}/*}}}*/

static inline void pid_s_clear(pid_s *this)
{/*{{{*/
  if (*this != -1)
  {
    // - check if process was terminated -
    int status;
    if (waitpid(*this,&status,WNOHANG) <= 0)
    {
      // - send terminate signal to process -
      if (kill(*this,SIGTERM) == 0)
      {
        // - wait on process to terminate -
        waitpid(*this,&status,0);
      }
    }
  }

  pid_s_init(this);
}/*}}}*/

static inline void pid_s_flush_all(pid_s *this)
{/*{{{*/
}/*}}}*/

static inline void pid_s_swap(pid_s *this,pid_s *a_second)
{/*{{{*/
  pid_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void pid_s_copy(const pid_s *this,const pid_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int pid_s_compare(const pid_s *this,const pid_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void pid_s_to_string(const pid_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"pid_s{%d}",*this);
}/*}}}*/
#endif

static inline int pid_s_kill(const pid_s *this,int a_signal)
{/*{{{*/
  debug_assert(*this != -1);

  if (kill(*this,a_signal))
  {
    throw_error(PID_KILL_ERROR);
  }

  return 0;
}/*}}}*/

// === inline methods of generated structures ==================================

// -- epoll_callback_s --
@begin
inlines epoll_callback_s
@end

// -- epoll_fd_event_s --
@begin
inlines epoll_fd_event_s
@end

// -- epoll_fd_events_s --
@begin
inlines epoll_fd_events_s
@end

// -- epoll_time_map_s --
@begin
inlines epoll_time_map_s
@end

// -- epoll_time_events_s --
@begin
inlines epoll_time_events_s
@end

static inline int epoll_time_events_s___compare_value(const epoll_time_events_s *this,const epoll_time_map_s *a_first,const epoll_time_map_s *a_second)
{/*{{{*/
  (void)this;

  ulli f_time = a_first->time;
  ulli s_time = a_second->time;

  return f_time < s_time ? -1 : f_time > s_time ? 1 : 0;
}/*}}}*/

// -- epoll_s --
@begin
inlines epoll_s
@end

static inline int epoll_s_create(epoll_s *this,int a_flags)
{/*{{{*/
  epoll_s_clear(this);

  this->fd = epoll_create1(a_flags);

  // - ERROR -
  if (this->fd == -1)
  {
    throw_error(EPOLL_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int epoll_s_fd(epoll_s *this,epoll_fd_s *a_epoll_fd,unsigned a_evts)
{/*{{{*/
  epoll_callback_s callback = {NULL,NULL,0};
  return epoll_s_fd_update(this,a_epoll_fd,a_evts,1,&callback);
}/*}}}*/

static inline int epoll_s_fd_callback(epoll_s *this,
    epoll_fd_s *a_epoll_fd,unsigned a_evts,epoll_fd_callback_t a_callback,void *a_object,unsigned a_index)
{/*{{{*/
  epoll_callback_s callback = {a_callback,a_object,a_index};
  return epoll_s_fd_update(this,a_epoll_fd,a_evts,1,&callback);
}/*}}}*/

static inline void epoll_s_timer_stamp(epoll_s *this,
    ulli a_time,epoll_time_callback_t a_callback,void *a_object,unsigned a_index,epoll_timer_s *a_epoll_timer)
{/*{{{*/
  epoll_time_map_s time_map = {a_time,0,{a_callback,a_object,a_index}};
  a_epoll_timer->timer = epoll_time_events_s_insert(&this->time_events,&time_map);
  a_epoll_timer->epoll = this;
}/*}}}*/

static inline int epoll_s_timer_delay(epoll_s *this,
    ulli a_delay,epoll_time_callback_t a_callback,void *a_object,unsigned a_index,epoll_timer_s *a_epoll_timer)
{/*{{{*/
  ulli time_now;
  if (clock_s_gettime(CLOCK_MONOTONIC,&time_now))
  {
    throw_error(EPOLL_GET_TIME_ERROR);
  }

  epoll_time_map_s time_map = {time_now + a_delay,0,{a_callback,a_object,a_index}};
  a_epoll_timer->timer = epoll_time_events_s_insert(&this->time_events,&time_map);
  a_epoll_timer->epoll = this;

  return 0;
}/*}}}*/

static inline int epoll_s_timer_period(epoll_s *this,
    ulli a_period,epoll_time_callback_t a_callback,void *a_object,unsigned a_index,epoll_timer_s *a_epoll_timer)
{/*{{{*/
  ulli time_now;
  if (clock_s_gettime(CLOCK_MONOTONIC,&time_now))
  {
    throw_error(EPOLL_GET_TIME_ERROR);
  }

  epoll_time_map_s time_map = {time_now + a_period,a_period,{a_callback,a_object,a_index}};
  a_epoll_timer->timer = epoll_time_events_s_insert(&this->time_events,&time_map);
  a_epoll_timer->epoll = this;

  return 0;
}/*}}}*/

static inline int epoll_s_timer_period_now(epoll_s *this,
    ulli a_period,epoll_time_callback_t a_callback,void *a_object,unsigned a_index,epoll_timer_s *a_epoll_timer)
{/*{{{*/
  ulli time_now;
  if (clock_s_gettime(CLOCK_MONOTONIC,&time_now))
  {
    throw_error(EPOLL_GET_TIME_ERROR);
  }

  epoll_time_map_s time_map = {time_now,a_period,{a_callback,a_object,a_index}};
  a_epoll_timer->timer = epoll_time_events_s_insert(&this->time_events,&time_map);
  a_epoll_timer->epoll = this;

  return 0;
}/*}}}*/

static inline void epoll_s_timer_remove(epoll_s *this,epoll_timer_s *a_epoll_timer)
{/*{{{*/
  epoll_time_events_s_remove(&this->time_events,a_epoll_timer->timer);
}/*}}}*/

// === inline methods of structure epoll_fd_s ===============================

static inline void epoll_fd_s_init(epoll_fd_s *this)
{/*{{{*/
  this->epoll = NULL;
  fd_s_init(&this->fd);
}/*}}}*/

static inline void epoll_fd_s_clear(epoll_fd_s *this)
{/*{{{*/
  if (this->epoll != NULL && this->fd != -1)
  {
    cassert(epoll_s_fd(this->epoll,this,0) == 0);
  }

  fd_s_clear(&this->fd);

  this->epoll = NULL;
}/*}}}*/

static inline void epoll_fd_s_flush_all(epoll_fd_s *this)
{/*{{{*/
}/*}}}*/

static inline void epoll_fd_s_swap(epoll_fd_s *this,epoll_fd_s *a_second)
{/*{{{*/
  epoll_fd_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void epoll_fd_s_copy(const epoll_fd_s *this,const epoll_fd_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int epoll_fd_s_compare(const epoll_fd_s *this,const epoll_fd_s *a_second)
{/*{{{*/
  return this->epoll == a_second->epoll && fd_s_compare(&this->fd,&a_second->fd);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void epoll_fd_s_to_string(const epoll_fd_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"epoll_fd_s{%p,",this->epoll);
  fd_s_to_string(&this->fd,a_trg);
  bc_array_s_append_format(a_trg,"}");
}/*}}}*/
#endif

static inline int epoll_fd_s_modify_events(epoll_fd_s *this,unsigned a_evts)
{/*{{{*/
  debug_assert(this->epoll != NULL);

  epoll_callback_s callback = {NULL,NULL,0};
  return epoll_s_fd_update(this->epoll,this,a_evts,0,&callback);
}/*}}}*/

// === inline methods of structure epoll_timer_s ===============================

static inline void epoll_timer_s_init(epoll_timer_s *this)
{/*{{{*/
  this->epoll = NULL;
  this->timer = c_idx_not_exist;
}/*}}}*/

static inline void epoll_timer_s_clear(epoll_timer_s *this)
{/*{{{*/
  if (this->epoll != NULL && this->timer != c_idx_not_exist)
  {
    epoll_s_timer_remove(this->epoll,this);
  }

  epoll_timer_s_init(this);
}/*}}}*/

static inline void epoll_timer_s_flush_all(epoll_timer_s *this)
{/*{{{*/
}/*}}}*/

static inline void epoll_timer_s_swap(epoll_timer_s *this,epoll_timer_s *a_second)
{/*{{{*/
  epoll_timer_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void epoll_timer_s_copy(const epoll_timer_s *this,const epoll_timer_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int epoll_timer_s_compare(const epoll_timer_s *this,const epoll_timer_s *a_second)
{/*{{{*/
  return this->epoll == a_second->epoll && this->timer == a_second->timer;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void epoll_timer_s_to_string(const epoll_timer_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"epoll_timer_s{%p,%u}",this->epoll,this->timer);
}/*}}}*/
#endif

#endif

