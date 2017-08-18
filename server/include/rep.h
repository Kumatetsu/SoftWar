/*
** rep.h for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul  9 22:03:07 2017 CASTELLARNAU Aurelien
** Last update Sun Jul  9 22:04:15 2017 CASTELLARNAU Aurelien
*/

#ifndef  _REP_H_
# define _REP_H

zsock_t	*init_rep(int rep_port);
char	*rep_read(zsock_t *socket);
int	rep_response(zsock_t *socket, char *message);

#endif /* !_REP_H_ */
