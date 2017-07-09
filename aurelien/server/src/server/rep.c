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
