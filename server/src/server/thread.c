/*
** thread.c for softwar in /home/naej/soft_war
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Thu Aug 17 17:00:01 2017 BILLAUD Jean
** Last update Fri Sep 15 17:03:38 2017 BILLAUD Jean
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <json/json.h>
#include "libmy.h"
#include "game_manager.h"
#include "thread_data.h"
#include "enum.h"

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
  count = 0;
  while (l)
    {
      if (((t_player*)(l->content))->energy <= 0) {
	count++;
	l = l->next;
	remove_link(&players, l->prev);
      } else {
	l = l->next;
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
  int		nb_player;

  srand(time(NULL));
  count = 0;
  cycle = thread->ctx->cycle;
  pub = ((t_swsock *)(thread->ctx->sockets->first->content))->socket;
  while (!zsys_interrupted) {
    usleep(cycle);
    /*
    ** là il va bombarder j'vais trouver un truc pour qu'il le fasse que quand ça passe de 0 à 1.
    */
    if (thread->info->game_status == 1) {
      zstr_sendf(pub, "%s %d", "Softwar", GAME_START);
    }
    dead_count = dead(thread->info->players);
    nb_player = (4 - dead_count);
    while (dead_count > count) {
      zstr_sendf(pub, "%s %d", "Softwar", CLIENT_DEAD);
      count++;
      my_putstr("client dead\n");
    }
    if (nb_player < 2)
      {
	 zstr_sendf(pub, "%s %d", "Softwar", CLIENT_WIN);
	 zstr_sendf(pub, "%s %d", "Softwar", GAME_END);
	 thread->info->game_status = 0;
	 my_putstr("client win, game is end\n");
      }
    json = game_info_to_json(thread->info);
    zstr_sendf(pub, "%s %d %s", "Softwar", CYCLE,json_object_to_json_string(json));
    my_putstr(json_object_to_json_string(json));
    my_putstr("\n");
    energy_fall(thread->info->map_size);
    refresh_ap(&thread->info);
    count = 0;
  }
  pthread_exit(NULL);
}

