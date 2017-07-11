/*
** user.h for Softwar in /home/naej/soft_war/SoftWar/server/include
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Tue Jul 11 16:37:52 2017 BILLAUD Jean
** Last update Tue Jul 11 17:00:19 2017 BILLAUD Jean
*/

#ifndef _PLAYER_H
#define _PLAYER_H

typedef struct s_player
{
  char	*identity;
  uint	x;
  uint	y;
  uint	energy;
  uint	looking;
}	t_player;

#endif /* !_PLAYER_H */
