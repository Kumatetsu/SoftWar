/*
** command.h for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul 16 00:16:54 2017 CASTELLARNAU Aurelien
** Last update Thu Aug 24 16:47:50 2017 BILLAUD Jean
*/

#ifndef  _COMMAND_H_
# define _COMMAND_H_

typedef char *(t_command)(t_game_manager**, char*, char*);

t_command	**get_commands();
char		*forward(t_game_manager **manager, char *identity, char *optional);
char		*left(t_game_manager **manager, char *identity, char *optional);
char		*right(t_game_manager **manager, char *identity, char *optional);
char		*backward(t_game_manager **manager, char *identity, char *optional);
char		*rightfwd(t_game_manager **manager, char *identity, char *optional);
char		*leftfwd(t_game_manager **manager, char *identity, char *optional);

#endif  /* !_COMMAND_H_ */
