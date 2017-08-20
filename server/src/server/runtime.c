/*
** runtime.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul 30 23:34:27 2017 CASTELLARNAU Aurelien
** Last update Sat Aug 19 22:18:07 2017 BILLAUD Jean
*/

#include <json/json.h>
#include "libmy.h"
#include "Game_manager.h"
#include "Softwar_ctx.h"
#include "Player.h"
#include "command.h"
#include "hash.h"
#include "pub.h"
#include "poll.h"
#include "router.h"
#include "exec.h"
#include "runtime.h"

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
  zmsg_t	*response;
  zframe_t	*address;
  char		*input;
  
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
      /*
      ** La fonction exec() est chargée  de l'exécution des
      ** commandes. C'est dans exec que ce fait le
      ** traitement de l'input (parsing, hash,
      ** modif de manager et donc de game info).
      */
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
  manager->set_map_size(ctx->size); // muahah
  init_network(&ctx);
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
