
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "liblinux_cll_test";

const char *test_names[] =
{/*{{{*/
  "fd",
  "dir",
  "mmap",
  "socket_address",
  "socket",
  "socket_udp",
  "socket_tcp",
  "aio",
  "pid",
  "pthread",
  "epoll",
  "rtc",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_fd,
  test_dir,
  test_mmap,
  test_socket_address,
  test_socket,
  test_socket_udp,
  test_socket_tcp,
  test_aio,
  test_pid,
  test_pthread,
  test_epoll,
  test_rtc,
};/*}}}*/

// === methods of generated structures =========================================

// -- fd_array_s --
@begin
methods fd_array_s
@end

// === test execution functions ================================================

void test_fd()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define FD_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  fd_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

#define FD_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  fd_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  CONT_INIT(regex_s,regex);
  regmatch_s match;

  fd_s fd = creat("tests/liblinux_cll_test/fd/test.file",0666);
  FD_S_TO_BUFFER(&fd);
  cassert(regex_s_create(&regex,"^fd_s\\{[0-9]+\\}$") == 0);
  cassert(regex_s_match(&regex,buffer.data,&match));

  CONT_INIT(fd_array_s,fd_array);
  unsigned idx = 0;
  do {
    fd_array_s_push_blank(&fd_array);
  } while(++idx < 5);
  FD_ARRAY_S_TO_BUFFER(&fd_array);
  cassert(strcmp(buffer.data,"[fd_s{-1},fd_s{-1},fd_s{-1},fd_s{-1},fd_s{-1}]") == 0);

  fd_s_clear(&fd);
  regex_s_clear(&regex);
  fd_array_s_clear(&fd_array);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_dir()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(string_tree_s,filenames);

  CONT_INIT_CLEAR(dir_s,dir);
  cassert(dir_s_open(&dir,"tests/liblinux_cll_test/resources/dir") == 0);

  struct dirent *de;
  do {
    cassert(dir_s_read(&dir,&de) == 0);
    if (de == NULL) { break; }

    // - is regular file -
    if (de->d_type == DT_REG)
    {
      CONT_INIT_CLEAR(string_s,filename);
      string_s_set_ptr(&filename,de->d_name);
      string_tree_s_swap_insert(&filenames,&filename);
    }
  } while(1);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  string_tree_s_to_string(&filenames,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[file_1,file_2,file_3,file_4,file_5]") == 0);
#endif
}/*}}}*/

void test_mmap()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define MMAP_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  mmap_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  CONT_INIT_CLEAR(fd_s,fd);
  fd = open("tests/liblinux_cll_test/mmap/test.file",O_CREAT | O_RDWR,0666);

  bc_array_s_push_blanks(&buffer,1024);
  memset(buffer.data,'0',buffer.used);
  cassert(fd_s_write(&fd,buffer.data,buffer.used) == 0);

  CONT_INIT_CLEAR(mmap_s,mmap);
  cassert(mmap_s_create(&mmap,NULL,1024,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0) == 0);
  fd_s_clear(&fd);

  char *ptr = (char *)mmap.address;
  char *ptr_end = ptr + mmap.length;
  do {
    *ptr = '0' + ((ptr - (char *)mmap.address) % 10);
  } while(++ptr < ptr_end);

  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_socket_address()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define SOCKET_ADDRESS_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  socket_address_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  socket_address_s address;
  cassert(socket_address_s_create(&address,"127.0.0.1",8000) == 0);
  SOCKET_ADDRESS_S_TO_BUFFER(&address);
  puts(buffer.data);

  CONT_INIT(bc_array_s,name);
  cassert(socket_address_s_name(&address,&name) == 0);
  bc_array_s_push(&name,'\0');
  cassert(strcmp(name.data,"127.0.0.1") == 0);

  unsigned short port = socket_address_s_port(&address);
  cassert(port == 8000);

  bc_array_s_clear(&name);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_socket()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define SOCKET_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  socket_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  CONT_INIT(socket_s,socket_0);
  cassert(socket_s_create(&socket_0,AF_INET,SOCK_STREAM) == 0);
  SOCKET_S_TO_BUFFER(&socket_0);
  puts(buffer.data);

  socket_s_clear(&socket_0);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_socket_udp()
{/*{{{*/
  CONT_INIT(string_array_s,arguments);
  string_array_s_push_ptr(&arguments,"./liblinux_cll_test_socket_udp_recvfrom");

  CONT_INIT(pid_s,recv_pid);
  cassert(pid_s_execute(&recv_pid,&arguments) == 0);
  usleep(100000);

  arguments.used = 0;
  string_array_s_push_ptr(&arguments,"./liblinux_cll_test_socket_udp_sendto");

  CONT_INIT(pid_s,send_pid);
  cassert(pid_s_execute(&send_pid,&arguments) == 0);

  int status;
  cassert(waitpid(send_pid,&status,0) != -1 && status == 0);
  cassert(waitpid(recv_pid,&status,0) != -1 && status == 0);

  pid_s_clear(&send_pid);
  pid_s_clear(&recv_pid);
  string_array_s_clear(&arguments);
}/*}}}*/

