/*
** Map_manager.c for softwar in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Sat Aug 19 17:21:50 2017 BILLAUD Jean
** Last update Sun Aug 27 16:17:09 2017 BILLAUD Jean
*/

#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include <libmy.h>
#include <Player.h>
#include <Energy_cell.h>
#include <map_manager.h>

/*
** ci dessous les get_*, pas sur de le garder.... parce que on peut très bien se débrouiller 
** en envoyant x et y et ça sera moins relou que gérer un tableau^^

int		*get_pos(char *identity, t_chain *players)
{
  int		pos[2];
  t_link	*link;
  t_player	*player;
  
  link = players->first;
  while (link)
    {
      player = (t_player *)link->content;
      if (my_strcmp(identity, player->identity) == 0)
	{
	  pos[0] = player->x;
	  pos[1] = player->y;
	  return (pos);
	}
      link = link->next;
    }
  return (NULL);
}
*/

/*
** ci-dessous les is_*
*/
int		is_free_square(uint x, uint y, t_chain *players, t_chain *ecs)
{
  t_link	*lp;
  t_link	*le;
  t_player	*tp;
  t_energy_cell	*te;
  
  lp = players->first;
  le = ecs->first;

  while (lp)
    {
      /**
	 petit problème j'arrive pas à cast la struct et directement demandé l'élément de la struc .... si jamais t'as la syntaxe je suis preneur :p
       **/
      tp =(t_player *)lp->content;
      if (tp->x == x
	  && tp->y == y)
	return (0);
      lp = lp->next;
    }

  while (le)
    {
      te = (t_energy_cell *)le->content;
      if (te->x == x
	  && te->y == y)
	return (2);
      le = le->next;
    }

  return (1);
}

int	is_wall(uint x, uint y, uint map_size)
{
  if (x > map_size || y > map_size)
    return (0);
  return (1);
}


t_map_manager		*get_map_manager()
{
  static t_map_manager	*manager;

  if (manager == NULL)
    {
      if ((manager = malloc(sizeof (*manager))) == NULL)
	{
	  my_log(__func__, MEM_ERR, 1);
	  return (NULL);
	}
      /*manager->get_pos = &get_pos;*/
      manager->is_free_square = &is_free_square;
      manager->is_wall = &is_wall;
    }
  return (manager);
}
