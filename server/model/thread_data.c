/*
** thead_data.c for softwar in /home/naej/soft_war/Soft_war/server
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Wed Sep 13 16:01:38 2017 BILLAUD Jean
** Last update Wed Sep 13 16:23:36 2017 BILLAUD Jean
*/

#include <json/json.h>
#include <stdio.h>
#include <stdlib.h>
#include "libmy.h"
#include "game_manager.h"
#include "softwar_ctx.h"
#include "thread_data.h"

t_thread	*init_thread(t_swctx *ctx, t_game_info *info)
{
  t_thread	*thread;

  if ((thread = malloc(sizeof (*thread))) == NULL)
    {
      my_log(__func__, MEM_ERR, 1);
      return (NULL);
    }
  thread->info = info;
  thread->ctx = ctx;
  return (thread);
}
