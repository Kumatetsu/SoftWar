/*
** thread.c for softwar in /home/naej/soft_war
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Thu Aug 17 17:00:01 2017 BILLAUD Jean
** Last update Sun Sep 10 18:07:28 2017 BILLAUD Jean
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <json/json.h>
#include "libmy.h"
#include "Game_manager.h"
#include "Softwar_ctx.h"

void *tic_thread(void *arg)
{
  t_player	*players;

  players = (*manager)->get_players();
  while (!zsys_interrupted) {
    usleep(500000);
    
  }
  
  pthread_exit(NULL);
}


