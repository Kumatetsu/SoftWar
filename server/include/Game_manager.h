/*
** Game_manager.h for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Jul 12 13:28:35 2017 CASTELLARNAU Aurelien
** Last update Wed Sep 13 19:21:48 2017 BILLAUD Jean
*/

#ifndef  _GAME_MANAGER_H_
# define _GAME_MANAGER_H_

#include <json/json.h>
#include <czmq.h>
#include "Player.h"
#include "Map_manager.h"
#include "Softwar_ctx.h"

typedef struct	s_game_info
{
  unsigned int	map_size;
  unsigned int	game_status;
  t_chain	*players;
  t_chain	*energy_cells;
  int		ready;
}		t_game_info;

typedef struct	s_game_manager
{
  t_game_info	**(*init)(uint, uint);
  void		(*free)();
  uint		(*get_map_size)();
  uint		(*get_game_status)();
  t_chain	*(*get_players)();
  t_player	*(*get_player)(char *identity);
  t_chain	*(*get_energy_cells)();
  void		(*set_map_size)(uint map_size);
  void		(*set_game_status)(uint	game_status);
  int		(*add_player)(t_player *player);
  void		(*set_energy_cells)(t_chain *ecs);
  void		(*set_players_pos)(t_chain *players, uint map_size);
  void		(*energy_fall)(uint map_size);
  t_game_info	**(*get_info)();
  t_swctx	*(*get_swctx)();
  t_map_manager *(*map_manager)();
  json_object	*(*serialize)(t_game_info *info);
  int		ready;
}		t_game_manager;

t_game_manager	*get_game_manager();
json_object	*game_info_to_json(t_game_info *info);

#endif /* !_GAME_INFO_H_ */
