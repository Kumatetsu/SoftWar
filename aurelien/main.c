/*
** main.c for SoftWar in /home/code/SoftWar/aurelien
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sat Jul  1 01:11:51 2017 CASTELLARNAU Aurelien
** Last update Sat Jul  1 02:29:20 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include <stdlib.h>
#include "libmy.h"
#include "protocol.h"
#include "argument.h"

int		main(int argc, char *argv[])
{
  int		act;
  t_chain	*options;
  t_link	*tmp;
  t_option	*opt;

  act = 0;
  options = get_options();
  if (parse(1, argc, argv, &options))
    {
      devlog(__func__, "parsing arguments failed", 1);
      return (1);
    }
  tmp = options->first;
  while (tmp)
    {
      opt = (t_option*)tmp->content;
      if (opt->to_execute)
	{
	  act = 1;
	  execute(opt);
	}
      tmp = tmp->next;
    }
  if (!act)
    help();
  my_log("main", "Test log error", 1);
  my_log("main", "Test log warning", 2);
  my_log("main", "Test log info", 3);
  my_log("main", "Test log debug\n", 4);
  delete_logger();
  delete_chain(&options);
  return(0);
}
