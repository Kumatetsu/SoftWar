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
#include "argument.h"
#include "Notification.h"

int		main(int argc, char *argv[])
{
  /*
  ** Program management part
  */
  int		act;
  t_chain	*options;
  t_link	*tmp;
  t_option	*opt;

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
  ** Server REP/REQ, PUB/SUB and polling management
  */
  // basic and hardcoded players management
  int		players;
  int		rep_port;
  int		pub_port;
  zsock_t	*responder;
  zsock_t	*publisher;
  zsock_t	*active_poll;
  zpoller_t	*poller;
  char		*message = NULL;

  /*
  ** hard port's definition
  */
  rep_port = 4242;
  pub_port = 4243;
  /*
  ** REP/REQ server init
  */
  my_log(__func__, "init responder", 3);
  responder = zsock_new(ZMQ_REP);
  assert(responder);
  my_log(__func__, "bind responder to port 4242", 3);
  zsock_bind(responder, "tcp://*:%d", rep_port);
  my_log(__func__, "server ready to REP/REQ on 4242", 3);
  /*
  ** PUB/SUB server init
  */
  my_log(__func__, "init publisher", 3);
  publisher = zsock_new(ZMQ_PUB);
  assert(publisher);
  my_log(__func__, "bind publisher to port 4243", 3);
  zsock_bind(publisher, "tcp://*:%d", pub_port);
  my_log(__func__, "publisher ready to publish on 4243", 3);
  while(!zsys_interrupted)
    {
      /*
      ** Poll init
      */
      my_log(__func__, "init poller", 3);
      poller = zpoller_new(responder, NULL);
      assert(poller);
      my_log(__func__, "wait an active socket", 3);
      /*
      ** Poll listen and wait
      */
      active_poll = (zsock_t*)zpoller_wait(poller, -1);
      assert(active_poll == responder); // allow to retrieve active socket
      assert(zpoller_expired(poller) == false); // check poller status
      assert(zpoller_terminated(poller) == false); // check poller status
      /*
      ** Read message from active socket
      */
      message = zstr_recv(responder);
      my_putstr("\nmessage received: ");
      my_putstr(message);
      my_putchar('\n');
      /*
      ** REP/REQ response sending
      */
      my_log(__func__, "response to request", 3);
      if (!my_strcmp(message, "client connection init"))
	{
	  zstr_sendf(responder, "%s", "Client connection acknowledge");
	  players++;
	}
      else if(!my_strcmp(message, "client connection destroyed"))
	{
	  zstr_sendf(message, "Good Bye!");
	  players--;
	}
      else
	zstr_sendf(responder, "%s", message);
      /*
      ** Publication to all clients:
      ** The sender should receive his message 2 times,
      ** other clients only once from publisher.
      */
      my_log(__func__, "publish message: ", 3);
      my_log(__func__, message, 3);
      zstr_sendf(publisher, "SoftWar %s", message);
      zstr_free(&message);
    }
  zsock_destroy(&responder);
  zsock_destroy(&publisher);
  /*
  ** libmy extended cleaning
  */
  delete_logger();
  delete_chain(&options);
  return(0);
}
