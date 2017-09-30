/*
** thread.c for softwar in /home/naej/soft_war
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Thu Aug 17 17:00:01 2017 BILLAUD Jean
** Last update Mon Sep 25 21:52:25 2017 BILLAUD Jean
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
      p->action = 10;
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

/*
** not tested ^^
*/
void		undisabledme(t_game_info **gi)
{
  t_link	*tmp;
  t_player	*p;

  tmp = (*gi)->players->first;
  while (tmp)
    {
      p = tmp->content;
      if (p->disabled > 0)
	p->disabled--;
      tmp = tmp->next;
    }
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
  char		output[1024];

  srand(time(NULL));
  count = 0;
  cycle = thread->ctx->cycle;
  pub = ((t_swsock *)(thread->ctx->sockets->first->content))->socket;
  if (thread == NULL)
    {
      my_log(__func__, "thread is null", 1);
      pthread_exit(NULL);
    }
  if (thread->info == NULL)
    {
      my_log(__func__, "thread info is null", 1);
      pthread_exit(NULL);
   }
  while (!zsys_interrupted)
    {
      usleep(cycle);
      undisabledme(&(thread->info));
      if (thread->info->game_status == 1)
	zstr_sendf(pub, "%s %d", "Softwar", GAME_START);
      dead_count = dead(thread->info->players);
      nb_player = (4 - dead_count);
      while (dead_count > count)
	{
	  zstr_sendf(pub, "%s %d", "Softwar", CLIENT_DEAD);
	  count++;
	  my_log(__func__, "client dead\n", 3);
	}
      if (nb_player < 2)
	{
	  zstr_sendf(pub, "%s %d", "Softwar", CLIENT_WIN);
	  zstr_sendf(pub, "%s %d", "Softwar", GAME_END);
	  thread->info->game_status = 0;
	  my_log(__func__, "client win, game is end\n", 3);
	}
      json = game_info_to_json(thread->info);
      sprintf(output, "%s %d %s", "Softwar", CYCLE, json_object_to_json_string(json));
      my_log(__func__, output, 5);
      zstr_sendf(pub, "%s %d %s", "Softwar", CYCLE, json_object_to_json_string(json));
      if (energy_fall(&thread->info))
	pthread_exit(NULL);
      refresh_ap(&thread->info);
      count = 0;
    }
  pthread_exit(NULL);
}
