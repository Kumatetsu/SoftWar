/*
** map_manager.h for SoftWar in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Fri Aug 18 17:01:41 2017 BILLAUD Jean
** Last update Sun Aug 20 18:24:42 2017 BILLAUD Jean
*/

#ifndef  _MAP_MANAGER_H_
# define _MAP_MANAGER_H_

#include <json/json.h>

typedef struct	s_map_manager
{
  uint		(*is_free_square)();
  uint		(*is_wall)();
}		t_map_manager;

t_map_manager	*get_map_manager();

#endif /* !_GAME_INFO_H_ */

/**test **/
