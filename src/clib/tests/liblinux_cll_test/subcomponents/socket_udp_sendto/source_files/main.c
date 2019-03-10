
@begin
include "main.h"
@end

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  socket_address_s src_addr;
  cassert(socket_address_s_create(&src_addr,"127.0.0.1",8000) == 0);

  socket_address_s trg_addr;
  cassert(socket_address_s_create(&trg_addr,"127.0.0.1",8001) == 0);

  CONT_INIT(socket_s,socket);
  cassert(socket_s_create(&socket,AF_INET,SOCK_DGRAM) == 0);
  cassert(socket_s_bind(&socket,&src_addr) == 0);

  const char data_message[] = "Message from socket_udp_sendto";
  const char end_message[] = "END";

  unsigned idx = 0;
  do {

    // - send data message -
    cassert(socket_s_sendto(&socket,&trg_addr,data_message,strlen(data_message)) == 0);
  } while(++idx < 10);

  // - send end message -
  cassert(socket_s_sendto(&socket,&trg_addr,end_message,strlen(end_message)) == 0);

  socket_s_clear(&socket);

  memcheck_release_assert();

  return 0;
}/*}}}*/

