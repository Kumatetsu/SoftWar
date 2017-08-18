/*
** command.h for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul 16 00:16:54 2017 CASTELLARNAU Aurelien
** Last update Sun Jul 16 00:20:06 2017 CASTELLARNAU Aurelien
*/

#ifndef  _COMMAND_H_
# define _COMMAND_H_

typedef char *(t_command)(t_game_manager**, char*, char*);

t_command	**get_commands();

#endif  /* !_COMMAND_H_ */
