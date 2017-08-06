/*
** energy_cell.h for Softwar in /home/naej/soft_war/SoftWar/server/include
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Tue Jul 11 16:55:28 2017 BILLAUD Jean
** Last update Wed Jul 12 16:45:16 2017 BILLAUD Jean
*/

#ifndef _ENERGY_CELL_H_
#define _ENERGY_CELL_H_

typedef struct  s_energy_cell
{
  unsigned int	x;
  unsigned int	y;
  unsigned int	value;
}		t_energy_cell;

t_energy_cell	*create_energy_cell(uint x, uint y, uint value);
void		free_energy_cell(t_energy_cell *ec);
void		free_energy_cells(t_chain **ecs);
json_object	*energy_cell_to_json(t_energy_cell *ec);
json_object	*energy_cells_to_json(t_chain *ecs);

#endif /* !_ENERGY_CELL_H_ */
