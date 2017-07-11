/*
** main.c for SoftWar in /home/code/SoftWar/aurelien
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sat Jul  1 01:11:51 2017 CASTELLARNAU Aurelien
** Last update Sat Jul  1 02:29:20 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include <stdlib.h>
#include <czmq.h>
#include "libmy.h"
#include "protocol.h"
#include "argument.h"

/*
** Client fake context

** Ce client est un PUB/SUB uniquement
** Je l'ai codé pour pouvoir recevoir les
** publications du serveur sans m'embêter avec
** le côté bloquand du readline.
** Se serait la partie display du client de jeu.
*/

int		main(int argc, char *argv[])
{
  
  /*
  ** Client program management
  */
  int           act;
  t_chain       *options;
  t_link        *tmp;
  t_option      *opt;

  act = 0;
  options = get_options();
  if (parse(1, argc, argv, &options))
    {
      devlog(__func__, "parsing arguments failed", 1);
      return (1);
    }
  tmp = options->first;
  while (tmp)
    {
      opt = (t_option*)tmp->content;
      if (opt->to_execute)
	{
	  act = 1;
	  execute(opt);
	}
      tmp = tmp->next;
    }
  if (!act)
    help();
  /*
  ** Client PUB/SUB uniquement
  */
  int		pub_port;
  zsock_t	*subscriber;
  char		*publication;

  pub_port = 4243; // hardcoded port
  
  my_log(__func__, "init subscriber", 3);
  subscriber = zsock_new(ZMQ_SUB); // SUB socket initialisation
  assert(subscriber); // test critique, si ca passe pas, c'est un gros fail critique.
  zsock_connect(subscriber, "tcp://127.0.0.1:%d", pub_port); // le client connect, le server bind
  my_log(__func__, "subscriber listening on 4243", 3);
  zsock_set_subscribe(subscriber, "SoftWar"); // on subscribe aux messages commençant par "SoftWar "
  while (!zsys_interrupted)
    {
      publication = zstr_recv(subscriber);
      my_putstr("message published on 4243: ");
      my_putstr(publication);
      my_putstr("\n");
      zstr_free(&publication);
    }
  zsock_destroy(&subscriber);
  /*
  ** libmy extended cleaning
  */
  delete_logger();
  delete_chain(&options);
  return(0);
}
