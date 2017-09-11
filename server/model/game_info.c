/*
** Game_info.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Mon Aug 14 19:00:42 2017 CASTELLARNAU Aurelien
** Last update Mon Aug 14 19:02:29 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include <stdlib.h>
#include "libmy.h"
#include "energy_cell.h"
#include "player.h"
#include "game_info.h"

t_game_info             **init_game_info(unsigned int map_size,
					 unsigned int game_status)
{
  static t_game_info    *game_info;

  if (game_info == NULL)
    {
      if ((game_info = malloc(sizeof (*game_info))) == NULL)
	{
	  my_log(__func__, MEM_ERR, 1);
	  return (NULL);
	}
      if ((game_info->players = create_chain(free_players)) == NULL)
	return (NULL);
      if ((game_info->energy_cells = create_chain(free_energy_cells)) == NULL)
	return (NULL);
      game_info->map_size = map_size;
      game_info->game_status = game_status;
    }
  return (&game_info);
}

t_game_info     **get_info()
{
  t_game_info   **gi;

  if ((gi = init_game_info(0, 0)) == NULL)
    return (NULL);
  return (gi);
}


