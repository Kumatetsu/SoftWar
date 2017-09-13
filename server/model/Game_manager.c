/*
** Game_manager.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Jul 12 13:50:33 2017 CASTELLARNAU Aurelien
** Last update Wed Sep 13 22:49:50 2017 BILLAUD Jean
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include "libmy.h"
#include "Player.h"
#include "Softwar_ctx.h"
#include "Energy_cell.h"
#include "Map_manager.h"
#include "Game_manager.h"
#include "pub.h"
#include "utils.h"

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

void			set_players_pos(t_chain *players, uint map_size)
{
  t_link		*node_player;
  t_player		*player;
  int			i;
  int 			x[4] = {map_size, 0, map_size, 0};
  int			y[4] = {map_size, map_size, 0, 0};

  i = 0;
  node_player = (players->first);
  while (node_player) {
    player = (t_player*)node_player->content;
    player->x = x[i];
    player->y = y[i];
    i++;
    node_player = node_player->next;
  }
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

void		energy_fall(uint map_size)
{
  int		x;
  int		y;
  int		power;
  t_game_info	**game_info;
  t_energy_cell	*ecs;
  t_map_manager *map;

  map = get_map_manager();
  game_info = get_info();
  x = rand() % (map_size + 1);
  y = rand() % (map_size + 1);
  power = rand() % (16 - 5) + 5;
  if (count_ecs((*game_info)->energy_cells, map_size) == 0)
    {
      if (map->is_free_square(x, y, (*game_info)->players, (*game_info)->energy_cells) == 0) {
	if ((ecs = create_energy_cell(x, y, power)) == NULL)
	  return;
	if (add_link(&((*game_info)->energy_cells), ecs))
	  return;
      } else {
	energy_fall(map_size);
      }
    }
  return;
}

int		add_player(t_player *player)
{
  t_game_info   **game_info;
  game_info = get_info();
  if (add_link(&((*game_info)->players), player))
    return (1);
  return (0);
}

/**
 ** Pas convaincu par cette fonction, on peut faire pop qu'un cell d'energie
 ** par round, on aura jamais à remaplce la t_chain créer à l'init du game, juste à add ou delete des energy cells...
 */
void		set_energy_cells(t_chain *ecs)
{
  t_game_info   **game_info;
  
  game_info = get_info();
  (*game_info)->energy_cells = ecs;
}

json_object	*game_info_to_json(t_game_info *info)
{
  json_object	*game_info_json;
  json_object	*map_size_json;
  json_object	*game_status_json;
  json_object	*players_json;
  json_object	*energy_cells_json;
  t_game_info   **game_info;
  
  game_info		= &info;
  game_info_json	= json_object_new_object();
  map_size_json		= json_object_new_int((*game_info)->map_size);
  game_status_json	= json_object_new_int((*game_info)->game_status);
  players_json		= players_to_json((*game_info)->players);
  energy_cells_json	= energy_cells_to_json((*game_info)->energy_cells);

  json_object_object_add(game_info_json, "map_size", map_size_json);
  json_object_object_add(game_info_json, "game_status", game_status_json);
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
      manager->init		 = &init_game_info;
      manager->free		 = &free_game_info;
      manager->get_info		 = &get_info;
      manager->get_map_size	 = &get_map_size;
      manager->get_game_status	 = &get_game_status;
      manager->get_player	 = &get_player;
      manager->get_players	 = &get_players;
      manager->get_energy_cells  = &get_energy_cells;
      manager->set_map_size	 = &set_map_size;
      manager->set_game_status	 = &set_game_status;
      manager->set_players_pos   = &set_players_pos;
      manager->add_player	 = &add_player;
      manager->set_energy_cells  = &set_energy_cells;
      manager->energy_fall       = &energy_fall;
      manager->serialize	 = &game_info_to_json;
      manager->map_manager       = &get_map_manager;
      manager->get_swctx         = &get_swctx;
      manager->ready		 = 0;
    }
  return (manager);
}
