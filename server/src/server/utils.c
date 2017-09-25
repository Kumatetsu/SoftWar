/*
** utils.c for SW in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Mon Sep  4 20:05:35 2017 BILLAUD Jean
** Last update Mon Sep 25 21:58:19 2017 BILLAUD Jean
*/


#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include "libmy.h"
#include "player.h"
#include "energy_cell.h"
#include "game_info.h"
#include "map_manager.h"
#include "game_manager.h"

int		check_mvmnt(uint x, uint y, char *identity, t_game_manager *manager)
{
  char		log[50];
  t_chain       *players;
  t_map_manager *map;
  int		map_size;

  players = manager->get_players();
  map = manager->map_manager();
  map_size = manager->get_map_size();
  if (map->is_wall(x, y, map_size) == 1)
    {
      sprintf(log, "%s tried to go in a wall", identity);
      my_log(__func__, log, 3);
      return (1);
    }
  else if (map->is_player(x, y, players) != NULL)
    {
      sprintf(log, "%s tried to walk on someone", identity);
      my_log(__func__, log, 3);
      return (1);
    }
  else
      return (0);
}

int		count_ecs(t_chain *ecs, uint map_size)
{
  t_link	*tmp;
  int		max;
  int		count;

  count = 0; 
  max = (map_size * map_size) - 4;
  tmp = ecs->first;
  while(tmp)
    {
      count++;
      tmp = tmp->next;
    }
  if (count < max)
    return (0);
  else
    return (1);
}

uint	**eq_watch(uint a, uint b, int operator_inv, int swap)
{
  uint	**coord;
  uint	swaper[2];
  uint	unit_swaper;
  int	i;
  int	x;
  int	y;
  int	z;

  
  x = (operator_inv) ? 1 : -1;
  y = (operator_inv) ? 2 : -2;
  z = (operator_inv) ? -1 : 1;
  coord = (uint **)malloc(4 * sizeof(uint *));
  for (i = 0; i < 4; i++)
    coord[i] = (uint *)malloc(2 * sizeof(uint));
  coord[0][0] = (a + x);
  coord[0][1] = b;
  coord[1][0] = (a + y);
  coord[1][1] = (b + z);
  coord[2][0] = (a + y);
  coord[2][1] = b;
  coord[3][0] = (a + y);
  coord[3][1] = (b + x);
  if (swap)
    {
      swaper[0] = coord[1][0];
      swaper[1] = coord[1][1];
      coord[1][0] = coord[3][0];
      coord[1][1] = coord[3][1];
      coord[3][0] = swaper[0];
      coord[3][1] = swaper[1];
      for (i = 0; i < 4; i++)
	{
	  unit_swaper = coord[i][0];
	  coord[i][0] = coord[i][1];
	  coord[i][1] = unit_swaper;
	}
    }
  return (coord);
}


/*
** ALLOCATE_MEMORY
*/
char		*compile_watch_return(t_game_manager **manager, uint **zone)
{
  int		it;
  char		*state;
  t_energy_cell	*ec;
  t_player	*p;

  if ((state = my_strdup("[")) == NULL)
    return (NULL);
  for (it = 0; it < 4; it++)
    {
      if ((state = my_strcat(state, "\"")) == NULL)
	return (NULL);
      if ((ec = is_energy_cell(zone[it][0], zone[it][1], (*manager)->get_energy_cells())) != NULL)
	{
	  if ((state = my_strcat(state, "energy")) == NULL)
	    return (NULL);
	}
      else if ((p = is_player(zone[it][0], zone[it][1], (*manager)->get_players())) != NULL)
	{
	  if ((state = my_strcat(state, p->identity)) == NULL)
	    return (NULL);
	}
      else
	{
	  if ((state = my_strcat(state, "empty")) == NULL)
	    return (NULL);
	}
      if (it < 3)
	{
	  if ((state = my_strcat(state, "\", ")) == NULL)
	    return (NULL);
	}
      else
	if ((state = my_strcat(state, "\"]")) == NULL)
	  return (NULL);
    }
  return (state);
}
