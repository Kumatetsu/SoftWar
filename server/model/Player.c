/*
** Player.c for Softwar in /home/naej/soft_war/SoftWar/server/model
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Tue Jul 11 17:07:34 2017 BILLAUD Jean
** Last update Wed Jul 12 16:55:27 2017 BILLAUD Jean
*/

#include <stdio.h>
#include "libmy.h"
#include "Player.h"

/*
** Crée une structure player
** Set avec son nom et son
** energie.
 */
t_player	*create_player(char *identity)
{
  t_player	*p;

  if ((p = malloc(sizeof (*p))) == NULL)
    {
      my_log(__func__, MEM_ERR, 1);
      return (NULL);
    }
  if ((p->name = my_strdup(name)) == NULL)
    return (NULL);
  p->energy = 50;
  return (p);
}

void	free_player(t_player *p)
{
  free(p->name);
  free(p);
}
