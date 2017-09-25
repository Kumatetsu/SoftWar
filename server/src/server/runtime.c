/*
** runtime.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul 30 23:34:27 2017 CASTELLARNAU Aurelien
** Last update Mon Sep 25 23:39:26 2017 BILLAUD Jean
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
  //pthread_t 	tic;
  //t_thread	*t;
  
  t_game_info	**gi;
  zmsg_t	*response;

  t_command	**commands;
  t_energy_cell	*ec;
  t_player	*player;
  t_player	*adversary;
  t_chain	*players;
  
  identify(manager, "foo", NULL);
  identify(manager, "foo", NULL);
  identify(manager, "foo", NULL);
  identify(manager, "foo", NULL);
  gi = (*manager)->get_info();
  players = (*manager)->get_players();

  (*manager)->set_players_pos(players, 4);
  
  /*
  ** pour init le hashage des commandes
  */
  commands = get_commands();
  
  if (commands == NULL)
    my_log(__func__, "im a shitty action to imply commands and compile", 4);
  /*
  ** on crée un player, on le place en x2y0
  ** on le fait regarder en bas  
  */
  if ((player = (*manager)->get_player("0x01")) == NULL)
    {
      my_log(__func__, "fail retrieving player by id 0x01", 4);
      return (1);
    }
  player->looking = 3;
  player->x = 2;
  player->action = 5000; 
  /*
  ** on ajoute une energy cell de value 2017 juste en face de lui
  ** position 1 dans le tableau de watch
  */
  if ((ec = create_energy_cell(2, 1, 2017)) == NULL)
    {
      my_log(__func__, "fail creating energy cell", 4);
      return (1);
    }
  if (add_link(&((*gi)->energy_cells), ec))
    {
      my_log(__func__, "failed adding energy cell in front of player", 4);
      return (1);
    }
  /*
  ** on déplace un joueur en diagonale droite de lui
  ** position 4 dans le tableau attendu après watch
  */
  if ((adversary = (*manager)->get_player("0x02")) == NULL)
    {
      my_log(__func__, "failed retrieving adversary with id 0x02!", 4);
      return (1);
    }
  adversary->x = 1;
  adversary->y = 2;
  /*
  ** watch: on veut ["energy", "empty", "empty", "0x02"]
  */
  my_log(__func__, "before call to exec", 4);
  if ((ret = exec("watch", manager, "0x01")) == NULL)
    return (1);
  my_log(__func__, "call to exec passed", 4);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);

  /*
  ** on veut ok|
  */
  my_log(__func__, "before call to exec", 4);
  if ((ret = exec("forward", manager, "0x01")) == NULL)
    return (1);
  my_log(__func__, "call to exec passed", 4);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);

  /*
  ** on veut 50 sur le premier log
  */
  sprintf(log, "Energy possessed by player before gather: %d", player->energy);
  my_log(__func__, log, 4);
  my_log(__func__, "before call to exec", 4);
  if ((ret = exec("gather", manager, "0x01")) == NULL)
    return (1);
  my_log(__func__, "call to exec passed", 4);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  /*
  ** on veut 50 + 2017 = 2067
  */
  sprintf(log, "Energy possessed by player after gather: %d", player->energy);
  my_log(__func__, log, 4);
  /*
  ** les déplacements, on va lui faire contourner 0x02 en passant par x3
  ** soit leftfwd, rightfwd, right, forward et watch...
  */

  
  player->action = 5000;
  sprintf(log, "ORIGINAL position of player: x: %d, y: %d", player->x, player->y);
  my_log(__func__, log, 3);
  my_log(__func__, "before call to exec", 4);
  if ((ret = exec("leftfwd", manager, "0x01")) == NULL)
    return (1);
  sprintf(log, "new position of player: x: %d, y: %d vue: %d", player->x, player->y, player->looking);
  my_log(__func__, log, 3);
  my_log(__func__, "call to exec passed", 4);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
