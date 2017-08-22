/*
** Game_manager.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Jul 12 13:50:33 2017 CASTELLARNAU Aurelien
** Last update Tue Aug 22 20:37:55 2017 BILLAUD Jean
*/

#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include "libmy.h"
#include "Player.h"
#include "Energy_cell.h"
#include "Game_manager.h"

t_game_info		**init_game_info(unsigned int map_size,
					unsigned int game_status)
{
  static t_game_info	*game_info;

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

t_game_info	**get_info()
{
  t_game_info	**gi;

  if ((gi = init_game_info(0, 0)) == NULL)
    return (NULL);
  return (gi);
}

uint		get_map_size()
{
  t_game_info  **game_info;

  game_info = get_info();
  return ((*game_info)->map_size);
}

uint		get_game_status()
{
  t_game_info  **game_info;

  game_info = get_info();
  return ((*game_info)->game_status);
}

t_player	*get_player(char *identity)
{
  t_link	*tmp;
  t_player	*player;
  char		log[80];
  t_game_info   **game_info;
  
  game_info = get_info();
  tmp = (*game_info)->players->first;
  while (tmp)
    {
      player = (t_player*)tmp->content;
      if (!my_strcmp(identity, player->identity))
	return (player);
      tmp = tmp->next;
    }
  sprintf(log, "player with identity '%s' not found", player->identity);
  my_log(__func__, log, 2);
  return (NULL);
}

t_chain		*get_players()
{
  t_game_info   **game_info;
  
  game_info = get_info();
  return ((*game_info)->players);
}

void			set_players_pos(t_chain *players, uint map)
{
  t_link		*node_player;
  int 			x[4] = {0, map, 0, map};
  int			y[4] = {0, 0, map, map};
  
  node_player = (players->first);
}

t_chain		*get_energy_cells()
{
  t_game_info   **game_info;
  
  game_info = get_info();
  return ((*game_info)->energy_cells);
}

void		set_map_size(uint map_size)
{
  t_game_info   **game_info;
  
  game_info = get_info();
  (*game_info)->map_size = map_size;
}

void		set_game_status(uint game_status)
{
  t_game_info   **game_info;
  
  game_info = get_info();
  (*game_info)->game_status = game_status;
}

int		add_player(t_player *player)
{
  t_game_info   **game_info;
  
  game_info = get_info();
  if (add_link(&((*game_info)->players), player))
    return (1);
  return (0);
}

void		set_energy_cells(t_chain *ecs)
{
  t_game_info   **game_info;
  
  game_info = get_info();
  (*game_info)->energy_cells = ecs;
}

json_object	*game_info_to_json()
{
  json_object	*game_info_json;
  json_object	*map_size_json;
  json_object	*game_status_json;
  json_object	*players_json;
  json_object	*energy_cells_json;
  t_game_info   **game_info;
  
  game_info		= get_info();
  game_info_json	= json_object_new_object();
  map_size_json		= json_object_new_int((*game_info)->map_size);
  game_status_json	= json_object_new_int((*game_info)->game_status);
  if ((*game_info)->players != NULL && (*game_info)->players->first != NULL)
    players_json		= players_to_json((*game_info)->players);
  energy_cells_json	= energy_cells_to_json((*game_info)->energy_cells);

  json_object_object_add(game_info_json, "map_size", map_size_json);
  json_object_object_add(game_info_json, "game_status", game_status_json);
  if ((*game_info)->players != NULL && (*game_info)->players->first != NULL)
    json_object_object_add(game_info_json, "players", players_json);
  json_object_object_add(game_info_json, "energy_cells", energy_cells_json);
  return (game_info_json);
}

void		free_game_info()
{
  t_game_info   **game_info;
  
  game_info = get_info();
  if (game_info != NULL && (*game_info) != NULL)
    {
      if ((*game_info)->players != NULL)
	delete_chain(&((*game_info)->players));
      if ((*game_info)->energy_cells != NULL)
	delete_chain(&((*game_info)->energy_cells));
      free((*game_info));
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
      manager->get_player	= &get_player;
      manager->get_players	= &get_players;
      manager->get_energy_cells = &get_energy_cells;
      manager->set_map_size	= &set_map_size;
      manager->set_game_status	= &set_game_status;
      manager->set_players_pos  = &set_players_pos;
      manager->add_player	= &add_player;
      manager->set_energy_cells = &set_energy_cells;
      manager->serialize	= &game_info_to_json;
      manager->ready		= 0;
    }
  return (manager);
}
