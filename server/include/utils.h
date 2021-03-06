/*
** utils.h for sw in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Mon Sep  4 20:38:50 2017 BILLAUD Jean
** Last update Wed Sep 13 22:26:14 2017 BILLAUD Jean
*/

#ifndef  _UTILS_H_
# define _UTILS_H_

int		check_mvmnt(uint x, uint y, char *identity, t_game_manager *manager);
int		count_ecs(t_chain *ecs, uint map_size);
uint		**eq_watch(uint a, uint b, int operator_inv, int swap);
char		*compile_watch_return(t_game_manager **manager, uint **zone);
char		*extract_front_from_watched(char *watched);

#endif /* !_UTILS_H_ */
