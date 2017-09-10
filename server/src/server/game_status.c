/*
** game_status.c for SoftWar in
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Mon Aug 14 19:21:40 2017 CASTELLARNAU Aurelien
** Last update Mon Aug 14 19:29:52 2017 CASTELLARNAU Aurelien
*/

#include "libmy.h"
#include "game_status.h"

/*
** return the game status
** !!!! ERROR MANAGEMENT TO FIX WITH POSSIBLE VALUES NOT TAKEN BY GAME STATUS!!!!!
*/
uint		get_game_status()
{
  int		error_return_to_define;
  t_game_info	**game_info;

  error_return_to_define = 1234559;
  if ((game_info = get_info()) == NULL)
    return (error_return_to_define);
  return ((*game_info)->game_status);
}

/*
** allow to set the game status
** return 0 if ok,
** return 1 if failed.
*/
int		set_game_status(uint game_status)
{
  t_game_info	**game_info;

  if ((game_info = get_info()) == NULL)
    return (1);
  (*game_info)->game_status = game_status;
  return (0);
}

