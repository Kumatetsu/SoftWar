/*
** argument.h for SoftWar in /home/code/SoftWar/aurelien
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sat Jul  1 01:53:49 2017 CASTELLARNAU Aurelien
** Last update Sat Jul  1 02:29:40 2017 CASTELLARNAU Aurelien
*/

#ifndef  _ARGUMENT_H_
# define _ARGUMENT_H_

void	help();
t_chain	*get_options();
int	execute(t_option *options);
int	sw_parse(int argc, char **argv);

#endif  /* !_ARGUMENT_H_ */
