/*
** protocol.h for SoftWar in /home/code/SoftWar/aurelien
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sat Jul  1 01:26:52 2017 CASTELLARNAU Aurelien
** Last update Sat Jul  1 02:07:14 2017 CASTELLARNAU Aurelien
*/

#ifndef  _PROTOCOL_H_
# define _PROTOCOL_H_

typedef char t_notification;

t_notification *get_notification(int type);
void		display_notification(t_chain *parameters);

#endif  /* !_PROTOCOL_H_ */
