/*
** Game_manager.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Jul 12 13:50:33 2017 CASTELLARNAU Aurelien
** Last update Sat Sep 30 12:37:00 2017 MESRATI Nada
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include "libmy.h"
#include "softwar_ctx.h"
#include "game_manager.h"

t_player	*get_player(char *identity)
{
  t_chain	*players;
  t_link	*tmp;
  t_player	*player;
  char		log[80];
  t_game_info   **game_info;
  
  game_info = get_info();
  if ((players = (*game_info)->players) != NULL)
    {
      tmp = players->first;
      while (tmp)
	{
	  player = (t_player*)tmp->content;
	  if (!my_strcmp(identity, player->identity))
	    return (player);
	  tmp = tmp->next;
	}
      sprintf(log, "player with identity '%s' not found", identity);
      my_log(__func__, log, 2);
    }
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
  int 			x[4] = {(map_size - 1), 0, (map_size - 1), 0};
  int			y[4] = {(map_size - 1), (map_size - 1), 0, 0};
  int			looking[4] = {1, 1, 4, 4};

  i = 0;
  node_player = players->first;
  while (node_player) {
    player = (t_player*)node_player->content;
    player->x = x[i];
    player->y = y[i];
    player->looking = looking[i];
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

int		energy_fall(t_game_info **info)
{
  int		x;
  int		y;
  int		power;
  t_energy_cell	*ecs;
  t_map_manager *map;
  uint		map_size;
  
  if ((map = get_map_manager()) == NULL)
    return (1);
  map_size = (*info)->map_size;
  x = rand() % (map_size);
  y = rand() % (map_size);
  power = rand() % (16 - 5) + 5;
  if (count_ecs((*info)->energy_cells, map_size) == 0)
    {
      if (map->is_energy_cell(x, y, (*info)->energy_cells) == NULL)
	{
	  if ((ecs = create_energy_cell(x, y, power)) == NULL)
	    return (1);
	  if (add_link(&((*info)->energy_cells), ecs))
	    return (1);
	}
      else
	energy_fall(info);
    }
  return (0);
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
