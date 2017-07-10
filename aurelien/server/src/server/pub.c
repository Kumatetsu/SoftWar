/*
** pub.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul  9 22:08:04 2017 CASTELLARNAU Aurelien
** Last update Sun Jul  9 22:13:13 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include <czmq.h>
#include "libmy.h"
#include "pub.h"

/*
** initialise et bind une pub socket
** avec les logs et les assertions.
*/
zsock_t		*init_pub(int pub_port)
{
  char		mess[50];
  zsock_t	*publisher;
  
  my_log(__func__, "init publisher", 3);
  publisher = zsock_new(ZMQ_PUB);
  assert(publisher);
  sprintf(mess, "bind publisher to port %d", pub_port);
  my_log(__func__, mess, 3);
  zsock_bind(publisher, "tcp://*:%d", pub_port);
  sprintf(mess, "publisher ready to publish on: %d", pub_port);
  my_log(__func__, mess, 3);
  return (publisher);
}
