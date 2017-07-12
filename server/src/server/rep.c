/*
** repreq.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul  9 21:58:29 2017 CASTELLARNAU Aurelien
** Last update Sun Jul  9 22:03:24 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include <czmq.h>
#include "libmy.h"
#include "rep.h"

/*
** Initialise une requête rep
** avec les logs et les assertions.
*/
zsock_t		*init_rep(int rep_port)
{
  zsock_t	*r;

  my_log(__func__, "init responder", 3);
  r = zsock_new(ZMQ_REP);
  assert(r);
  my_log(__func__, "bind responder to port 4242", 3);
  zsock_bind(r, "tcp://*:%d", rep_port);
  my_log(__func__, "server ready to REP/REQ on 4242", 3);
  return (r);
}

char	*rep_read(zsock_t *socket)
{
  char	*message;

  my_log(__func__, "extract message from rep socket", 3);
  message = zstr_recv(socket);
  my_putstr_color("cyan", "\nMessage received from [user]: ");
  my_putstr(message);
  my_putchar('\n');
  rep_response(socket, message);
  return (message);
}

void	rep_response(zsock_t *socket, char *message )
{
  char	log[60];

  sprintf(log, "response to request: %s", message);
  my_log(__func__, log, 3);
  if (!my_strcmp(message, "client connection init"))
    {
      zstr_sendf(socket, "%s", "Client connection acknowledge");
      // ctx->players++;
    }
  else if(!my_strcmp(message, "client connection destroyed"))
    {
      zstr_sendf(socket, "Good Bye!");
      // ctx->players--;
    }
  else
    zstr_sendf(socket, "%s", message);
}
