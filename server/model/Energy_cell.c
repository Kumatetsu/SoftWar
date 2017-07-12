/*
** Energy_cell.c for Softwar in /home/naej/soft_war/SoftWar/server/model
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Tue Jul 11 17:41:31 2017 BILLAUD Jean
** Last update Wed Jul 12 16:53:30 2017 BILLAUD Jean
*/

#include <stdio.h>
#include "libmy.h"
#include "Energie_cell.h"

t_energy_cell	*create_energy_cell(unsigned int x, unsigned int y, unsigned int value)
{
  t_energy_cell	*e;

  if ((e = malloc(sizeof(*e))) == NULL)
    {
      my_log(__func__, MEM_ERR, 1);
      return (NULL);
    }
  e->x = x;
  e->y = y;
  e->value = value;
  return (e);
}

void	free_energy_cell(t_energy_cell *e)
{
  free(e);
}
