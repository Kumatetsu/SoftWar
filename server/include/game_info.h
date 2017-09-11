/*
** Game_info.h for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Mon Aug 14 19:03:31 2017 CASTELLARNAU Aurelien
** Last update Mon Aug 14 19:04:50 2017 CASTELLARNAU Aurelien
*/

#ifndef  _GAME_INFO_H_
# define _GAME_INFO_H_

typedef struct  s_game_info
{
  unsigned int  map_size;
  unsigned int  game_status;
  t_chain       *players;
  t_chain       *energy_cells;
  int           ready;
}               t_game_info;

t_game_info	**init_game_info(uint map_size, uint game_status);
t_game_info	**get_info();

#endif   /* !_GAME_INFO_H_ */
