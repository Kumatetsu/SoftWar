/*
** poll.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul  9 22:18:46 2017 CASTELLARNAU Aurelien
** Last update Sun Jul  9 22:26:44 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include <czmq.h>
#include "libmy.h"
#include "softwar_ctx.h"
#include "rep.h"
#include "router.h"
#include "poll.h"

/*
** Et là ca commence à me plaire,
** on a une poll de socket, les sockets
** et leurs noms, dans la même variable...
*/
zmsg_t  	*init_poll(t_swctx **ctx)
{
  zmsg_t	*message;
  t_link	*tmp;
  t_swsock	*sw_socket;

  (*ctx)->poller = zpoller_new(NULL);
  tmp = (*ctx)->sockets->first;
  while (tmp)
    {
      sw_socket = tmp->content;
      if (my_strcmp(sw_socket->name, "pubi"))
	{
	  my_log(__func__, "add socket to poll", 3);
	  my_log(__func__, sw_socket->name, 3);
	  zpoller_add((*ctx)->poller, (zsock_t*)sw_socket->socket);
	}
      tmp = tmp->next;
    }
  if (!listen_and_wait(ctx))
    message = read_socket(ctx);
  if (!message)
    my_log(__func__, "no message return... failed", 2);
  return (message);
}

/*
** listen_and_wait set l'active_socket du ctx
** ctx->active_socket est de notre type perso t_swsock.
** donc la socket en provenance du zpoller_wait 
*/
int		listen_and_wait(t_swctx **ctx)
{
  char		log[BUFFER];
  t_link	*tmp;
  t_swsock	*sw_socket;

  tmp = (*ctx)->sockets->first;
  my_log(__func__, "wait an active socket", 3);
  (*ctx)->active_socket->socket = (zsock_t*)zpoller_wait((*ctx)->poller, -1);
  assert(zpoller_expired((*ctx)->poller) == false);
  assert(zpoller_terminated((*ctx)->poller) == false);
  while (tmp)
    {
      sw_socket = tmp->content;
      if (((zsock_t*)sw_socket->socket) == (zsock_t*)((*ctx)->active_socket->socket))
	if (((*ctx)->active_socket->name = my_strdup(sw_socket->name)) == NULL)
	  return (1);
      tmp = tmp->next;
    }
  if ((*ctx)->active_socket->name == NULL)
    {
      /*
      ** retourne mais affiche un warning,
      ** un fail ici n'arrête pas le serveur pour autant.
      */
      my_log(__func__, "polling failed: unable to retrieve active socket", 2);
      return (1);
    }
  sprintf(log, "active socket set: %s", (*ctx)->active_socket->name);
  my_log(__func__, log, 3);
  return (0);
}

/*
** Pour le moment, le couple read_socket/router_read
** est inutile...
** La fonction read_socket aurait pour but de différencier
** le traitement en fonction de la socket. 
*/
zmsg_t		*read_socket(t_swctx **ctx)
{
  zmsg_t	*msg;

  my_log(__func__, "try to compare setted active socket with known ones", 3);
  if (!my_strcmp((*ctx)->active_socket->name, "router"))
    {
      if ((msg = router_read((*ctx)->active_socket->socket)) == NULL)
	{
	  my_log(__func__, "reading router socket failed", 1);
	  return (NULL);
	}
      return (msg);
    }
  my_log(__func__, "unable to compare the active socket, no match", 2);
  return (NULL);
}

/*
** Retrouve la socket dans le ctx grâce à son nom
*/
zsock_t		*get_socket(char *name, t_swctx **ctx)
{
  char		log[BUFFER];
  t_link	*tmp;
  t_swsock	*sw_s;

  tmp = (*ctx)->sockets->first;
  while (tmp)
    {
      sw_s = tmp->content;
      if (!my_strcmp(sw_s->name, name))
	return (sw_s->socket);
      tmp = tmp->next;
    }
  sprintf(log, "no socket with name: %s", name);
  my_log(__func__, log, 2);
  return (NULL);
}
