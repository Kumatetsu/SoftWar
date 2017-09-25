/*
** map.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Mon Aug 14 19:05:59 2017 CASTELLARNAU Aurelien
** Last update Mon Aug 14 19:07:52 2017 CASTELLARNAU Aurelien
*/

#include "libmy.h"
#include "game_info.h"
#include "map.h"

/*
** return the size of the map
** as define in parsing argument process
** return 0 if failed.
*/
uint            get_map_size()
{
  t_game_info  **game_info;

  if ((game_info = get_info()) == NULL)
    return (0);
  return ((*game_info)->map_size);
}

/*
** set the size of the map into game_info
** return 0 if ok
** return 1 if failed.
*/
int		set_map_size(uint size)
{
  t_game_info	**game_info;

  if ((game_info = get_info()) == NULL)
    return (1);
  (*game_info)->map_size = size;
  return (0);
}