my_log(__func__, "before call to exec", 4);
 sprintf(log, "position: x: %d, y: %d looking: %d before rightfwd", player->x, player->y, player->looking);
  my_log(__func__, log, 4);
 
  if ((ret = exec("rightfwd", manager, "0x01")) == NULL)
    return (1);
  sprintf(log, "new position of player: x: %d, y: %d looking: %d after rightfw", player->x, player->y, player->looking);
  my_log(__func__, log, 3);
  my_log(__func__, "call to exec passed", 4);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  my_log(__func__, log, 3);
  my_log(__func__, "before call to exec", 4);
  if ((ret = exec("right", manager, "0x01")) == NULL)
    return (1);
  sprintf(log, "new position of player: x: %d, y: %d", player->x, player->y);
  my_log(__func__, log, 3);
  my_log(__func__, "call to exec passed", 4);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  my_log(__func__, "before call to exec", 4);
  if ((ret = exec("forward", manager, "0x01")) == NULL)
    return (1);
  sprintf(log, "new position of player: x: %d, y: %d", player->x, player->y);
  my_log(__func__, log, 3);
  my_log(__func__, "call to exec passed", 4);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  /*
  ** On veut ["0x02", "empty", "empty", empty"]
  */
  my_log(__func__, "before call to exec", 4);
  if ((ret = exec("watch", manager, "0x01")) == NULL)
    return (1);
  my_log(__func__, "call to exec passed", 4);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  sprintf(log, "new position of player: x: %d, y: %d", player->x, player->y);
  my_log(__func__, log, 3);

  /*
  ** le spy shit
  */
  my_log(__func__, "before call to exec", 4);
  if ((ret = exec("inspect|0x02", manager, "0x01")) == NULL)
    return (1);
  my_log(__func__, "call to exec passed", 4);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  sprintf(log, "inspector energy: %d, action: %d, disabled: %d, adversary energy: %d, action: %d, disabled: %d", player->energy, player->action, player->disabled, adversary->energy, adversary->action, adversary->disabled);
  my_log(__func__, log, 3);

  /*
  ** le bourre pif
  */
  my_log(__func__, "before call to exec", 4);
  if ((ret = exec("attack", manager, "0x01")) == NULL)
    return (1);
  my_log(__func__, "call to exec passed", 4);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  sprintf(log, "attacker energy: %d, action: %d, disabled: %d, adversary energy: %d, action: %d, disabled: %d", player->energy, player->action, player->disabled, adversary->energy, adversary->action, adversary->disabled);
  my_log(__func__, log, 3);

  /*
  ** On veut x:0, y=2
  */
  my_log(__func__, "before call to exec for jump", 3);
  sprintf(log, "energy == %d", player->energy);
  my_log(__func__, log, 3);
  if ((ret = exec("jump", manager, "0x01")) == NULL)
    return (1);
  my_log(__func__, "call to exec passed for jump", 3);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  sprintf(log, "new position of player: x: %d, y: %d after jump", player->x, player->y);
  my_log(__func__, log, 3);
  sprintf(log, "energy == %d", player->energy);
  my_log(__func__, log, 3);
  /*
  ** on veut une erreur pour jump in wall donc un ko
  */
  my_log(__func__, "before call to exec for jump in wall", 3);
  if ((ret = exec("jump", manager, "0x01")) == NULL)
    return (1);
  my_log(__func__, "call to exec passed for jump in wall", 3);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  sprintf(log, "new position of player: x: %d, y: %d after jump", player->x, player->y);
  my_log(__func__, log, 3);

  /*
  ** on crée une erreur sur le saut sur un player
  */
  player->looking = 1;
  sprintf(log, "looking  %d", player->looking);
  my_log(__func__, log, 3);
  my_log(__func__, "before call to exec for jump on player adversary", 3);
  if ((ret = exec("jump", manager, "0x01")) == NULL)
    return (1);
  my_log(__func__, "call to exec passed for jump on adversary", 3);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  sprintf(log, "new position of player: x: %d, y: %d after jump on player", player->x, player->y);
  my_log(__func__, log, 4);

  /*
  ** on lui dit d'attendre le prochain tour on veut que ses points tombent à 0
  */
  sprintf(log, "action  %d", player->action);
  my_log(__func__, log, 3);
  my_log(__func__, "before call to exec for next", 3);
  if ((ret = exec("next", manager, "0x01")) == NULL)
    return (1);
  my_log(__func__, "call to exec passed for next", 3);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  sprintf(log, "player waiting next turn, action: %d", player->action);
  my_log(__func__, log, 3);

/*
  **on veut qu'il quite et qu'il ne fasse plus partie des joueurs
  */
  sprintf(log, "action  %d", player->action);
  my_log(__func__, log, 3);
  my_log(__func__, "before call to exec for leave", 3);
  if ((ret = exec("leave", manager, "0x02")) == NULL)
    return (1);
  my_log(__func__, "call to exec passed for leave", 3);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  sprintf(log, "player left");
  my_log(__func__, log, 3);

  /*
  **on veut que 01 selfstat
  */

  (*manager)->ready = 1;
  my_put_nbr(player->energy);
  my_putstr("\n\n\n");
  my_log(__func__, "before call to exec for selfstat", 3);
  if ((ret = exec("selfstats", manager, "0x01")) == NULL)
    return (1);
  my_log(__func__, "call to exec passed for selfstat", 3);
  sprintf(log, "return: %s", ret);
  my_log(__func__, log, 3);
  sprintf(log, "player left");
  my_log(__func__, log, 3);

  
  /*
  ** On passe aux actions sur adversaire dans le prochain...
  */
  
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
    
   /*t = init_thread(*ctx, *gi);
  if (pthread_create(&tic, NULL, tic_thread, t) == -1) {
    my_putstr("erroooooor");
    perror("pthread_create");
    return EXIT_FAILURE;
  }*/
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