void test_socket_tcp()
{/*{{{*/
  CONT_INIT(string_array_s,arguments);
  string_array_s_push_ptr(&arguments,"./liblinux_cll_test_socket_tcp_server");

  CONT_INIT(pid_s,server_pid);
  cassert(pid_s_execute(&server_pid,&arguments) == 0);
  usleep(100000);

  arguments.used = 0;
  string_array_s_push_ptr(&arguments,"./liblinux_cll_test_socket_tcp_client");

  CONT_INIT(pid_s,client_pid);
  cassert(pid_s_execute(&client_pid,&arguments) == 0);

  int status;
  cassert(waitpid(client_pid,&status,0) != -1 && status == 0);
  cassert(waitpid(server_pid,&status,0) != -1 && status == 0);

  pid_s_clear(&client_pid);
  pid_s_clear(&server_pid);
  string_array_s_clear(&arguments);
}/*}}}*/

void test_aio()
{/*{{{*/
#if SUBSYSTEM_TYPE == SUBSYSTEM_TYPE_LINUX
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define AIO_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  aio_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  // - aio_s_write -
  CONT_INIT_CLEAR(file_s,file);
  cassert(file_s_open(&file,"tests/liblinux_cll_test/aio/write","w") == 0);

  CONT_INIT_CLEAR(aio_s,aio);

  const char write_data[] =
"{\n"
"  \"null\": null,\n"
"  \"integer\": 123,\n"
"  \"float\": 123.400000,\n"
"  \"string\": \"Hello world!\",\n"
"  \"array\": [\n"
"    0,\n"
"    1,\n"
"    2,\n"
"    3,\n"
"    4,\n"
"    5,\n"
"    6,\n"
"    7,\n"
"    8,\n"
"    9\n"
"  ],\n"
"  \"object\": {\n"
"    \"first\": 0,\n"
"    \"second\": 1,\n"
"    \"third\": 2\n"
"  }\n"
"}";

  size_t data_size = sizeof(write_data);
  cassert(aio_s_write(&aio,stream_s_fd(&file),0,(void *)write_data,data_size) == 0);

  int done;
  do {
    usleep(100);

    cassert(aio_s_is_done(&aio,&done) == 0);
  } while(!done);

  ssize_t result;
  cassert(aio_s_return(&aio,&result) == 0 && result == (ssize_t)data_size);

  // - aio_s_read -
  cassert(file_s_open(&file,"tests/liblinux_cll_test/aio/write","r") == 0);

  char read_data[data_size];
  cassert(aio_s_read(&aio,stream_s_fd(&file),0,(void *)read_data,data_size) == 0);

  do {
    usleep(100);

    cassert(aio_s_is_done(&aio,&done) == 0);
  } while(!done);

  cassert(aio_s_return(&aio,&result) == 0 && result == (ssize_t)data_size);
  cassert(strcmp(write_data,read_data) == 0);

  bc_array_s_clear(&buffer);
#endif
#endif
}/*}}}*/

