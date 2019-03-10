
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "liblinux_cll_test";

const char *test_names[] =
{/*{{{*/
  "fd",
  "socket_address",
  "socket",
  "socket_udp",
  "socket_tcp",
  "pid",
  "epoll",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_fd,
  test_socket_address,
  test_socket,
  test_socket_udp,
  test_socket_tcp,
  test_pid,
  test_epoll,
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
  cassert(regex_s_create(&regex,"^fd_s{[0-9]\\+}$") == 0);
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

  CONT_INIT(regex_s,regex);
  //regmatch_s match;

  //CONT_INIT(epoll_s,epoll);
  //EPOLL_S_TO_BUFFER(&epoll);
  //puts(buffer.data);
  //cassert(strcmp(buffer.data,"{fd:fd_s{-1},events:[],revents:[]}") == 0);

  //// - epoll_s_create -
  //epoll_s_create(&epoll,0);
  //EPOLL_S_TO_BUFFER(&epoll);
  //puts(buffer.data);

  //cassert(regex_s_create(&regex,"^{fd:fd_s{[0-9]\\+},events:\\[\\],revents:\\[\\]}$") == 0);
  //cassert(regex_s_match(&regex,buffer.data,&match));

  //// - epoll_s_fd -
  //cassert(epoll_s_fd(&epoll,0,EPOLLIN | EPOLLPRI) == 0);
  //cassert(epoll_s_fd(&epoll,1,EPOLLIN | EPOLLPRI) == 0);
  //cassert(epoll_s_fd(&epoll,2,EPOLLIN | EPOLLPRI) == 0);

  //EPOLL_S_TO_BUFFER(&epoll);
  //puts(buffer.data);

  //cassert(regex_s_create(&regex,"^{fd:fd_s{[0-9]\\+},events:\\[3,3,3\\],revents:\\[\\]}$") == 0);
  //cassert(regex_s_match(&regex,buffer.data,&match));

  //cassert(epoll_s_fd(&epoll,0,EPOLLIN) == 0)
  //cassert(epoll_s_fd(&epoll,1,0) == 0)

  //EPOLL_S_TO_BUFFER(&epoll);
  //puts(buffer.data);

  //cassert(regex_s_create(&regex,"^{fd:fd_s{[0-9]\\+},events:\\[1,0,3\\],revents:\\[\\]}$") == 0);
  //cassert(regex_s_match(&regex,buffer.data,&match));

  //int fd = 0;
  //while (fd < epoll.fd_events.used)
  //{
  //  cassert(epoll_s_fd(&epoll,fd++,0) == 0);
  //}

  //EPOLL_S_TO_BUFFER(&epoll);
  //puts(buffer.data);

  //cassert(regex_s_create(&regex,"^{fd:fd_s{[0-9]\\+},events:\\[0,0,0\\],revents:\\[\\]}$") == 0);
  //cassert(regex_s_match(&regex,buffer.data,&match));

  //CONT_INIT(pipe_s,pipe);
  //cassert(pipe_s_popen(&pipe,"ls","r") == 0);

  //cassert(epoll_s_fd(&epoll,stream_s_fd(&pipe),EPOLLIN | EPOLLPRI) == 0);

  //// - epoll_s_wait -
  //cassert(epoll_s_wait(&epoll,16,-1) == 0);
  //UI_ARRAY_S_TO_BUFFER(&epoll.revents);
  //puts(buffer.data);

  //pipe_s_clear(&pipe);
  //epoll_s_clear(&epoll);
  regex_s_clear(&regex);
  bc_array_s_clear(&buffer);
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

