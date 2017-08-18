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
** En attendant Nada, ca nous permet de tester notre serveur.
**
** Ce client est un REP/REQ PUB/SUB foireu.
** En effet, on aimerait qu'un client reçoive les PUB/SUB
** en continue, sans pour autant que cela l'empêche de rentrer
** des inputs. Les conditions ne sont pour l'instant pas bien
** définies et je me contente d'un trick à l'aide d'un int pour
** être sur de récupérer les 2 envois du serveur.

** Le workflow est le suivant:
** 1 - Input REP/REQ (la REQ du coup...) avec un readline()
** 2 - réception de la réponse (la REP du serveur)
** 3 - tour de boucle intérieure car synchroniseur != 2
** 4 - réception de la publication par le subscriber.
*/

int		main(int argc, char *argv[])
{
  
  /*
  ** Client program management
  ** gestion des paramètres/options
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
  ** Client REP/REQ and PUB/SUB management in poller.
  */
  int		rep_port;
  int		pub_port;
  zsock_t	*dealer;
  zsock_t	*subscriber;
  zsock_t	*active_poll;
  zpoller_t	*poller;
  char		*input;
  zmsg_t	*msg;
  char		*output;
  char		**commands;
  zmsg_t	*response;
  char		*publication;
  zframe_t	*address;
  char		log[60];
  char		*identity;
  
  /*
  ** hardcoded ports
  */
  rep_port = 4242;
  pub_port = 4243;

  /*
  ** Initialisation de la socket REP/REQ côté REQUEST
  */
  my_log(__func__, "init request socket", 3);
  dealer = zsock_new(ZMQ_DEALER);
  assert(dealer);
  my_log(__func__, "connect dealer to localhost:4242", 3);
  zsock_connect(dealer, "tcp://localhost:%d", rep_port);

  /*
  ** On envoit un premier message qui permet
  ** de simuler une gestion du nombre de joueur
  ** côté serveur. Pareil à la déconnection.
  */
  my_log(__func__, "inform serveur of connection", 3);
  msg = zmsg_new();
  zmsg_pushstr(msg, "identity");
  zmsg_pushstr(msg, "identify");
  my_log(__func__, "before zmsg send", 3);
  zmsg_send(&msg, dealer);
  my_log(__func__, "before zmsg recv", 3);
  response = zmsg_recv(dealer);
  address = zmsg_pop(response);
  if (address == NULL)
    my_log(__func__, "address est null", 3);
  else
    my_log(__func__, "address isnt null", 3);
  output = zmsg_popstr(response);
  commands = my_split(output, '|');
  if (!my_strcmp(commands[0], "ok"))
    {
      my_log(__func__, "connection ok", 3);
      if ((identity = my_strdup(commands[1])) == NULL)
	return (1);
      sprintf(log, "identity set to: %s", identity);
      my_log(__func__, log, 3);
    }

  /*
  ** On initialise le subscriber PUB/SUB
  */
  my_log(__func__, "init subscriber", 3);
  subscriber = zsock_new(ZMQ_SUB);
  assert(subscriber);
  my_log(__func__, "connect subscriber to localhost:4243", 3);
  zsock_connect(subscriber, "tcp://localhost:%d", pub_port);

  /*
  ** On le lie au channel SoftWar, côté serveur,
  ** les messages envoyés devront commencer par
  ** "SoftWar ".
  */
  my_log(__func__, "subscribe to channel SoftWar", 3);
  zsock_set_subscribe(subscriber, "SoftWar");
  while (!zsys_interrupted)
    {
      msg = zmsg_new();
      if ((input = readline()) != NULL)
	{
	  my_log(__func__, "before push identity", 3);
	  zmsg_pushstr(msg, identity);
	  my_log(__func__, "before push input", 3);
	  zmsg_pushstr(msg, input);
	  my_log(__func__, "before push address", 3);
	  zmsg_send(&msg, dealer);
	}
      my_putchar('\n');

      /*
      ** CLIENT poll init
      ** Je déclare une poll avec mes deux socket
      */
      my_log(__func__, "poller init", 3);
      poller = zpoller_new(dealer, subscriber, NULL);
      assert(poller);
      /*
      ** poller listen and wait
      ** je demande à mon poller d'attendre
      */
      my_log(__func__, "poller wait an active socket", 3);
      active_poll = (zsock_t*)zpoller_wait(poller, 20); // n'attendra pas plus de 20 msec
      /*
      ** je test sur active_poll
      ** pour savoir quelle socket
      ** s'est manifesté
      */
      if (active_poll == dealer)
	{
	  assert(zpoller_expired(poller) == false);
	  assert(zpoller_terminated(poller) == false);
	  response = zmsg_recv(dealer);
	  address = zmsg_pop(response);
	  while (zmsg_size(response) > 0)
	    {
	      my_putstr("\nresponse from sw_server: ");
	      my_putstr(zmsg_popstr(response));
	      my_putchar('\n');
	    }
	  zmsg_destroy(&response);
	}
      if (active_poll == subscriber)
	{
	  assert(zpoller_expired(poller) == false);
	  assert(zpoller_terminated(poller) == false);
	  publication = zstr_recv(subscriber);
	  my_putstr("\nmessage published on 4243: ");
	  my_putstr(publication);
	  my_putchar('\n');
	  zstr_free(&publication);
	}
    }
  /*
  ** avec le zsys_interrupted, un ctrl + c nous amène là...
  */
  msg = zmsg_new();
  zmsg_pushstr(msg, identity);
  zmsg_pushstr(msg, "leave|");
  zmsg_send(&msg, dealer);
  zmsg_destroy(&msg);
  response = zmsg_recv(dealer);
  while (zmsg_size(response) > 0)
    {
      my_putstr("\nSoftwar says: ");
      my_putstr(zmsg_popstr(response));
      my_putstr("\n");
    }
  zmsg_destroy(&response);
  zsock_destroy(&dealer);
  zsock_destroy(&subscriber);
  zstr_free(&input);
  zstr_free(&publication);
  zpoller_destroy(&poller);
  /*
  ** libmy extended cleaning
  */
  delete_logger();
  delete_chain(&options);
  return(0);
}
