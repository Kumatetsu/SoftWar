/*
** router.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Mon Jul 31 18:12:17 2017 CASTELLARNAU Aurelien
** Last update Sat Sep 30 18:04:25 2017 BILLAUD Jean
*/

#include <czmq.h>
#include "libmy.h"
#include "router.h"

/*
** Initialise une socket router
*/
zsock_t		*init_router(int rep_port)
{
  zsock_t	*router;
  char		log[80];

  my_log(__func__, "init router", 3);
  router = zsock_new(ZMQ_ROUTER);
  assert(router);
  sprintf(log, "bind router to port %d", rep_port);
  my_log(__func__, log, 3);
  zsock_bind(router, "tcp://*:%d", rep_port);
  sprintf(log, "server ready to REQ/ROUTER on %d", rep_port);
  my_log(__func__, log, 3);
  return (router);
}

zmsg_t		*router_read(zsock_t *socket)
{
  zmsg_t	*message;
  zframe_t	*address;
  char		*identity;
  char		*void_frame;
  char		*content;

  if ((message = malloc(sizeof(zmsg_t*))) == NULL)
    return (NULL);
  if ((message = zmsg_recv(socket)) != NULL)
    {
      /*
      ** illustration inutile
      ** la première frame contient l'adresse,
      ** elle est pop puis push,
      ** elle se retrouve à la première place
      */
      address = zmsg_pop(message);
      void_frame = zmsg_popstr(message);
      content = zmsg_popstr(message);
      identity = zmsg_popstr(message);
      zmsg_pushstr(message, identity);
      zmsg_pushstr(message, content);
      zmsg_pushstr(message, void_frame);
      zmsg_push(message, address);
      my_log(__func__, "Dans l'ordre: void_frame, content, identity", 3);
      my_log(__func__, void_frame, 3);
      my_log(__func__, content, 3);
      my_log(__func__, identity, 3);
    }
  return (message);
}

/*
int	router_response(zsock_t *socket, zmsg_t **msg)
{
  to be continued?
}
*/
