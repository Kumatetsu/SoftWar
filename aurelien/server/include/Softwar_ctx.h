/*
** softwar_context.h for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul  9 22:28:05 2017 CASTELLARNAU Aurelien
** Last update Sun Jul  9 22:33:30 2017 CASTELLARNAU Aurelien
*/

#ifndef  _SOFTWAR_CONTEXT_H_
# define _SOFTWAR_CONTEXT_H_

#include <czmq.h>

typedef struct	s_softwar_context
{
  unsigned int	rep_port;
  unsigned int	pub_port;
  unsigned int	cycle;
  zsock_t	*active_socket;
  zpoller_t	*poller;
  t_chain	*sockets;
}		t_swctx;

t_swctx		*init_swctx(char *opt, t_chain *parameters);
t_swctx		*get_swctx();
void		free_ctx();

#endif /* !_SOFTWAR_CONTEXT_H_ */
