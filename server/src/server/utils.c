/*
** utils.c for SW in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Mon Sep  4 20:05:35 2017 BILLAUD Jean
** Last update Wed Sep  6 16:31:08 2017 BILLAUD Jean
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
  char		log[50];
  if (map->is_wall(x, y, map_size) == 1) {
    sprintf(log, "%s tried to go in a wall", identity);
    return (1);
  } else if (map->is_free_square(x, y, players, ecs) == 1) {
    sprintf(log, "%s tried to walk on something", identity);
    return (1);
  } else
    return (0);
}
