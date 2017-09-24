/*
** utils.c for SW in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Mon Sep  4 20:05:35 2017 BILLAUD Jean
** Last update Fri Sep 15 17:03:31 2017 BILLAUD Jean
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
  t_chain       *ecs;
  t_map_manager *map;
  int		map_size;
  
  players = manager->get_players();
  ecs = manager->get_energy_cells();
  map = manager->map_manager();
  map_size = manager->get_map_size();
  if (map->is_wall(x, y, map_size) == 1)
    {
      sprintf(log, "%s tried to go in a wall", identity);
      my_log(__func__, log, 4);
      return (1);
    }
  else if (map->is_free_square(x, y, players, ecs) == 1)
    {
      sprintf(log, "%s tried to walk on something", identity);
      my_log(__func__, log, 4);
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
    }
  return (coord);
}
