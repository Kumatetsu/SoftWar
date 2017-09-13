/*
** thread_data.h for software in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Wed Sep 13 16:01:51 2017 BILLAUD Jean
** Last update Wed Sep 13 16:22:52 2017 BILLAUD Jean
*/

#ifndef  _THREAD_DATA_H_
# define _THREAD_DATA_H_

#include "libmy.h"
#include "Game_manager.h"
#include "Softwar_ctx.h"

typedef struct s_thread_data
{
  t_game_info	*info;
  t_swctx	*ctx;
}		t_thread;

t_thread	*init_thread(t_swctx *ctx, t_game_info *info);

#endif /* !_THREAD_DATA_H_ */
