/*
** Map_manager.c for softwar in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Sat Aug 19 17:21:50 2017 BILLAUD Jean
** Last update Mon Sep  4 17:44:00 2017 BILLAUD Jean
*/

#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include <libmy.h>
#include <Player.h>
#include <Energy_cell.h>
#include <map_manager.h>

/*
** ci-dessous les is_*
*/
int		is_free_square(uint x, uint y, t_chain *players, t_chain *ecs)
{
  t_link	*tp;
  t_link	*te;

  tp = players->first;
  te = ecs->first;
  
  while (tp)
    {
      if (((t_player *)(tp->content))->x == x
	  && ((t_player *)(tp->content))->y == y)
	return (0);
      tp = tp->next;
    }

  while (te)
    {
      if (((t_energy_cell *)(te->content))->x == x
	  && ((t_energy_cell *)(te->content))->y == y)
	return (2);
      te = te->next;
    }

  return (1);
}

int	is_wall(uint x, uint y, uint map_size)
{
  if (x > map_size || y > map_size)
    return (0);
  return (1);
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
      manager->is_free_square = &is_free_square;
      manager->is_wall = &is_wall;
    }
  return (manager);
}
