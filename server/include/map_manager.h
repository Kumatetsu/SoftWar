/*
** map_manager.h for SoftWar in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Fri Aug 18 17:01:41 2017 BILLAUD Jean
** Last update Sun Sep 24 13:13:03 2017 BILLAUD Jean
*/

#ifndef  _MAP_MANAGER_H_
# define _MAP_MANAGER_H_

typedef struct	s_map_manager
{
  int		(*get_pos)(char *identity, t_chain *players);
  int		(*is_free_square)(uint x, uint y, t_chain *players, t_chain *ecs);
  int		(*is_wall)(int x, int y, int map_size);
  t_energy_cell	*(*is_energy_cell)(uint x, uint y, t_chain *ecs);
  t_player	*(*is_player)(uint x, uint y, t_chain *players);
}		t_map_manager;

t_energy_cell	*is_energy_cell(uint x, uint y, t_chain *ecs);
t_player	*is_player(uint x, uint y, t_chain *players);
t_map_manager	*get_map_manager();

#endif /* !_GAME_INFO_H_ */
