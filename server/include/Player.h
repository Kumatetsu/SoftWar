/*
** user.h for Softwar in /home/naej/soft_war/SoftWar/server/include
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Tue Jul 11 16:37:52 2017 BILLAUD Jean
** Last update Wed Jul 12 16:45:44 2017 BILLAUD Jean
*/

#ifndef _PLAYER_H
#define _PLAYER_H

typedef struct	s_player
{
  char		*identity;
  uint		x;
  uint		y;
  uint		energy;
  uint		looking;
}		t_player;

json_object	*player_to_json(t_player *player);
json_object	*players_to_json(t_chain *players); 

#endif /* !_PLAYER_H */
