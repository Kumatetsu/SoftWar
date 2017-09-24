/*
** exec.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Aug  2 21:58:43 2017 CASTELLARNAU Aurelien
** Last update Wed Aug  2 22:16:12 2017 CASTELLARNAU Aurelien
*/

#include "libmy.h"
#include "hash.h"
#include "game_manager.h"
#include "command.h"
#include "exec.h"

/*
** Méthode très basique pour l'instant
** possibilité d'ajouter une logique de validation
*/
char	**parse_input(char *input)
{
  char	**arr;

  if ((arr = my_split(input, '|')) == NULL)
    return (NULL);
  if (arr[1] == NULL
      && (arr[1] = my_strdup("no argument provide")) == NULL)
    return (NULL);
  return (arr);
}

/*
** paramètres: le char * contenant la commande (identify|identity)
**	       le t_game_manager (et donc la Game_info)
**	       l'id du player.
** fait appel aux méthode de hash.c pour récupérer la bonne
** fonction dans commands et l'exécuter.
*/
char		*exec(	char *input,
			t_game_manager **manager,
			char *active_id)
{
  char		log[50];
  int		hash;
  t_command	**commands;
  char		**inputs;
  char		*output;

  my_log(__func__, "start", 3);
  commands = get_commands();
  if ((inputs = parse_input(input)) == NULL)
    return (NULL);
  if ((hash = hash_command(inputs[0])) == HCOMMANDERR)
    my_log(__func__, "retrieve hash error", 2);
  if (hash != HCOMMANDUNKNOW)
    {
      sprintf(log, "execution of function %s", inputs[0]);
      my_log(__func__, log, 3);
      if ((output = commands[hash](manager, active_id, inputs[1])) == NULL)
	{
	  sprintf(log, "execution of function %s failed", inputs[0]);
	  my_log(__func__, log, 2);
	  if ((output = my_strdup("ko|server error")) == NULL)
	    return (NULL);
	  return (output);
	}
      my_log(__func__, output, 4);
      return (output);
    }  
  sprintf(log, "command %s doesn't exist", inputs[0]);
  my_log(__func__, log, 2);
  if ((output = my_strdup("ko|unknow command")) == NULL)
    return (NULL);
  return (output);
}
