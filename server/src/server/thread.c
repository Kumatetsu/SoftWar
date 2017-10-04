/*
** thread.c for softwar in /home/naej/soft_war
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Thu Aug 17 17:00:01 2017 BILLAUD Jean
** Last update Sat Sep 30 15:21:40 2017 BILLAUD Jean
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

/*
** return management: 
** -1 : no more players
** -2: remove link failed, server error
** >0: count of dead people.  
*/
int		dead(t_chain *players)
{
  t_link	*l;
  t_player	*p;
  int		rm_link;
  int		count;
  
  l = players->first;
  count = 0;
  p = NULL;
  while (l)
    {
      p = (t_player*)(l->content);
      if (p == NULL)
	return (-2);
      if (p->energy <= 0 || p->energy > 100)
	{
	  count++;
	  l = l->next;
	  rm_link = remove_link(&players, l->prev);
	  if (rm_link == -1)
	    {
	      my_log(__func__, "no more players", 3);
	      return (-1);
	    }
	  else if (rm_link == 1)
	    {
	      my_log(__func__, "remove link failed", 4);
	      return (-2);
	    }
	  else
	    my_log(__func__, "client deleted", 4);
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
  char		output[1024];

  srand(time(NULL));
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
      if (dead_count == -2)
	pthread_exit(NULL);
      while (dead_count)
	{
	  zstr_sendf(pub, "%s %d", "Softwar", CLIENT_DEAD);
	  my_log(__func__, "client dead\n", 3);
	  --dead_count;
	}
      if (thread->info->players->index < 2 && thread->info->game_status)
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
    }
  pthread_exit(NULL);
}
