
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_process.h"
include "cl_channel.h"
include "sd_conf.h"
include "sd_channel.h"
include "sd_segfile.h"
include "sd_trace.h"
@end

// - error codes -
#define ERROR_SD_DAEMON_CONFIG_READ_ERROR 1
#define ERROR_SD_DAEMON_CONFIG_DATA_ERROR 2
#define ERROR_SD_DAEMON_EPOLL_WAIT_ERROR 3
#define ERROR_SD_DAEMON_INVALID_SEGMENT_TYPE 4

#define ERROR_SD_DAEMON_SEGMENT_FILE_CREATE_ERROR 5
#define ERROR_SD_DAEMON_SEGMENT_NOT_EXIST 6
#define ERROR_SD_DAEMON_SEGMENT_INVALID_WRITE_SIZE 7
#define ERROR_SD_DAEMON_SEGMENT_WRITE_ERROR 8

#define ERROR_SD_DAEMON_TRACE_FILES_MMAP_ERROR 9
#define ERROR_SD_DAEMON_TRACE_CREATE_ERROR 10
#define ERROR_SD_DAEMON_TRACE_NOT_EXIST 11
#define ERROR_SD_DAEMON_TRACE_INVALID_WRITE_SIZE 12
#define ERROR_SD_DAEMON_TRACE_WRITE_ERROR 13

#define ERROR_SD_DAEMON_CHANNEL_CREATE_ERROR 20
#define ERROR_SD_DAEMON_CHANNEL_SEND_MESSAGE_ERROR 21

#define ERROR_SD_TRACE_DESCR_FILE_OPEN_ERROR 1
#define ERROR_SD_TRACE_DESCR_MMAP_CREATE_ERROR 2

// === definition of structure sd_segment_handle_s =============================

typedef int (*sd_segment_clear_cb_t)(void *a_object);
typedef int (*sd_segment_write_record_cb_t)(void *a_object,time_s a_time,unsigned a_size,const char *a_data);
typedef int (*sd_segment_read_record_cb_t)(void *a_object,time_s *a_time,bc_array_s *a_trg);

typedef struct sd_segment_handle_s
{
  void *segment;
  sd_segment_clear_cb_t sd_segment_clear_cb;
  sd_segment_write_record_cb_t sd_segment_write_record_cb;
  sd_segment_read_record_cb_t sd_segment_read_record_cb;
}
sd_segment_handle_s;

@begin
define sd_segment_handle_s dynamic
@end

