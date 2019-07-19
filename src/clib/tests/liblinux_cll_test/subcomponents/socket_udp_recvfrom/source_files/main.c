
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
  cassert(socket_address_s_create(&address,"127.0.0.1",8001) == 0);

  CONT_INIT(socket_s,socket);
  cassert(socket_s_create(&socket,AF_INET,SOCK_DGRAM) == 0);
  cassert(socket_s_bind(&socket,&address) == 0);

  CONT_INIT(bc_array_s,message);
  socket_address_s src_addr;

  do {

    // - receive message -
    message.used = 0;
    cassert(socket_s_recvfrom(&socket,&message,&src_addr) == 0);
    bc_array_s_push(&message,'\0');

    // - print message -
    puts(message.data);

    // - if end message was received -
    if (strcmp(message.data,"END") == 0)
    {
      break;
    }
  } while(1);

  bc_array_s_clear(&message);
  socket_s_clear(&socket);

  memcheck_release_assert();

  return 0;
}/*}}}*/

