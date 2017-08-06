/*
** argument.c for SoftWar in /home/code/SoftWar/aurelien/src
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sat Jul  1 01:39:47 2017 CASTELLARNAU Aurelien
** Last update Sat Jul  1 02:24:24 2017 CASTELLARNAU Aurelien
*/

#include <stdlib.h>
#include "libmy.h"
#include "argument.h"
#include "Softwar_ctx.h"
#include "Notification.h"

/*
** DONT FORGET TO COMPLETE IF ADD OPTION
*/
void		help()
{
  my_putstr_color("cyan", "\n----- HELP -----\n\n");
  my_putstr_color("cyan", "./demo [-v] ?debug|info|warning|error\n");
  my_putstr_color("cyan", "Default -v value is info\n");
  my_putstr_color("cyan", "Without verbose mode, log error in error.log\n");
  my_putstr_color("cyan", "./demo [-log] ![filepath], set a filepath for logs\n");
  my_putstr_color("cyan", "./demo [-proto] !int[int...4], display the differents notifications\n");
  my_putstr_color("cyan", "./demo [-rep-port] !int > 0 port for REP/REQ\n");
  my_putstr_color("cyan", "./demo [-pub-port] !int > 0 port for PUB/SUB\n");
  my_putstr_color("cyan", "./demo [-cycle] !int > 0 time in micro sec between two ticks\n");
}

/*
** On définit les options acceptées
** par le programme et comment elles
** doivent être gérées. 
** voir t_option et option_manager.c dans libmy
*/
t_chain		*get_options()
{
  t_chain	*options;
  t_option	*logger;
  t_option	*log_file;
  t_option	*protocol;
  t_option	*rep_port;
  t_option	*pub_port;
  t_option	*cycle;
  
  if ((options = create_chain(free_options_in_chain)) == NULL)
    {
      // devlog construit un logger temporaire
      // (avec build_default_logger() >> libmy/logger.c)
      // qui affiche avant l'instanciation du logger static
      // (avec build_logger() >> libmy/logger.c)
      // comme le logger par défaut est réglé en dur sur DEBUG
      // devlog affiche le log systématiquement.
      devlog(__func__, "create chain failed", 1);
      return (NULL);
    }
  if ((protocol = new_option(OPTIONNAL, 1, 5, "-proto", display_notification)) == NULL)
    {
      devlog(__func__, "create option protocol failed", 1); 
      return (NULL);
    }
  if ((logger = new_option(OPTIONNAL, 0, 1, "-v", build_logger)) == NULL)
    {
      devlog(__func__, "create option verbose failed", 1); 
      return (NULL);
    }
  if ((log_file = new_option(OPTIONNAL, 1, 0, "-log", build_logger)) == NULL)
    {
      devlog(__func__, "create option protocol failed", 1); 
      return (NULL);
    }
  if ((rep_port = new_option(OPTIONNAL, 1, 0, "-rep-port", init_swctx)) == NULL)
    {
      devlog(__func__, "create option -rep-port failed", 1); 
      return (NULL);
    }
  if ((pub_port = new_option(OPTIONNAL, 1, 0, "-pub-port", init_swctx)) == NULL)
    {
      devlog(__func__, "create option -pub-port failed", 1); 
      return (NULL);
    }
  if ((cycle = new_option(OPTIONNAL, 1, 0, "-cycle", init_swctx)) == NULL)
    {
      devlog(__func__, "create option cycle failed", 1); 
      return (NULL);
    }
  if (add_link(&options, protocol))
    {
      devlog(__func__, "add protocol option to chain failed", 1);
      return (NULL);
    }
  if (add_link(&options, logger))
    {
      devlog(__func__, "add logger option to chain failed", 1);
      return (NULL);
    }
  if (add_link(&options, log_file))
    {
      devlog(__func__, "add log_file option to chain failed", 1);
      return (NULL);
    }
  if (add_link(&options, rep_port))
    {
      devlog(__func__, "add rep-port option to chain failed", 1);
      return (NULL);
    }
  if (add_link(&options, pub_port))
    {
      devlog(__func__, "add pub-port option to chain failed", 1);
      return (NULL);
    }
  if (add_link(&options, cycle))
    {
      devlog(__func__, "add cycle option to chain failed", 1);
      return (NULL);
    }
  return (options);
}

/*
** execute()
** fonction appellée pour gérée le cast
** et l'appel du callback (action)
** de l'option passée en paramètre.

** MANQUE LA GESTION D'ERREUR!! 
*/
int	execute(t_option *option)
{
  char	*opt;

  opt = option->name;
  if (!my_strcmp(opt, "-h"))
    (*(void (*)(void))option->action)();
  if (!my_strcmp(opt, "-v") || !my_strcmp(opt, "-log")
      || !my_strcmp(opt, "-rep-port") || !my_strcmp(opt, "-pub-port")
      || !my_strcmp(opt, "-cycle"))
    (*(void (*)(char*, t_chain*))option->action)(opt, option->parameters);
  if (!my_strcmp(opt, "-proto"))
    (*(void (*)(t_chain*))option->action)(option->parameters);
  return (0);
}

/*
** argument parsing process calls
*/
int		sw_parse(int argc, char **argv)
{
  int		act;
  t_chain	*options;
  t_link        *tmp;
  t_option      *opt;
  
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
	  execute(opt); // gestion d'erreur?
	}
      tmp = tmp->next;
    }
  if (!act)
    {
      my_log(__func__, "nothing passed to the program", 2);
      help();
    }
  delete_chain(&options);
  return (0);
}
