/*
** softwar_context.h for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul  9 22:28:05 2017 CASTELLARNAU Aurelien
** Last update Tue Jul 11 16:57:22 2017 BILLAUD Jean
*/

#ifndef  _SOFTWAR_CONTEXT_H_
#define _SOFTWAR_CONTEXT_H_

#include <czmq.h>

typedef struct  s_swsock
{
  char		*name;
  zsock_t	*socket;
}		t_swsock;

typedef struct	s_softwar_context
{
  unsigned int	rep_port;
  unsigned int	pub_port;
  unsigned int	cycle;
  t_swsock	*active_socket;
  zpoller_t	*poller;
  t_chain	*sockets;
}		t_swctx;


t_swctx		*init_swctx(char *opt, t_chain *parameters);
t_swctx		*get_swctx();
void		free_ctx();
t_swsock	*create_socket(char *name, zsock_t *socket);

#endif /* !_SOFTWAR_CONTEXT_H_ */
