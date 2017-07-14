/*
** Game_manager.h for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Jul 12 13:28:35 2017 CASTELLARNAU Aurelien
** Last update Wed Jul 12 14:25:08 2017 CASTELLARNAU Aurelien
*/

#ifndef  _GAME_MANAGER_H_
# define _GAME_MANAGER_H_

typedef struct	s_game_info
{
  unsigned int	map_size;
  unsigned int	game_status;
  t_chain	*players;
  t_chain	*energy_cells;
}		t_game_info;

typedef struct	s_game_manager
{
  t_game_info	*(*init)(uint, uint, t_chain*, t_chain*);
  void		(*free)();
  uint		(*get_map_size)();
  uint		(*get_game_status)();
  t_chain	*(*get_players)();
  t_chain	*(*get_energy_cells)();
  void		(*set_map_size)(uint map_size);
  void		(*set_game_status)(uint	game_status);
  void		(*set_players)(t_chain *players);
  void		(*set_energy_cells)(t_chain *ecs);
  t_game_info	*(*get_info)();
  json_object	*(*serialize)(void);
}		t_game_manager;

t_game_manager	*get_game_manager();

#endif /* !_GAME_INFO_H_ */