static inline void sd_segment_handle_s_init(sd_segment_handle_s *this);
static inline void sd_segment_handle_s_clear(sd_segment_handle_s *this);
static inline void sd_segment_handle_s_flush_all(sd_segment_handle_s *this);
static inline void sd_segment_handle_s_swap(sd_segment_handle_s *this,sd_segment_handle_s *a_second);
static inline void sd_segment_handle_s_copy(const sd_segment_handle_s *this,const sd_segment_handle_s *a_src);
static inline int sd_segment_handle_s_compare(const sd_segment_handle_s *this,const sd_segment_handle_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void sd_segment_handle_s_to_string(const sd_segment_handle_s *this,bc_array_s *a_trg);
#endif

static inline void sd_segment_handle_s_create(sd_segment_handle_s *this,
    void *a_segment,
    sd_segment_clear_cb_t a_sd_segment_clear_cb,
    sd_segment_write_record_cb_t a_sd_segment_write_record_cb,
    sd_segment_read_record_cb_t a_sd_segment_read_record_cb);
static inline int sd_segment_handle_s_write_record(sd_segment_handle_s *this,
    time_s a_time,unsigned a_size,const char *a_data);
static inline int sd_segment_handle_s_read_record(sd_segment_handle_s *this,
    time_s *a_time,bc_array_s *a_trg);

// === definition of generated structures ======================================

// -- sd_segment_descr_s --
@begin
struct
<
sd_conf_segment_s:config
sd_segment_handle_s:handle
>
sd_segment_descr_s;
@end

// -- sd_segment_tree_s --
@begin
safe_rb_tree<sd_segment_descr_s> sd_segment_tree_s;
@end

// -- sd_trace_descr_s --
@begin
struct
<
sd_conf_trace_s:config
sd_trace_mmap_s:mmap_header
sd_trace_mmap_s:mmap_trace
sd_trace_mmap_s:mmap_timestamp
sd_trace_s:trace
>
sd_trace_descr_s;
@end

WUR static inline int sd_trace_descr_s_mmap_file(sd_trace_mmap_s *a_trace_mmap,sd_conf_trace_data_s *a_trace_data);
void sd_trace_descr_s_read_to_message(sd_trace_descr_s *this,lli a_record_id,bc_array_s *a_trg);

// -- sd_trace_tree_s --
@begin
safe_rb_tree<sd_trace_descr_s> sd_trace_tree_s;
@end

// -- sd_daemon_s --
@begin
struct
<
bi:config_changed_flag
sd_config_s:config
sd_config_s:last_config

sd_segment_tree_s:segments
sd_trace_tree_s:traces

sd_channel_s:channel

bc_array_s:buffer
epoll_s:epoll
>
sd_daemon_s;
@end

WUR int sd_daemon_s_create(sd_daemon_s *this);

WUR int sd_daemon_s_process_config(sd_daemon_s *this);
WUR int sd_daemon_s_update_segments(sd_daemon_s *this);
WUR int sd_daemon_s_update_traces(sd_daemon_s *this);

WUR int sd_daemon_s_run(sd_daemon_s *this);

WUR int sd_daemon_s_channel_callback(void *a_sd_daemon,unsigned a_index,unsigned a_type,va_list a_ap);

// === inline methods of structure sd_segment_handle_s =========================

static inline void sd_segment_handle_s_init(sd_segment_handle_s *this)
{/*{{{*/
  this->segment = NULL;
  this->sd_segment_clear_cb = NULL;
  this->sd_segment_write_record_cb = NULL;
  this->sd_segment_read_record_cb = NULL;
}/*}}}*/

static inline void sd_segment_handle_s_clear(sd_segment_handle_s *this)
{/*{{{*/
  if (this->segment != NULL)
  {
    this->sd_segment_clear_cb(this->segment);
    cfree(this->segment);
  }

  sd_segment_handle_s_init(this);
}/*}}}*/

static inline void sd_segment_handle_s_flush_all(sd_segment_handle_s *this)
{/*{{{*/
}/*}}}*/

static inline void sd_segment_handle_s_swap(sd_segment_handle_s *this,sd_segment_handle_s *a_second)
{/*{{{*/
  sd_segment_handle_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void sd_segment_handle_s_copy(const sd_segment_handle_s *this,const sd_segment_handle_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int sd_segment_handle_s_compare(const sd_segment_handle_s *this,const sd_segment_handle_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void sd_segment_handle_s_to_string(const sd_segment_handle_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_ptr(a_trg,"sd_segment_handle_s{}");
}/*}}}*/
#endif

static inline void sd_segment_handle_s_create(sd_segment_handle_s *this,
    void *a_segment,
    sd_segment_clear_cb_t a_sd_segment_clear_cb,
    sd_segment_write_record_cb_t a_sd_segment_write_record_cb,
    sd_segment_read_record_cb_t a_sd_segment_read_record_cb)
{/*{{{*/
  sd_segment_handle_s_clear(this);

  this->segment = a_segment;
  this->sd_segment_clear_cb = a_sd_segment_clear_cb;
  this->sd_segment_write_record_cb = a_sd_segment_write_record_cb;
  this->sd_segment_read_record_cb = a_sd_segment_read_record_cb;
}/*}}}*/

static inline int sd_segment_handle_s_write_record(sd_segment_handle_s *this,
    time_s a_time,unsigned a_size,const char *a_data)
{/*{{{*/
  return this->sd_segment_write_record_cb(this->segment,a_time,a_size,a_data);
}/*}}}*/

static inline int sd_segment_handle_s_read_record(sd_segment_handle_s *this,
    time_s *a_time,bc_array_s *a_trg)
{/*{{{*/
  return this->sd_segment_read_record_cb(this->segment,a_time,a_trg);
}/*}}}*/

// === inline methods of generated structures ==================================

// -- sd_segment_descr_s --
@begin
inlines sd_segment_descr_s
@end

// -- sd_segment_tree_s --
@begin
inlines sd_segment_tree_s
@end

static inline int sd_segment_tree_s___compare_value(const sd_segment_tree_s *this,const sd_segment_descr_s *a_first,const sd_segment_descr_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->config.segment_id;
  const string_s *second = &a_second->config.segment_id;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }

  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- sd_trace_descr_s --
@begin
inlines sd_trace_descr_s
@end

static inline int sd_trace_descr_s_mmap_file(sd_trace_mmap_s *a_trace_mmap,sd_conf_trace_data_s *a_trace_data)
{/*{{{*/
  CONT_INIT_CLEAR(fd_s,fd);
  fd = open(a_trace_data->path.data,O_RDWR,0);

  if (fd == -1)
  {
    throw_error(SD_TRACE_DESCR_FILE_OPEN_ERROR);
  }

  if (sd_trace_mmap_s_create(a_trace_mmap,NULL,a_trace_data->size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,a_trace_data->offset))
  {
    throw_error(SD_TRACE_DESCR_MMAP_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

// -- sd_trace_tree_s --
@begin
inlines sd_trace_tree_s
@end

static inline int sd_trace_tree_s___compare_value(const sd_trace_tree_s *this,const sd_trace_descr_s *a_first,const sd_trace_descr_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->config.trace_id;
  const string_s *second = &a_second->config.trace_id;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }

  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

// -- sd_daemon_s --
@begin
inlines sd_daemon_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

