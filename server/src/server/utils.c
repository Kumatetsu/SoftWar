/*
** utils.c for SW in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Mon Sep  4 20:05:35 2017 BILLAUD Jean
** Last update Mon Sep  4 20:44:55 2017 BILLAUD Jean
*/


#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include "libmy.h"
#include "Softwar_ctx.h"
#include "Player.h"
#include "Energy_cell.h"
#include "Game_manager.h"
#include "hash.h"
#include "command.h"
#include "Enum.h"


int		check_mvmnt(uint x, uint y, t_map_manager *map, t_chain *players, t_chain *ecs, uint map_size, char *identity)
{
  switch (true)
    {
    case (map->is_wall(x, y, map_size) == 1):
      sprintf(log, "%s tried to go in a wall", identity);
      return (1);
    case (map->is_free_square(x, y, players, ecs) == 1):
      sprintf(log, "%s tried to walk on another player", identity);
      return (1);
    case (map->is_free_square(x, y, players, ecs) == 2):
      sprintf(log, "%s increased his power", identity);
      /*ici on va avoir besoin d'un add Energy to player + delete energy cell*/
      return (0);
    default:
      return (0);
    }
}
