
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

  CONT_INIT(socket_s,srv_socket);
  cassert(socket_s_create(&srv_socket,AF_INET,SOCK_STREAM) == 0);
  cassert(socket_s_listen(&srv_socket,&address,10) == 0);

  CONT_INIT(socket_s,conn_socket);
  CONT_INIT(bc_array_s,message);

  unsigned idx = 0;
  do {
    cassert(socket_s_accept(&srv_socket,&conn_socket,&address) == 0);

    // - receive message -
    message.used = 0;
    cassert(fd_s_read(&conn_socket,&message) == 0);

    // - print message -
    bc_array_s_push(&message,'\0');
    puts(message.data);
    --message.used;

    bc_array_s_append_ptr(&message," - Response");

    // - send response -
    cassert(fd_s_write(&conn_socket,message.data,message.used) == 0);

  } while(++idx < 10);

  bc_array_s_clear(&message);
  socket_s_clear(&conn_socket);
  socket_s_clear(&srv_socket);

  memcheck_release_assert();

  return 0;
}/*}}}*/

