/*
** Notification.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Fri Jul  7 21:57:30 2017 CASTELLARNAU Aurelien
** Last update Sat Jul  8 01:01:11 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include <stdlib.h>
#include "libmy.h"
#include "Notification.h"

static char *notifications[] = { "cycle_info",
				 "game_started",
				 "game_finished",
				 "client_dead",
				 "client_win" };

char **get_notifications()
{
  return notifications;
}

void		display_notification(t_chain *parameters)
{
  t_link	*tmp;
  int           type;

  tmp = parameters->first;
  while (tmp)
    {
      type = my_getnbr((char*)tmp->content);
      if (type > 4)
	{
	  my_putstr_color("red", "\nargument of -proto option must be an in\
teger ");
	  my_putstr_color("red", "\nbetween 0 and 4 included.\n");
	}
      else
	{
	  my_putstr_color("blue", "\nCall of notification: ");
	  my_putstr_color("red", notifications[type]);
	  my_putstr("\n");
	}
      tmp = tmp->next;
    }
}
