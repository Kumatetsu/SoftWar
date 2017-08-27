/*
** command.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul 16 00:15:51 2017 CASTELLARNAU Aurelien
** Last update Sun Aug 27 15:18:27 2017 BILLAUD Jean
*/

#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include "libmy.h"
#include "Softwar_ctx.h"
#include "Player.h"
#include "Energy_cell.h"
#include "Game_manager.h"
#include "hash.h"
#include "command.h"
#include "Enum.h"

/*
** génère un unique id
*/
char            *new_uid()
{
  static int    id;
  char          uid[20];
  char          *heap_uid;

  sprintf(uid, "0x0%d", ++id);
  if ((heap_uid = my_strdup(uid)) == NULL)
    return (NULL);
  return (heap_uid);
}

/*
**
*/
char		*identify(t_game_manager **manager, char *identity, char *optional)
{
  t_player	*new;
  char		log[50];
  char		tmp[50];
  char		*output;

  sprintf(log, "call function identify \noptional: %s", optional);
  my_log(__func__, log, 3);
  if ((*manager)->ready)
    {
      my_log(__func__, "server full", 3);
      if ((output = my_strdup("ko|server full")) == NULL)
	return (NULL);
      return (output);
    }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, log, 3);
  my_log(__func__, "attribute new id", 3);
  if (manager == NULL || (*manager) == NULL || identity == NULL)
    return (NULL);
  if ((new = create_player(new_uid())) == NULL)
    return (NULL);
  if ((*manager)->add_player(new))
    return (NULL);
  if ((*manager)->get_players()->index == 4)
    (*manager)->ready = 1;
  sprintf(tmp, "ok|%s", new->identity);
  if ((output = my_strdup(tmp)) == NULL)
    return (NULL);
  return (output);
}

char	*self_id(t_game_manager **manager, char *identity, char *optional)
{
  char	log[50];

  if ((*manager)->ready)
    sprintf(log, "manager ready, parameter: %s", identity);
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function self_id", 3);
  my_log(__func__, optional, 3);
  return (identity);
}

char		*leave(t_game_manager **manager, char *identity, char *optional)
{
  char		log[80];
  t_link	*tmp;
  t_chain	*players;
  t_player	*p;
  char		*output;

  if ((*manager)->ready)
    sprintf(log, "manager ready, parameter: %s", identity);
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function leave", 3);
  my_log(__func__, optional, 3);
  players = (*manager)->get_players();
  tmp = players->first;
  while (tmp)
    {
      p = (t_player*)tmp->content;
      if (!my_strcmp(p->identity, identity))
	{
	  sprintf(log, "remove link where player identity is: %s.", identity);
	  my_log(__func__, log, 3);
	  if (remove_link(&players, tmp) == -1)
	    my_log(__func__, "no more players", 3);
	  if ((output = my_strdup("Good Bye!")) == NULL)
		return (NULL);
	  if (players->index < 4)
	    (*manager)->ready = 0;
	  return (output);
	}
      tmp = tmp->next;
    }
  sprintf(log, "player %s not found, can't remove it", identity);
  my_log(__func__, log, 2);
  if ((output = my_strdup(log)) == NULL)
    return (NULL);
  return (output);
}

/*
** fonctions en cours de définition,
** la mise en place des mouvement et des energy cells va permettre de facilement
** définir des fonctions de map pour qu'elle soient utilisable et pour les energy cells
** et pour les players !
** phase 1 : gérer les mouvements
** phase 2 : gérer le placement initial des players et apparition des energy cells
** phase 3 : gérer les calculs de map sur map manager
** phse 4 et fin: mettre map manager in game manager
** phase 5 (bonus): faire en sorte que ça marche xD
*/
char		*forward(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];
  t_player	*player;

  if ((*manager)->ready) {    
    sprintf(log, "manager ready, parameter: %s", identity);
    player = (*manager)->get_player(identity);
    switch (player->looking)
      {
      case LEFT:
	player->x = player->x - 1;
	break;
      case UP:
	player->y = player->y - 1;
	break;
      case RIGHT:
	player->x = player->x + 1;
	break;
      case DOWN:
	player->y = player->y + 1;
	break;
    }
  }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function forward", 3);
  my_log(__func__, optional, 3);
  return (identity);
}

char		*backward(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];
  t_player	*player;
  
  if ((*manager)->ready) {
    sprintf(log, "manager ready, parameter: %s", identity);
    player = (*manager)->get_player(identity);
    switch (player->looking)
      {
      case LEFT:
	player->x = player->x + 1;
	break;
      case UP:
	player->y = player->y + 1;
	break;
      case RIGHT:
	player->x = player->x - 1;
	break;
      case DOWN:
	player->y = player->y - 1;
	break;
      }
  }
 else
   sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function backward", 3);
  my_log(__func__, optional, 3);
  return (identity);
}

char		*left(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];
  t_player	*player;

  if ((*manager)->ready){
    sprintf(log, "manager ready, parameter: %s", identity);
    player = (*manager)->get_player(identity);
    if (player->looking == LEFT) {
      player->looking = DOWN;
    } else {
      player->looking--;
    }
  }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function left", 3);
  my_log(__func__, optional, 3);
  return (identity);
}

char		*right(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];
  t_player	*player;
  
  if ((*manager)->ready) {
    sprintf(log, "manager ready, parameter: %s", identity);
    player = (*manager)->get_player(identity);
    if (player->looking == DOWN){
      player->looking = LEFT;
    } else {
      player->looking++;
    }
  }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function right", 3);
  my_log(__func__, optional, 3);
  return (identity);
}

char		*leftfwd(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];
  
  if ((*manager)->ready) {
    sprintf(log, "manager ready, parameter: %s", identity);
    //le optionnal je suis vraiment pas sûr je sais pas dans quel cas il sera là???
    //donc coup de poker ça sera changé si besoin;
    left(manager, identity, optional);
    forward(manager,identity, optional);
  }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function leftfwd", 3);
  my_log(__func__, optional, 3);
  return (identity);
}

char		*rightfwd(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];

  if ((*manager)->ready) {
    sprintf(log, "manager ready, parameter: %s", identity);
    right(manager, identity, optional);
    forward(manager, identity, optional);
  }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function rightfwd", 3);
  my_log(__func__, optional, 3);
  return (identity);
}


/*
** Attention, tout ajout de fonction doit donner
** lieu à une modification de HCOMMANDSIZE dans hash.h
** réfléchir à une solution dynamique.
** une t_command est de type:
**  char *(*)(t_game_manager**, char*, char*);
** avec char* 1 == identity et char* 2 == argument optionnel 
** Définie en static, ne contenant que des pointeurs,
** pas besoin de free quoi que ce soit...
*/
t_command		**get_commands()
{
  static t_command	*commands[HCOMMANDSIZE] = { NULL };
  
  if (commands[0] == NULL)
    {
      commands[hash_command("identify")] = identify; // 1
      commands[hash_command("selfid")] = self_id; // 2
      commands[hash_command("leave")] = leave; // 3
      commands[hash_command("forward")] = forward; // 4
      commands[hash_command("backward")] = backward; // 5
      commands[hash_command("leftfwd")] = leftfwd; // 6
      commands[hash_command("rightfwd")] = rightfwd; // 7
      commands[hash_command("left")] = left; // 8
      commands[hash_command("right")] = right; // 9
      // etc
    }
  return (commands);
}
