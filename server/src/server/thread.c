/*
** thread.c for softwar in /home/naej/soft_war
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Thu Aug 17 17:00:01 2017 BILLAUD Jean
** Last update Thu Sep 14 17:47:27 2017 BILLAUD Jean
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <json/json.h>
#include "libmy.h"
#include "Player.h"
#include "Game_manager.h"
#include "Softwar_ctx.h"
#include "thread_data.h"

void		refresh_ap(t_game_info **info)
{
  t_link	*l;
  t_player	*p;
  
  l = (*info)->players->first;
  while (l)
    {
      p = ((t_player *)(l->content));
      p->action = 1;
      l = l->next;
    }

  return;
}

void 		*tic_thread(void *manager)
{
  t_thread	*thread = (t_thread *)(manager);
  zsock_t	*pub;
  json_object	*json;
  uint		cycle;

  srand(time(NULL));
  cycle = thread->ctx->cycle;
  pub = ((t_swsock *)(thread->ctx->sockets->first->content))->socket;
  my_put_nbr(cycle);
  while (!zsys_interrupted) {
    usleep(cycle);
    json = game_info_to_json(thread->info);
    zstr_sendf(pub, "%s %s", "Softwar", json_object_to_json_string(json));
    my_putstr(json_object_to_json_string(json));
    my_putstr("\n");
    energy_fall(thread->info->map_size);
    refresh_ap(&thread->info);
  }
  pthread_exit(NULL);
}

