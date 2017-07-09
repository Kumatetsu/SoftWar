/*
** Softwar_Context.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul  9 22:32:32 2017 CASTELLARNAU Aurelien
** Last update Sun Jul  9 22:34:32 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include <czmq.h>
#include "libmy.h"
#include "Softwar_ctx.h"

/*
** private function
*/
void	free_sockets(t_chain **sockets);

/*
** global and static context.
*/
static t_swctx	*ctx = NULL;

t_swctx	*finalize_ctx()
{
  if (ctx == NULL)
    {
      if ((ctx = malloc(sizeof (*ctx))) == NULL)
	{
	  my_log(__func__, MEM_ERR, 1);
	  return (NULL);
	}
      ctx->rep_port = 0;
      ctx->pub_port = 0;
      ctx->cycle = 0;
      if ((ctx->sockets = create_chain(free_sockets)) == NULL)
	{
	  my_log(__func__, MEM_ERR, 1);
	  return (NULL);
	}
    }
  if (ctx->rep_port == 0)
    ctx->rep_port = 4242;
  if (ctx->pub_port == 0)
    ctx->pub_port = 4243;
  if (ctx->cycle == 0)
    ctx->cycle = 1000000;
  return (ctx);
}

t_swctx			*init_swctx(char *opt, t_chain *parameters)
{
  t_link		*tmp;
  char			*param;

  param = NULL;
  if (parameters != NULL)
    {
      tmp = parameters->dictionnary[0];
      param = tmp->content;
    }
  if (ctx == NULL)
    {
      if ((ctx = malloc(sizeof (*ctx))) == NULL)
	{
	  my_log(__func__, MEM_ERR, 1);
	  return (NULL);
	}
      ctx->rep_port = 0;
      ctx->pub_port = 0;
      ctx->cycle = 0;
      ctx->active_socket = NULL;
      ctx->poller = NULL;
      if ((ctx->sockets = create_chain(free_sockets)) == NULL)
	{
	  my_log(__func__, MEM_ERR, 1);
	  return (NULL);
	}
    }
  if (!my_strcmp(opt, "-rep-port"))
    ctx->rep_port = my_getnbr(param);
  if (!my_strcmp(opt, "-pub-port"))
    ctx->pub_port = my_getnbr(param);
  if (!my_strcmp(opt, "-cycle"))
    ctx->cycle = my_getnbr(param);
  return (ctx);
}

t_swctx	*get_swctx()
{
  return (finalize_ctx());
}

void		free_sockets(t_chain **sockets)
{
  t_link	*tmp;
  zsock_t	*socket;

  tmp = (*sockets)->first;
  while (tmp)
    {
      socket = (zsock_t*)tmp->content;
      zsock_destroy(&socket);
      tmp = tmp->next;
    }
}

void		free_ctx()
{
  if (ctx != NULL)
    {
      if (ctx->poller != NULL)
	zpoller_destroy(&(ctx->poller));
      if (ctx->sockets != NULL)
	delete_chain(&(ctx->sockets));
      /*
      ** Not sure at all...
      if (ctx->active_socket != NULL)
        zsock_destroy(ctx->active_socket);
      */
      free(ctx);
    }
}
