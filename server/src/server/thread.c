/*
** thread.c for softwar in /home/naej/soft_war
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Thu Aug 17 17:00:01 2017 BILLAUD Jean
** Last update Thu Sep 14 20:10:18 2017 BILLAUD Jean
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
#include "Enum.h"

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

int		dead(t_chain *players)
{
  t_link	*l;
  int		count;
  
  l = players->first;
  while (l)
    {
      if (((t_player*)(l->content))->energy <= 0) {
	count++;
	l = l->next;
	remove_link(&players, l->prev);
      }
    }
  return (count);
}

void 		*tic_thread(void *manager)
{
  t_thread	*thread = (t_thread *)(manager);
  zsock_t	*pub;
  json_object	*json;
  uint		cycle;
  int		dead_count;
  int		count;

  srand(time(NULL));
  count = 0;
  cycle = thread->ctx->cycle;
  pub = ((t_swsock *)(thread->ctx->sockets->first->content))->socket;
  zstr_sendf(pub, "%s %d", "Softwar", GAME_START);
  my_put_nbr(cycle);
  while (!zsys_interrupted) {
    usleep(cycle);
    dead_count = dead(thread->info->players);
    while(dead_count >= count) {
      zstr_sendf(pub, "%s %d", "Softwar", CLIENT_DEAD);
    }  
    json = game_info_to_json(thread->info);
    zstr_sendf(pub, "%s %d %s", "Softwar", CYCLE,json_object_to_json_string(json));
    my_putstr(json_object_to_json_string(json));
    my_putstr("\n");
    energy_fall(thread->info->map_size);
    refresh_ap(&thread->info);
    count = 0;
  }
   zstr_sendf(pub, "%s %d", "Softwar", GAME_END);
  pthread_exit(NULL);
}

