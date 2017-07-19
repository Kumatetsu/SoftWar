/*
** Game_manager.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Jul 12 13:50:33 2017 CASTELLARNAU Aurelien
** Last update Wed Jul 12 14:16:41 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include "libmy.h"
#include "Player.h"
#include "Energy_cell.h"
#include "Game_manager.h"

static t_game_info *game_info = NULL;

t_game_info	*init_game_info(unsigned int map_size,
				unsigned int game_status,
				t_chain *players,
				t_chain *energy_cells)
{
  if ((game_info = malloc(sizeof (*game_info))) == NULL)
    {
      my_log(__func__, MEM_ERR, 1);
      return (NULL);
    }
  game_info->players = players;
  game_info->map_size = map_size;
  game_info->game_status = game_status;
  game_info->energy_cells = energy_cells;
  return (game_info);
}

t_game_info	*get_info()
{
  return (game_info);
}

uint		get_map_size()
{
  return (game_info->map_size);
}

uint		get_game_status()
{
  return (game_info->game_status);
}

t_chain		*get_players()
{
  return (game_info->players);
}

t_chain		*get_energy_cells()
{
  return (game_info->energy_cells);
}

void		set_map_size(uint map_size)
{
  game_info->map_size = map_size;
}

void		set_game_status(uint game_status)
{
  game_info->game_status = game_status;
}

void		set_players(t_chain *players)
{
  game_info->players = players;
}

void		set_energy_cells(t_chain *ecs)
{
  game_info->energy_cells = ecs;
}

json_object	*game_info_to_json()
{
  json_object	*game_info_json;
  json_object	*map_size_json;
  json_object	*game_status_json;
  json_object	*players_json;
  json_object	*energy_cells_json;

  game_info_json	= json_object_new_object();
  map_size_json		= json_object_new_int(game_info->map_size);
  game_status_json	= json_object_new_int(game_info->game_status);
  players_json		= players_to_json(game_info->players);
  energy_cells_json	= energy_cells_to_json(game_info->energy_cells);

  json_object_object_add(game_info_json, "map_size", map_size_json);
  json_object_object_add(game_info_json, "game_status", game_status_json);
  json_object_object_add(game_info_json, "players", players_json);
  json_object_object_add(game_info_json, "energy_cells", energy_cells_json);
  return (game_info_json);
}

void	free_game_info()
{
  if (game_info != NULL)
    {
      if (game_info->players != NULL)
	delete_chain(&(game_info->players));
      if (game_info->energy_cells != NULL)
	delete_chain(&(game_info->energy_cells));
      free(game_info);
    }
}

t_game_manager		*get_game_manager()
{
  static t_game_manager	*manager;

  if (manager == NULL)
    {
      if ((manager = malloc(sizeof (*manager))) == NULL)
	{
	  my_log(__func__, MEM_ERR, 1);
	  return (NULL);
	}
      manager->init		= &init_game_info;
      manager->free		= &free_game_info;
      manager->get_info		= &get_info;
      manager->get_map_size	= &get_map_size;
      manager->get_game_status	= &get_game_status;
      manager->get_players	= &get_players;
      manager->get_energy_cells = &get_energy_cells;
      manager->set_map_size	= &set_map_size;
      manager->set_game_status	= &set_game_status;
      manager->set_players	= &set_players;
      manager->set_energy_cells = &set_energy_cells;
      manager->serialize	= &game_info_to_json;
    }
  return (manager);
}
