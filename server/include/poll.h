/*
** poll.h for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Mon Jul 10 01:15:44 2017 CASTELLARNAU Aurelien
** Last update Mon Jul 10 01:17:17 2017 CASTELLARNAU Aurelien
*/

#ifndef  _POLL_H_
# define _POLL_H_

zmsg_t	*init_poll(t_swctx **ctx);
int	listen_and_wait(t_swctx **ctx);
zmsg_t	*read_socket(t_swctx **ctx);
zsock_t	*get_socket(char *name, t_swctx **ctx);

#endif  /* !_POLL_H_ */
