
@begin
include "main.h"
@end

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  socket_address_s address;
  cassert(socket_address_s_create(&address,"127.0.0.1",8000) == 0);

  CONT_INIT(socket_s,socket);
  CONT_INIT(bc_array_s,message);

  const char data_message[] = "Message from socket_tcp_client";

  unsigned idx = 0;
  do {

    // - socket_s_connect -
    cassert(socket_s_create(&socket,AF_INET,SOCK_STREAM) == 0);
    cassert(socket_s_connect(&socket,&address) == 0);

    // - send message -
    cassert(fd_s_write(&socket,data_message,strlen(data_message)) == 0)

    // - receive response -
    message.used = 0;
    cassert(fd_s_read(&socket,&message) == 0)

    // - print response -
    bc_array_s_push(&message,'\0');
    puts(message.data);

  } while(++idx < 10);

  bc_array_s_clear(&message);
  socket_s_clear(&socket);

  memcheck_release_assert();

  return 0;
}/*}}}*/

