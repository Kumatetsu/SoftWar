/*
** argument.h for SoftWar in /home/code/SoftWar/aurelien
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sat Jul  1 01:53:49 2017 CASTELLARNAU Aurelien
** Last update Tue Jul 11 16:38:51 2017 BILLAUD Jean
*/

#ifndef _ARGUMENT_H_
#define _ARGUMENT_H_

void	help();
t_chain	*get_options();
int	execute(t_option *options);
int	sw_parse(int argc, char **argv);

#endif  /* !_ARGUMENT_H_ */
