/*
** main.c for SoftWar in /home/code/SoftWar/aurelien
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sat Jul  1 01:11:51 2017 CASTELLARNAU Aurelien
** Last update Sun Sep 10 16:33:45 2017 BILLAUD Jean
*/

#include <czmq.h>

int		main(int argc, char *argv[])
{
  if (argc < 2) {
    printf("Port number is mandatory\n");
    return 0;
  }

  zsock_t *chat_srv_socket = zsock_new(ZMQ_PUB);
  zsock_bind(chat_srv_socket, "tcp://*:%s", argv[1]);
  printf("Server listening on tcp://*:%s", argv[1]);

  while (!zsys_interrupted) {
    char message[1024];

    if (!fgets(message, 1024, stdin)) {
      break;
    }
    zstr_sendf(chat_srv_socket, "%s", message);
  }
  zsock_destroy(&chat_srv_socket);
  return 0;
}
