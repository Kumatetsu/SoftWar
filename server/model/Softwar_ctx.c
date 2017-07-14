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
/*
** Fonction passée à la création de la chaine
** t_swctx->sockets
** Permet de détruire les sockets stockées
** dans une liste chainée passée en paramètre.
*/
void	free_sockets(t_chain **sockets);

/*
** global and static context.
*/
static t_swctx	*ctx = NULL;

/*
** Crée une socket du type SoftWar
** adaptée à notre contexte, c'est une
** socket avec un char * en plus pour nous...
*/
t_swsock	*create_socket(char *name, zsock_t *socket)
{
  t_swsock	*s;

  if ((s = malloc(sizeof (*s))) == NULL)
    {
      my_log(__func__, MEM_ERR, 1);
      return (NULL);
    }
  if ((s->name = my_strdup(name)) == NULL)
    return (NULL);
  s->socket = socket;
  return (s);
}

/*
** finalize_ctx
** appellée par le getter pour s'assurer de toujours
** avoir le context. Si la définition du context
** par le biais du process de parsing foire, on est sûr
** d'avoir au moins les valeurs par défaut.
** Si le ctx est bien setté de partout, finalize ne fait rien.
*/
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
      if ((ctx->active_socket = malloc(sizeof ((*ctx->active_socket)))) == NULL)
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

/*
** Fonction conçue pour être exécutée avec les options suivantes:
** -rep-port [int >0]
** -pub-port [int > 0]
** -cycle [int > 0]
** passée comme callback au parseur d'argument, 
** cette fonction définie le SoftWar_ctx
*/
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
      if ((ctx->active_socket = malloc(sizeof ((*ctx->active_socket)))) == NULL)
	{
	  my_log(__func__, MEM_ERR, 1);
	  return (NULL);
	}
      ctx->rep_port = 0;
      ctx->pub_port = 0;
      ctx->cycle = 0;
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

/*
** Getter. le ctx étant une instance
** unique, privée et statique accessible
** uniquement grâce à cette méthode, c'est un singleton.
** Important dans le cas des threads: 
** si deux threads cherchent à modifier le ctx en même temps
** ca peut poser problème. Voir mutexes et courses concurrentielles.
*/
t_swctx	*get_swctx()
{
  return (finalize_ctx());
}

/*
** fonction de libération de la t_chain contenue dans le ctx.
*/
void		free_sockets(t_chain **sockets)
{
  t_link	*tmp;
  t_swsock	*s;

  tmp = (*sockets)->first;
  while (tmp)
    {
      s = (t_swsock*)tmp->content;
      zsock_destroy(&(s->socket));
      free(s->name);
      tmp = tmp->next;
    }
}

/*
** Libération du ctx.
*/
void		free_ctx()
{
  if (ctx != NULL)
    {
      if (ctx->sockets != NULL)
	{
	  if (ctx->sockets->free != NULL) // DEBUG
	    my_log(__func__, "sockets list got free function!", 4); //DEBUG
	  delete_chain(&(ctx->sockets));
	}
      if (ctx->active_socket != NULL)
	free(ctx->active_socket->name);
      if (ctx->poller != NULL)
	zpoller_destroy(&(ctx->poller));
      /*
      ** Not sure at all...
      ** cette socket est renvoyé par le poller
      ** c'est la socket active à chaque tour de boucle
      ** je ne sais pas si zpoller_destroy s'en occuppe déjà
      ** ou même si cette socket reste en vie ou est un pointeur
      ** sur l'une des notre...
      if (ctx->active_socket->socket != NULL)
        zsock_destroy(&(ctx->active_socket->socket));
      */
      free(ctx);
    }
}
