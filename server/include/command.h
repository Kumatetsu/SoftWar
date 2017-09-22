/*
** command.h for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul 16 00:16:54 2017 CASTELLARNAU Aurelien
** Last update Wed Sep 13 20:27:16 2017 BILLAUD Jean
*/

#ifndef  _COMMAND_H_
# define _COMMAND_H_

# define FAIL 0
# define SUCCESS 1

typedef char *(t_command)(t_game_manager**, char*, char*);

t_command	**get_commands();
char		*identify(t_game_manager **manager, char *identity, char *optional);
#endif  /* !_COMMAND_H_ */
