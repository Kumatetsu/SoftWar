/*
** Energy_cell.c for Softwar in /home/naej/soft_war/SoftWar/server/model
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Tue Jul 11 17:41:31 2017 BILLAUD Jean
** Last update Fri Aug 18 16:37:36 2017 BILLAUD Jean
*/

#include <stdio.h>
#include <json/json.h>
#include "libmy.h"
#include "Energy_cell.h"

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

void    free_energy_cell(t_energy_cell *e)
{
  if (e != NULL)
    free(e);
}

void		free_energy_cells(t_chain **ecs)
{
  t_link	*tmp;
  t_energy_cell	*ec;

  if (!chain_is_null(ecs))
    {
      tmp = (*ecs)->first;
      while (tmp)
	{
	  ec = tmp->content;
          free_energy_cell(ec);
	  tmp = tmp->next;
	}
    }
}

json_object	*energy_cell_to_json(t_energy_cell *ec)
{
  json_object	*ec_json;
  json_object   *x_json;
  json_object   *y_json;
  json_object   *value_json;

  ec_json	= json_object_new_object();
  x_json        = json_object_new_int(ec->x);
  y_json        = json_object_new_int(ec->y);
  value_json	= json_object_new_int(ec->value);
  json_object_object_add(ec_json, "x", x_json);
  json_object_object_add(ec_json, "y", y_json);
  json_object_object_add(ec_json, "value", value_json);
  return (ec_json);
}

json_object	*energy_cells_to_json(t_chain *ecs)
{
  t_link	*tmp;
  t_energy_cell	*ec;
  json_object	*ecs_json;

  ecs_json = json_object_new_array();
  tmp = ecs->first;
  while (tmp)
    {
      ec = tmp->content;
      json_object_array_add(ecs_json, energy_cell_to_json(ec));
      tmp = tmp->next;
    }
  return (ecs_json);
}
