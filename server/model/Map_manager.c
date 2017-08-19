/*
** Map_manager.c for softwar in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Sat Aug 19 17:21:50 2017 BILLAUD Jean
** Last update Sat Aug 19 22:16:34 2017 BILLAUD Jean
*/

#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>

/*
** ci dessous les get_*
*/
int		get_pos(char *identity, t_chain *players)
{
  int		pos[2];
  t_player	*player;

  player = players->first;
  while (player->next != NULL)
    {
      if (my_strcmp(identity, player->identity == 0))
	{
	  pos[1] = player->x;
	  pos[2] = player->y;
	  return (pos);
	}
      player = player->next;
    }
  return (0);
}

/*
** ci-dessous les is_*
*/
unsigned int		*is_free_square(t_chain *players, t_chain *ecs)
{
  
}




t_map_manager		*get_map_manager()
{
  static t_map_manager	*manager;

  if (manager == NULL)
    {
      if ((manager = mallo(sizeof (*manager))) == NULL)
	{
	  my_log(__func__, MEM_ERR, 1);
	  return (NULL);
	}
      manager->get_pos = &get_pos;
      manager->is_free_square = &is_free_square;
      manager->is_wall = &is_wall;
    }
  return (manager);
}
