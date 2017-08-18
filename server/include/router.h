/*
** router.h for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Mon Jul 31 19:34:13 2017 CASTELLARNAU Aurelien
** Last update Mon Jul 31 19:36:08 2017 CASTELLARNAU Aurelien
*/

#ifndef  _ROUTER_H_
# define _ROUTER_H_

zsock_t	*init_router(int rep_port);
zmsg_t	*router_read(zsock_t *socket);
//int	router_response(zsock_t *socket, zmsg_t **msg);

#endif  /* !_ROUTER_H_ */
