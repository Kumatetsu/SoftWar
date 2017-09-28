/*
** runtime.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul 30 23:34:27 2017 CASTELLARNAU Aurelien
** Last update Thu Sep 28 17:01:54 2017 BILLAUD Jean
*/

#include <json/json.h>
#include <pthread.h>
#include "libmy.h"
#include "game_manager.h"
#include "softwar_ctx.h"
#include "player.h"
#include "command.h"
#include "hash.h"
#include "pub.h"
#include "poll.h"
#include "router.h"
#include "exec.h"
#include "runtime.h"
#include "thread.h"
#include "thread_data.h"

int		init_network(t_swctx **ctx)
{
  t_swsock	*pub;
  t_swsock	*router;
 
  // pub/sub socket
  pub = create_socket("pub", init_pub((*ctx)->pub_port));
  if (add_link(&((*ctx)->sockets), pub))
    return (1);
  // req/router socket
  router = create_socket("router", init_router((*ctx)->rep_port));
  if (add_link(&((*ctx)->sockets), router))
    return (1);
  return (0);
}

/*
** Boucle de jeu
*/
int		serve_game(t_swctx **ctx, t_game_manager **manager)
{
  char		log[80];
  
  char		*ret;
  zframe_t	*address;
  char		*input;
  pthread_t 	tic;
  t_thread	*t;
  
  t_game_info	**gi;
  zmsg_t	*response;

  /*t_command	**commands;
  t_energy_cell	*ec;
  t_player	*player;
  t_player	*adversary;
  t_chain	*players;*/
  
  gi = (*manager)->get_info();  
  t = init_thread(*ctx, *gi);
  
  sprintf(log, "after init thread");
  my_log(__func__, log, 3);
  
  if (pthread_create(&tic, NULL, tic_thread, t) == -1) {
    my_putstr("erroooooor");
    perror("pthread_create");
    return EXIT_FAILURE;
  }
  
  while (!zsys_interrupted)
    {
    if ((response = init_poll(ctx)) == NULL)
    return (1);
      address = zmsg_pop(response);
      if (((*ctx)->active_id = my_strdup(zmsg_popstr(response))) == NULL)
	return (1);
      input = zmsg_popstr(response);
      sprintf(log, "active id: %s, input: %s", (*ctx)->active_id, input);
      my_log(__func__, log, 3);
      if ((ret = exec(input, manager, (*ctx)->active_id)) == NULL)
	return (1);
      sprintf(log, "return: %s", ret);
      my_log(__func__, log, 3);
      zmsg_pushstr(response, ret);
      zmsg_pushstr(response, (*ctx)->active_id);
      zmsg_push(response, address);
      zmsg_send(&response, (*ctx)->active_socket->socket);
      my_log(__func__, "zmsg sent", 3);
    }
  return (0);
}

int			init_runtime()
{
  t_swctx		*ctx;
  t_game_manager	*manager;

  ctx = get_swctx();
  manager = get_game_manager();
  manager->set_map_size(ctx->size);
  if(init_network(&ctx) == 1) {
    my_putstr("init sockets faileds");
    return (0);
  }
  if (serve_game(&ctx, &manager))
    {
      my_log(__func__, "serving game failed", 1);
      return (1);
    }
  free_hashes();
  manager->free(); // free game_info static structure
  free(manager); // free the function pointer structure 'game_manager'
  free_ctx();
  return (0);
}
