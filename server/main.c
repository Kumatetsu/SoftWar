/*
** main.c for SoftWar in /home/code/SoftWar/aurelien
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sat Jul  1 01:11:51 2017 CASTELLARNAU Aurelien
** Last update Sat Jul  1 02:29:20 2017 CASTELLARNAU Aurelien
*/

#include "libmy.h"
#include "argument.h"
#include "runtime.h"

int		main(int argc, char *argv[])
{
  if (sw_parse(argc, argv))
    my_log(__func__, "failed to parse SoftWar arguments", 2);
  if (init_runtime())
    my_log(__func__, "runtime error", 1);
  delete_logger();
  return(0);
}
