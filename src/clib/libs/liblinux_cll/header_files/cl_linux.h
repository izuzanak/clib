
#ifndef CL_LIB_LINUX_H
#define CL_LIB_LINUX_H

@begin
include "cl_time.h"
include "cl_sys.h"
@end

#include <aio.h>
#include <errno.h>
#include <fcntl.h>

#if SUBSYSTEM_TYPE == SUBSYSTEM_TYPE_MSYS2
#include <cygwin/fs.h>
#else
#include <linux/fs.h>
#include <linux/rtc.h>
#endif

#include <net/if.h>
#include <netdb.h>
#include <signal.h>

#ifndef DISABLE_EPOLL
#include <sys/epoll.h>
#else
#define EPOLLIN  POLLIN
#define EPOLLOUT POLLOUT
#define EPOLLPRI POLLPRI
#define EPOLL_CLOEXEC 0
#endif

#include <dirent.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <sys/uio.h>
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

#define ERROR_DIR_OPEN_ERROR 1
#define ERROR_DIR_READ_ERROR 2

#define ERROR_MMAP_CREATE_ERROR 1

#define ERROR_AIO_WRITE_ERROR 1
#define ERROR_AIO_READ_ERROR 2
#define ERROR_AIO_RETURN_ERROR 3
#define ERROR_AIO_ERROR_ERROR 4

#define ERROR_PID_MISSING_PROGRAM_NAME 1
#define ERROR_PID_CANNOT_CREATE_NEW_PROCESS 2
#define ERROR_PID_KILL_ERROR 3

#define ERROR_PTHREAD_CREATE_ERROR 1
#define ERROR_PTHREAD_NOT_JOINABLE_ERROR 2
#define ERROR_PTHREAD_JOIN_ERROR 3
#define ERROR_PTHREAD_DETACH_ERROR 4

#define ERROR_SIGNAL_SIMPLE_HANDLER_INSTALL_ERROR 1

#define ERROR_EPOLL_CREATE_ERROR 1
#define ERROR_EPOLL_CONTROL_ADD_ERROR 2
#define ERROR_EPOLL_CONTROL_MODIFY_ERROR 3
#define ERROR_EPOLL_CONTROL_DELETE_ERROR 4
#define ERROR_EPOLL_WAIT_SIGNAL_INTERRUPTED 5
#define ERROR_EPOLL_WAIT_ERROR 6
#define ERROR_EPOLL_GET_TIME_ERROR 7
#define ERROR_EPOLL_TIMER_CREATE_ERROR 8
#define ERROR_EPOLL_TIMER_SETTIME_ERROR 9
#define ERROR_EPOLL_TIMER_GETTIME_ERROR 10
#define ERROR_EPOLL_FD_CALLBACK_ERROR 11

#define ERROR_RTC_DEVICE_OPEN_ERROR 1
#define ERROR_RTC_READ_TIME_ERROR 2
#define ERROR_RTC_WRITE_TIME_ERROR 3

// === definition of structure fd_s ============================================

typedef int fd_s;
@begin
define fd_s dynamic
@end

