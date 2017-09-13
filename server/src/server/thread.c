/*
** thread.c for softwar in /home/naej/soft_war
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Thu Aug 17 17:00:01 2017 BILLAUD Jean
** Last update Wed Sep 13 17:11:04 2017 BILLAUD Jean
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <json/json.h>
#include "libmy.h"
#include "Player.h"
#include "Game_manager.h"
#include "Softwar_ctx.h"
#include "thread_data.h"

void 		*tic_thread(void *manager)
{
  t_thread	*thread = (t_thread *)(manager);
  zsock_t	*pub;

  pub = ((t_swsock *)(thread->ctx->sockets->first->content))->socket;
  my_put_nbr(thread->info->map_size);
  while (!zsys_interrupted) {
    usleep(500000);
    zstr_sendf(pub, "%s", "Softwar toto");
    my_putstr("a");
  }
  pthread_exit(NULL);
}