void test_pid()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define PID_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  pid_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  CONT_INIT(string_array_s,arguments);

  string_array_s_push_blank(&arguments);
  string_s_set_ptr(string_array_s_last(&arguments),"ls");

  // - pid_s_execute -
  CONT_INIT(pid_s,pid_0);
  cassert(pid_s_execute(&pid_0,&arguments) == 0);

  int status;
  waitpid(pid_0,&status,0);

  pid_s_clear(&pid_0);
  string_array_s_clear(&arguments);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void *thread_function(void *a_arg)
{/*{{{*/
  (void)a_arg;

  return NULL;
}/*}}}*/

void test_pthread()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(bc_array_s,buffer);

#define PTHREAD_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  pthread_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  CONT_INIT_CLEAR(regex_s,regex);
  cassert(regex_s_create(&regex,"^pthread_s\\{0x[0-9a-f]+,1\\}$") == 0);
  regmatch_s match;

  CONT_INIT_CLEAR(pthread_s,thread);

  // - pthread_s_join -
  PTHREAD_S_TO_BUFFER(&thread);
#if SUBSYSTEM_TYPE == SUBSYSTEM_TYPE_MSYS2
  cassert(strcmp(buffer.data,"pthread_s{0x0,0}") == 0);
#else
  cassert(strcmp(buffer.data,"pthread_s{(nil),0}") == 0);
#endif

  cassert(pthread_s_create(&thread,NULL,thread_function,NULL) == 0);
  PTHREAD_S_TO_BUFFER(&thread);
  cassert(regex_s_match(&regex,buffer.data,&match));

  cassert(pthread_s_join(&thread,NULL) == 0);
  PTHREAD_S_TO_BUFFER(&thread);
#if SUBSYSTEM_TYPE == SUBSYSTEM_TYPE_MSYS2
  cassert(strcmp(buffer.data,"pthread_s{0x0,0}") == 0);
#else
  cassert(strcmp(buffer.data,"pthread_s{(nil),0}") == 0);
#endif

  // - pthread_s_detach -
  cassert(pthread_s_create(&thread,NULL,thread_function,NULL) == 0);
  PTHREAD_S_TO_BUFFER(&thread);
  cassert(regex_s_match(&regex,buffer.data,&match));

  cassert(pthread_s_join(&thread,NULL) == 0);
  PTHREAD_S_TO_BUFFER(&thread);
#if SUBSYSTEM_TYPE == SUBSYSTEM_TYPE_MSYS2
  cassert(strcmp(buffer.data,"pthread_s{0x0,0}") == 0);
#else
  cassert(strcmp(buffer.data,"pthread_s{(nil),0}") == 0);
#endif
#endif
}/*}}}*/

void test_epoll()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define EPOLL_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  epoll_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

#define UI_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  ui_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_rtc()
{/*{{{*/
#define ENABLE_RTC_TEST_ONE 0
#define ENABLE_RTC_TEST_TWO 0

#if ENABLE_RTC_TEST_ONE
  CONT_INIT_CLEAR(rtc_s,rtc);
  cassert(rtc_s_open(&rtc,"/dev/rtc0") == 0);

  time_s time;
  cassert(rtc_s_read_time(&rtc,&time) == 0);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  time_s_to_string(&time,&buffer);
#endif

#if ENABLE_RTC_TEST_TWO
  time_s time;
  cassert(clock_s_gettime(CLOCK_REALTIME,&time) == 0);

  CONT_INIT_CLEAR(rtc_s,rtc);
  cassert(rtc_s_open(&rtc,"/dev/rtc0") == 0);
  cassert(rtc_s_write_time(&rtc,time) == 0);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

