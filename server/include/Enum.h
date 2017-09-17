/*
** Enum.h for softwar in /home/naej/soft_war/Soft_war/server/include
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Mon Aug 21 17:13:47 2017 BILLAUD Jean
** Last update Thu Sep 14 17:29:39 2017 BILLAUD Jean
*/

#ifndef  _ENUM_H_
# define _ENUM_H_

enum position {
  LEFT = 0,
  UP = 1,
  RIGHT = 2,
  DOWN = 3,
};

enum NotificationType {
  CYCLE = 0,
  GAME_START = 1,
  GAME_END = 2,
  CLIENT_DEAD = 3,
  CLIENT_WIN = 4
};
#endif /* !_ENUM_H_ */
