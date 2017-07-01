/*
** protocol.c for SoftWar in /home/code/SoftWar/aurelien/src
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sat Jul  1 01:22:04 2017 CASTELLARNAU Aurelien
** Last update Sat Jul  1 02:34:22 2017 CASTELLARNAU Aurelien
*/

#include "libmy.h"
#include "protocol.h"

static t_notification *notifications[] = { "infos de cycle",
						"Début de partie",
						"Fin de partie",
						"Mort",
						"Victoire" };

t_notification *get_notification(int type)
{
  return notifications[type];
}

void		display_notification(t_chain *parameters)
{
  t_link	*tmp;
  int		type;

  tmp = parameters->first;
  while (tmp)
    {
      type = my_getnbr((char*)tmp->content);
      if (type > 4)
	{
	  my_putstr_color("red", "\nargument of -proto option must be an integer ");
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