static inline void fd_s_init(fd_s *this);
static inline void fd_s_clear(fd_s *this);
static inline void fd_s_flush_all(const fd_s *this);
static inline void fd_s_swap(fd_s *this,fd_s *a_second);
static inline void fd_s_copy(const fd_s *this,const fd_s *a_src);
static inline int fd_s_compare(const fd_s *this,const fd_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void fd_s_to_string(const fd_s *this,bc_array_s *a_trg);
#endif

WUR liblinux_cll_EXPORT int fd_s_write(const fd_s *this,const void *a_src,size_t a_size);
WUR liblinux_cll_EXPORT int fd_s_writev(const fd_s *this,struct iovec *a_iov,int a_iovcnt,size_t a_size);
WUR liblinux_cll_EXPORT int fd_s_read(const fd_s *this,bc_array_s *a_trg);
WUR liblinux_cll_EXPORT int fd_s_read_cnt(const fd_s *this,size_t a_count,bc_array_s *a_trg);
WUR liblinux_cll_EXPORT int fd_s_read_max(const fd_s *this,size_t a_count,bc_array_s *a_trg);

// === definition of structure dir_s ===========================================

typedef DIR * dir_s;
@begin
define dir_s dynamic
@end

static inline void dir_s_init(dir_s *this);
static inline void dir_s_clear(dir_s *this);
static inline void dir_s_flush_all(dir_s *this);
static inline void dir_s_swap(dir_s *this,dir_s *a_second);
static inline void dir_s_copy(dir_s *this,const dir_s *a_src);
static inline int dir_s_compare(const dir_s *this,const dir_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void dir_s_to_string(const dir_s *this,bc_array_s *a_trg);
#endif

WUR static inline int dir_s_open(dir_s *this,const char *a_pathname);
WUR static inline int dir_s_read(dir_s *this,struct dirent **a_entry);

// === definition of structure mmap_s ==========================================

typedef struct mmap_s
{
  void *address;
  size_t length;
}
mmap_s;

@begin
define mmap_s dynamic
@end

static inline void mmap_s_init(mmap_s *this);
static inline void mmap_s_clear(mmap_s *this);
static inline void mmap_s_flush_all(mmap_s *this);
static inline void mmap_s_swap(mmap_s *this,mmap_s *a_second);
static inline void mmap_s_copy(const mmap_s *this,const mmap_s *a_src);
static inline int mmap_s_compare(const mmap_s *this,const mmap_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void mmap_s_to_string(const mmap_s *this,bc_array_s *a_trg);
#endif

WUR static inline int mmap_s_create(mmap_s *this,
  void *a_addr,size_t a_length,int a_prot,int a_flags,int a_fd,off_t a_offset);

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
static inline void socket_s_flush_all(const socket_s *this);
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

#if SUBSYSTEM_TYPE == SUBSYSTEM_TYPE_LINUX
// === definition of structure aio_s ===========================================

typedef struct aiocb aio_s;
@begin
define aio_s dynamic
@end

static inline void aio_s_init(aio_s *this);
static inline void aio_s_clear(aio_s *this);
static inline void aio_s_flush_all(aio_s *this);
static inline void aio_s_swap(aio_s *this,aio_s *a_second);
static inline void aio_s_copy(const aio_s *this,const aio_s *a_src);
static inline int aio_s_compare(const aio_s *this,const aio_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void aio_s_to_string(const aio_s *this,bc_array_s *a_trg);
#endif

WUR static inline int aio_s_write(aio_s *this,int a_fd,off_t a_offset,const void *a_src,size_t a_size);
WUR static inline int aio_s_read(aio_s *this,int a_fd,off_t a_offset,void *a_src,size_t a_size);
WUR static inline int aio_s_return(aio_s *this,ssize_t *a_return);
WUR static inline int aio_s_is_done(aio_s *this,int *a_done);
#endif

// === definition of structure pid_s ===========================================

typedef pid_t pid_s;
@begin
define pid_s dynamic
@end

static inline void pid_s_init(pid_s *this);
static inline void pid_s_clear(pid_s *this);
static inline void pid_s_flush_all(const pid_s *this);
static inline void pid_s_swap(pid_s *this,pid_s *a_second);
static inline void pid_s_copy(const pid_s *this,const pid_s *a_src);
static inline int pid_s_compare(const pid_s *this,const pid_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void pid_s_to_string(const pid_s *this,bc_array_s *a_trg);
#endif

WUR liblinux_cll_EXPORT int pid_s_execute(pid_s *this,const string_array_s *a_arguments);
WUR static inline int pid_s_kill(const pid_s *this,int a_signal);

// === definition of structure pthread_s =======================================

typedef struct pthread_s
{
  pthread_t *thread_ptr;
  int joinable;
}
pthread_s;

@begin
define pthread_s dynamic
@end

static inline void pthread_s_init(pthread_s *this);
static inline void pthread_s_clear(pthread_s *this);
static inline void pthread_s_flush_all(pthread_s *this);
static inline void pthread_s_swap(pthread_s *this,pthread_s *a_second);
static inline void pthread_s_copy(const pthread_s *this,const pthread_s *a_src);
static inline int pthread_s_compare(const pthread_s *this,const pthread_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void pthread_s_to_string(const pthread_s *this,bc_array_s *a_trg);
#endif

static inline int pthread_s_create(pthread_s *this,
    const pthread_attr_t *a_attr,void *(*a_function)(void *),void *a_arg);
static inline int pthread_s_join(pthread_s *this,void **a_retval);
static inline int pthread_s_detach(pthread_s *this);
static inline void pthread_s_exit(void *a_retval);

// === definition of structure signal_s ========================================

typedef void (*signal_callback_t)(int);
WUR liblinux_cll_EXPORT int signal_s_simple_handler(signal_callback_t a_handler);

// === definition of generated structures ======================================

typedef struct epoll_s epoll_s;

#ifndef DISABLE_EPOLL
typedef struct epoll_event epoll_event_s;
#else
typedef union epoll_data
{/*{{{*/
  void *ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
}/*}}}*/
epoll_data_t;

typedef struct epoll_event_s
{/*{{{*/
  uint32_t events;
  epoll_data_t data;
}/*}}}*/
epoll_event_s;
#endif

typedef int (*epoll_fd_callback_t)(void *a_object,unsigned a_index,epoll_event_s *a_event);

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

// -- epoll_s --
typedef struct epoll_fd_s epoll_fd_s;
typedef epoll_fd_s epoll_timer_s;

@begin
struct
<
#if !DISABLE_EPOLL
fd_s:fd
#else
pollfd_array_s:pollfd_array
#endif
epoll_fd_events_s:fd_events
>
epoll_s;
@end

static inline int epoll_s_create(epoll_s *this,int a_flags);
WUR liblinux_cll_EXPORT int epoll_s_fd_update(epoll_s *this,
    int a_fd,unsigned a_evts,int a_update_cb,const epoll_callback_s *a_callback);
WUR liblinux_cll_EXPORT int epoll_s_wait(epoll_s *this,int a_timeout);

typedef int (*epoll_fd_update_t)(int a_fd,unsigned a_evts,int a_update_cb,const epoll_callback_s *a_callback);
extern epoll_fd_update_t g_epoll_fd_update;

WUR static inline int epoll_s_fd_callback(
    epoll_fd_s *a_epoll_fd,unsigned a_evts,epoll_fd_callback_t a_callback,void *a_object,unsigned a_index);
WUR static inline int epoll_s_timer_callback(
    epoll_timer_s *a_epoll_timer,struct itimerspec *a_itimerspec,int a_flags,
    epoll_fd_callback_t a_callback,void *a_object,unsigned a_index);

// === definition of structure epoll_fd_s ======================================

typedef struct epoll_fd_s
{
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

// === definition of structure epoll_borrow_fd_s ===============================

typedef epoll_fd_s epoll_borrow_fd_s;
@begin
define epoll_borrow_fd_s dynamic
@end

static inline void epoll_borrow_fd_s_init(epoll_borrow_fd_s *this);
static inline void epoll_borrow_fd_s_clear(epoll_borrow_fd_s *this);
static inline void epoll_borrow_fd_s_flush_all(epoll_borrow_fd_s *this);
static inline void epoll_borrow_fd_s_swap(epoll_borrow_fd_s *this,epoll_borrow_fd_s *a_second);
static inline void epoll_borrow_fd_s_copy(const epoll_borrow_fd_s *this,const epoll_borrow_fd_s *a_src);
static inline int epoll_borrow_fd_s_compare(const epoll_borrow_fd_s *this,const epoll_borrow_fd_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void epoll_borrow_fd_s_to_string(const epoll_borrow_fd_s *this,bc_array_s *a_trg);
#endif

// === definition of structure epoll_timer_s ===================================

typedef epoll_fd_s epoll_timer_s;
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

WUR static inline int epoll_timer_s_settime(const epoll_timer_s *this,
    struct itimerspec *a_itimerspec,int a_flags);
WUR static inline int epoll_timer_s_gettime(const epoll_timer_s *this,
    struct itimerspec *a_itimerspec);

#if SUBSYSTEM_TYPE == SUBSYSTEM_TYPE_LINUX
// === definition of structure rtc_s ===========================================

typedef int rtc_s;
@begin
define rtc_s dynamic
@end

static inline void rtc_s_init(rtc_s *this);
static inline void rtc_s_clear(rtc_s *this);
static inline void rtc_s_flush_all(const rtc_s *this);
static inline void rtc_s_swap(rtc_s *this,rtc_s *a_second);
static inline void rtc_s_copy(const rtc_s *this,const rtc_s *a_src);
static inline int rtc_s_compare(const rtc_s *this,const rtc_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void rtc_s_to_string(const rtc_s *this,bc_array_s *a_trg);
#endif

WUR static inline int rtc_s_open(rtc_s *this,const char *a_path);
WUR static inline int rtc_s_read_time(const rtc_s *this,time_s *a_trg);
WUR static inline int rtc_s_write_time(const rtc_s *this,time_s a_time);
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

static inline void fd_s_flush_all(const fd_s *this)
{/*{{{*/
  (void)this;
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

// === inline methods of structure dir_s ======================================

static inline void dir_s_init(dir_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void dir_s_clear(dir_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    closedir(*this);
  }

  dir_s_init(this);
}/*}}}*/

static inline void dir_s_flush_all(dir_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

static inline void dir_s_swap(dir_s *this,dir_s *a_second)
{/*{{{*/
  dir_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void dir_s_copy(dir_s *this,const dir_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int dir_s_compare(const dir_s *this,const dir_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void dir_s_to_string(const dir_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"dir_s{%p}",*this);
}/*}}}*/
#endif

static inline int dir_s_open(dir_s *this,const char *a_pathname)
{/*{{{*/
  dir_s_clear(this);

  *this = opendir(a_pathname);

  // - ERROR -
  if (*this == NULL)
  {
    throw_error(DIR_OPEN_ERROR);
  }

  return 0;
}/*}}}*/

static inline int dir_s_read(dir_s *this,struct dirent **a_entry)
{/*{{{*/
  errno = 0;
  *a_entry = readdir(*this);

  // - ERROR -
  if (*a_entry == NULL && errno != 0)
  {
    throw_error(DIR_READ_ERROR);
  }

  return 0;
}/*}}}*/

// === inline methods of structure mmap_s ======================================

static inline void mmap_s_init(mmap_s *this)
{/*{{{*/
  this->address = NULL;
  this->length = 0;
}/*}}}*/

static inline void mmap_s_clear(mmap_s *this)
{/*{{{*/
  if (this->address != NULL)
  {
    munmap(this->address,this->length);
  }

  mmap_s_init(this);
}/*}}}*/

static inline void mmap_s_flush_all(mmap_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

static inline void mmap_s_swap(mmap_s *this,mmap_s *a_second)
{/*{{{*/
  mmap_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void mmap_s_copy(const mmap_s *this,const mmap_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int mmap_s_compare(const mmap_s *this,const mmap_s *a_second)
{/*{{{*/
  return this->address == a_second->address && this->length == a_second->length;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void mmap_s_to_string(const mmap_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"mmap_s{%p,%zu}",this->address,this->length);
}/*}}}*/
#endif

static inline int mmap_s_create(mmap_s *this,
  void *a_addr,size_t a_length,int a_prot,int a_flags,int a_fd,off_t a_offset)
{/*{{{*/
  mmap_s_clear(this);

  void *address = mmap(a_addr,a_length,a_prot,a_flags,a_fd,a_offset);

  // - ERROR -
  if (address == MAP_FAILED) // NOLINT(performance-no-int-to-ptr)
  {
    throw_error(MMAP_CREATE_ERROR);
  }

  this->address = address;
  this->length = a_length;

  return 0;
}/*}}}*/

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

static inline void socket_s_flush_all(const socket_s *this)
{/*{{{*/
  (void)this;
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

#if SUBSYSTEM_TYPE == SUBSYSTEM_TYPE_LINUX
// === inline methods of structure aio_s =======================================

static inline void aio_s_init(aio_s *this)
{/*{{{*/
  this->aio_fildes = -1;
}/*}}}*/

static inline void aio_s_clear(aio_s *this)
{/*{{{*/
  if (this->aio_fildes != -1)
  {
    aio_cancel(this->aio_fildes,this);
  }

  aio_s_init(this);
}/*}}}*/

static inline void aio_s_flush_all(aio_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

static inline void aio_s_swap(aio_s *this,aio_s *a_second)
{/*{{{*/
  aio_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void aio_s_copy(const aio_s *this,const aio_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int aio_s_compare(const aio_s *this,const aio_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void aio_s_to_string(const aio_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"aio_s{%d}",this->aio_fildes);
}/*}}}*/
#endif

static inline int aio_s_write(aio_s *this,int a_fd,off_t a_offset,const void *a_src,size_t a_size)
{/*{{{*/
  aio_s_clear(this);

  this->aio_fildes = a_fd;
  this->aio_offset = a_offset;
  this->aio_buf = (void *)a_src;
  this->aio_nbytes = a_size;
  this->aio_reqprio = 0;
  this->aio_sigevent.sigev_notify = SIGEV_NONE;
  this->aio_lio_opcode = 0;

  // - ERROR -
  if (aio_write(this))
  {
    aio_s_init(this);

    throw_error(AIO_WRITE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int aio_s_read(aio_s *this,int a_fd,off_t a_offset,void *a_src,size_t a_size)
{/*{{{*/
  aio_s_clear(this);

  this->aio_fildes = a_fd;
  this->aio_offset = a_offset;
  this->aio_buf = a_src;
  this->aio_nbytes = a_size;
  this->aio_reqprio = 0;
  this->aio_sigevent.sigev_notify = SIGEV_NONE;
  this->aio_lio_opcode = 0;

  // - ERROR -
  if (aio_read(this))
  {
    aio_s_init(this);

    throw_error(AIO_READ_ERROR);
  }

  return 0;
}/*}}}*/

static inline int aio_s_return(aio_s *this,ssize_t *a_return)
{/*{{{*/
  if ((*a_return = aio_return(this)) == - 1)
  {
    throw_error(AIO_RETURN_ERROR);
  }

  aio_s_init(this);

  return 0;
}/*}}}*/

static inline int aio_s_is_done(aio_s *this,int *a_done)
{/*{{{*/
  int err = aio_error(this);

  switch (err)
  {
    case 0:
      *a_done = 1;
      break;
    case EINPROGRESS:
      *a_done = 0;
      break;

    // - ERROR -
    default:
      throw_error(AIO_ERROR_ERROR);
  }

  return 0;
}/*}}}*/
#endif

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

static inline void pid_s_flush_all(const pid_s *this)
{/*{{{*/
  (void)this;
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

// === inline methods of structure pthread_s ===================================

static inline void pthread_s_init(pthread_s *this)
{/*{{{*/
  this->thread_ptr = NULL;
  this->joinable = 0;
}/*}}}*/

static inline void pthread_s_clear(pthread_s *this)
{/*{{{*/
  if (this->thread_ptr != NULL)
  {
    if (this->joinable)
    {
      cassert(pthread_join(*this->thread_ptr,NULL) == 0);
    }

    cfree(this->thread_ptr);
  }

  pthread_s_init(this);
}/*}}}*/

static inline void pthread_s_flush_all(pthread_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

static inline void pthread_s_swap(pthread_s *this,pthread_s *a_second)
{/*{{{*/
  pthread_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void pthread_s_copy(const pthread_s *this,const pthread_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int pthread_s_compare(const pthread_s *this,const pthread_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void pthread_s_to_string(const pthread_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"pthread_s{%p,%d}",this->thread_ptr,this->joinable);
}/*}}}*/
#endif

static inline int pthread_s_create(pthread_s *this,
    const pthread_attr_t *a_attr,void *(*a_function)(void *),void *a_arg)
{/*{{{*/
  pthread_s_clear(this);

  // - allocate pthread memory -
  this->thread_ptr = cmalloc(sizeof(pthread_t));

  if (pthread_create(this->thread_ptr,a_attr,a_function,a_arg))
  {
    // - release pthread memory -
    cfree(this->thread_ptr);
    pthread_s_init(this);

    throw_error(PTHREAD_CREATE_ERROR);
  }

  // - set joinable flag -
  this->joinable = 1;

  return 0;
}/*}}}*/

static inline int pthread_s_join(pthread_s *this,void **a_retval)
{/*{{{*/
  if (!this->joinable)
  {
    throw_error(PTHREAD_NOT_JOINABLE_ERROR);
  }

  if (pthread_join(*this->thread_ptr,a_retval))
  {
    throw_error(PTHREAD_JOIN_ERROR);
  }

  // - reset joinable flag -
  this->joinable = 0;

  pthread_s_clear(this);

  return 0;
}/*}}}*/

static inline int pthread_s_detach(pthread_s *this)
{/*{{{*/
  if (!this->joinable)
  {
    throw_error(PTHREAD_NOT_JOINABLE_ERROR);
  }

  if (pthread_detach(*this->thread_ptr))
  {
    throw_error(PTHREAD_DETACH_ERROR);
  }

  // - reset joinable flag -
  this->joinable = 0;

  pthread_s_clear(this);

  return 0;
}/*}}}*/

static inline void pthread_s_exit(void *a_retval)
{/*{{{*/
  pthread_exit(a_retval);
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

// -- epoll_s --
@begin
inlines epoll_s
@end

static inline int epoll_s_create(epoll_s *this,int a_flags)
{/*{{{*/
  epoll_s_clear(this);

#ifndef DISABLE_EPOLL
  this->fd = epoll_create1(a_flags);

  // - ERROR -
  if (this->fd == -1)
  {
    throw_error(EPOLL_CREATE_ERROR);
  }
#else
  (void)a_flags;
#endif

  return 0;
}/*}}}*/

static inline int epoll_s_fd_callback(
    epoll_fd_s *a_epoll_fd,unsigned a_evts,epoll_fd_callback_t a_callback,void *a_object,unsigned a_index)
{/*{{{*/
  epoll_callback_s callback = {a_callback,a_object,a_index};
  return g_epoll_fd_update(a_epoll_fd->fd,a_evts,1,&callback);
}/*}}}*/

static inline int epoll_s_timer_callback(
    epoll_timer_s *a_epoll_timer,struct itimerspec *a_itimerspec,int a_flags,
    epoll_fd_callback_t a_callback,void *a_object,unsigned a_index)
{/*{{{*/

  // - create timer if necessary -
  if (a_epoll_timer->fd == -1)
  {
    // - create timer -
    a_epoll_timer->fd = timerfd_create(CLOCK_MONOTONIC,0);

    if (a_epoll_timer->fd == -1 ||
        epoll_s_fd_callback(a_epoll_timer,EPOLLIN,a_callback,a_object,a_index))
    {
      throw_error(EPOLL_TIMER_CREATE_ERROR);
    }
  }

  // - set timer -
  if (timerfd_settime(a_epoll_timer->fd,a_flags,a_itimerspec,NULL))
  {
    throw_error(EPOLL_TIMER_SETTIME_ERROR);
  }

  return 0;
}/*}}}*/

// === inline methods of structure epoll_fd_s ==================================

static inline void epoll_fd_s_init(epoll_fd_s *this)
{/*{{{*/
  fd_s_init(&this->fd);
}/*}}}*/

static inline void epoll_fd_s_clear(epoll_fd_s *this)
{/*{{{*/
  if (this->fd != -1)
  {
    epoll_callback_s callback = {NULL,NULL,0};
    cassert(g_epoll_fd_update(this->fd,0,1,&callback) == 0);

    fd_s_clear(&this->fd);
  }
}/*}}}*/

static inline void epoll_fd_s_flush_all(epoll_fd_s *this)
{/*{{{*/
  (void)this;
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
  return fd_s_compare(&this->fd,&a_second->fd);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void epoll_fd_s_to_string(const epoll_fd_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"epoll_fd_s{");
  fd_s_to_string(&this->fd,a_trg);
  bc_array_s_append_format(a_trg,"}");
}/*}}}*/
#endif

static inline int epoll_fd_s_modify_events(epoll_fd_s *this,unsigned a_evts)
{/*{{{*/
  epoll_callback_s callback = {NULL,NULL,0};
  return g_epoll_fd_update(this->fd,a_evts,0,&callback);
}/*}}}*/

// === inline methods of structure epoll_borrow_fd_s ===========================

static inline void epoll_borrow_fd_s_init(epoll_borrow_fd_s *this)
{/*{{{*/
  this->fd = -1;
}/*}}}*/

static inline void epoll_borrow_fd_s_clear(epoll_borrow_fd_s *this)
{/*{{{*/
  if (this->fd != -1)
  {
    epoll_callback_s callback = {NULL,NULL,0};
    cassert(g_epoll_fd_update(this->fd,0,1,&callback) == 0);

    epoll_borrow_fd_s_init(this);
  }
}/*}}}*/

static inline void epoll_borrow_fd_s_flush_all(epoll_borrow_fd_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

static inline void epoll_borrow_fd_s_swap(epoll_borrow_fd_s *this,epoll_borrow_fd_s *a_second)
{/*{{{*/
  epoll_borrow_fd_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void epoll_borrow_fd_s_copy(const epoll_borrow_fd_s *this,const epoll_borrow_fd_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int epoll_borrow_fd_s_compare(const epoll_borrow_fd_s *this,const epoll_borrow_fd_s *a_second)
{/*{{{*/
  return this->fd == a_second->fd;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void epoll_borrow_fd_s_to_string(const epoll_borrow_fd_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"epoll_borrow_fd_s{%d}",this->fd);
}/*}}}*/
#endif

// === inline methods of structure epoll_timer_s ===============================

static inline void epoll_timer_s_init(epoll_timer_s *this)
{/*{{{*/
  fd_s_init(&this->fd);
}/*}}}*/

static inline void epoll_timer_s_clear(epoll_timer_s *this)
{/*{{{*/
  if (this->fd != -1)
  {
    epoll_callback_s callback = {NULL,NULL,0};
    cassert(g_epoll_fd_update(this->fd,0,1,&callback) == 0);

    fd_s_clear(&this->fd);
  }
}/*}}}*/

static inline void epoll_timer_s_flush_all(epoll_timer_s *this)
{/*{{{*/
  (void)this;
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
  return fd_s_compare(&this->fd,&a_second->fd);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void epoll_timer_s_to_string(const epoll_timer_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"epoll_timer_s{");
  fd_s_to_string(&this->fd,a_trg);
  bc_array_s_append_format(a_trg,"}");
}/*}}}*/
#endif

static inline int epoll_timer_s_settime(const epoll_timer_s *this,
    struct itimerspec *a_itimerspec,int a_flags)
{/*{{{*/

  // - set timer -
  if (timerfd_settime(this->fd,a_flags,a_itimerspec,NULL))
  {
    throw_error(EPOLL_TIMER_SETTIME_ERROR);
  }

  return 0;
}/*}}}*/

static inline int epoll_timer_s_gettime(const epoll_timer_s *this,
    struct itimerspec *a_itimerspec)
{/*{{{*/

  // - get timer values -
  if (timerfd_gettime(this->fd,a_itimerspec))
  {
    throw_error(EPOLL_TIMER_GETTIME_ERROR);
  }

  return 0;
}/*}}}*/

#if SUBSYSTEM_TYPE == SUBSYSTEM_TYPE_LINUX
// === definition of structure rtc_s ===========================================

static inline void rtc_s_init(rtc_s *this)
{/*{{{*/
  *this = -1;
}/*}}}*/

static inline void rtc_s_clear(rtc_s *this)
{/*{{{*/
  if (*this != -1)
  {
    close(*this);
  }

  rtc_s_init(this);
}/*}}}*/

static inline void rtc_s_flush_all(const rtc_s *this)
{/*{{{*/
  (void)this;
}/*}}}*/

static inline void rtc_s_swap(rtc_s *this,rtc_s *a_second)
{/*{{{*/
  rtc_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void rtc_s_copy(const rtc_s *this,const rtc_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int rtc_s_compare(const rtc_s *this,const rtc_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void rtc_s_to_string(const rtc_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"rtc_s{%d}",*this);
}/*}}}*/
#endif

static inline int rtc_s_open(rtc_s *this,const char *a_path)
{/*{{{*/
  rtc_s_clear(this);

  // - open rtc device -
  *this = open(a_path,O_RDWR);

  // - ERROR -
  if (*this == -1)
  {
    throw_error(RTC_DEVICE_OPEN_ERROR);
  }

  return 0;
}/*}}}*/

static inline int rtc_s_read_time(const rtc_s *this,time_s *a_trg)
{/*{{{*/
  struct rtc_time rtc_time;

  // - ERROR -
  if (ioctl(*this,RTC_RD_TIME,&rtc_time))
  {
    throw_error(RTC_READ_TIME_ERROR);
  }

  datetime_s datetime =
  {
    rtc_time.tm_year <= 70 ? 1970 : 1900 + rtc_time.tm_year,
    rtc_time.tm_mon + 1,
    rtc_time.tm_mday,
    0,
    rtc_time.tm_hour,
    rtc_time.tm_min,
    rtc_time.tm_sec,
    0,
    0,
    0
  };

  // - ERROR -
  if (datetime_s_to_nanosec(&datetime,a_trg))
  {
    throw_error(RTC_READ_TIME_ERROR);
  }

  return 0;
}/*}}}*/

static inline int rtc_s_write_time(const rtc_s *this,time_s a_time)
{/*{{{*/
  datetime_s datetime;
  datetime_s_from_nanosec(&datetime,a_time);

  struct rtc_time rtc_time =
  {
    datetime.sec,
    datetime.min,
    datetime.hour,
    datetime.day,
    datetime.month - 1,
    datetime.year - 1900,
    0,
    0,
    0
  };

  // - ERROR -
  if (ioctl(*this,RTC_SET_TIME,&rtc_time))
  {
    throw_error(RTC_WRITE_TIME_ERROR);
  }

  return 0;
}/*}}}*/
#endif

#endif

