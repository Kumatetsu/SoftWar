/*
** Map_manager.c for softwar in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Sat Aug 19 17:21:50 2017 BILLAUD Jean
** Last update Wed Sep  6 16:30:43 2017 BILLAUD Jean
*/

#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include <libmy.h>
#include <player.h>
#include <energy_cell.h>
#include <map_manager.h>

t_energy_cell	*is_energy_cell(uint x, uint y, t_chain *ecs)
{
  t_link	*te;
  t_energy_cell	*ec;

  if (ecs == NULL)
    {
      my_log(__func__, "ecs == null", 4);
      return (NULL);
    }
  te = ecs->first;
  while (te)
    {
      ec = (t_energy_cell*)(te->content); 
      if (ec->x == x && ec->y == y)
	return (ec);
      te = te->next;
    }
  return (NULL);
}

t_player	*is_player(uint x, uint y, t_chain *players)
{
  t_link	*tp;
  t_player	*p;

  if (players == NULL)
    {
      my_log(__func__, "player == null", 4);
      return (NULL);
    }
  tp = players->first;
  while (tp)
    {
      p = (t_player*)(tp->content);
      if (p->x == x && p->y == y)
	return (p);
      tp = tp->next;
    }
  return (NULL);
}

/*
** ci-dessous les is_*
*/
int	is_free_square(uint x, uint y, t_chain *players, t_chain *ecs)
{
  int	is_free;

  is_free = (is_energy_cell(x, y, ecs) == NULL && is_player(x, y, players) == NULL) ? 1 : 0;
  return (is_free);
}

int	is_wall(uint x, uint y, uint map_size)
{
  if (x > map_size || y > map_size)
    return (1);
  return (0);
}


t_map_manager		*get_map_manager()
{
  static t_map_manager	*manager;

  if (manager == NULL)
    {
      if ((manager = malloc(sizeof (*manager))) == NULL)
	{
	  my_log(__func__, MEM_ERR, 1);
	  return (NULL);
	}
      manager->is_player = &is_player;
      manager->is_energy_cell = &is_energy_cell;
      manager->is_free_square = &is_free_square;
      manager->is_wall = &is_wall;
    }
  return (manager);
}
