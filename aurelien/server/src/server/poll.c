/*
** poll.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul  9 22:18:46 2017 CASTELLARNAU Aurelien
** Last update Sun Jul  9 22:26:44 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include <czmq.h>
#include "libmy.h"
#include "poll.h"

zpoller_t	*init_poll();
