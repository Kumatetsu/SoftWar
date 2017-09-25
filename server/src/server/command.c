/*
** command.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul 16 00:15:51 2017 CASTELLARNAU Aurelien
** Last update Thu Sep 14 17:25:58 2017 BILLAUD Jean
*/

#include <stdio.h>
#include <stdlib.h>
#include <json/json.h>
#include "libmy.h"
#include "softwar_ctx.h"
#include "player.h"
#include "energy_cell.h"
#include "game_manager.h"
#include "hash.h"
#include "utils.h"
#include "command.h"
#include "enum.h"

/*
** génère le bon début d'output
** en fonction d'un bool
** ALLOCATE MEMORY
*/

char	*generate_output(int success)
{
  char	tmp[4];
  char	*output;
  
  if (success)
    sprintf(tmp, "ok|");
  else
    sprintf(tmp, "ko|");
  if ((output = my_strdup(tmp)) == NULL)
    return (NULL);
  return (output);
}

/*
** génère le bon début d'output
** en fonction d'un bool
** ALLOCATE MEMORY
*/
char	*generate_output_param(int success, char *param)
{
  char	tmp[80];
  char  log[80];
  char	*output;

  my_log(__func__, "generating output", 4);
  sprintf(log, "Softwar is generating an output with parameter: %s", param);
  my_log(__func__, log, 4);
  if (success)
    sprintf(tmp, "ok|%s", param);
  else
    sprintf(tmp, "ko|");
  if ((output = my_strdup(tmp)) == NULL)
    return (NULL);
  sprintf(log, "returnin output: %s", output);
  my_log(__func__, log, 4);
  return (output);
}

/*
** génère un unique id
** ALLOCATE MEMORY
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
      if((output = my_strdup("ko|server full")) == NULL)
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
  (*manager)->set_change(1);
  my_log(__func__, output, 4);
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
  (*manager)->set_change(1);
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
  (*manager)->set_change(1);
  return (output);
}

/*
**
*/
char		*forward(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*p;
  
  success = 0;
  if ((*manager)->ready)
    {    
      sprintf(log, "manager ready, parameter: %s", identity);
      my_log(__func__, log, 4);
      p = (*manager)->get_player(identity);
      if (p == NULL)
	{
	  sprintf(log, "can't retrieve player by id: %s", identity);
	  my_log(__func__, log, 2);
	  return (NULL);
	}
      switch (p->looking)
	{
	case LEFT:
	  if (check_mvmnt(p->x - 1, p->y, identity, (*manager)) == 1)
	    sprintf(log, "%s can't move left", identity);
	  else if (p->action < 0.5)
	    sprintf(log, "%s action point are too low", identity);
	  else
	    {
	      p->action = p->action - 0.5;
	      p->x = p->x - 1;
	      success = 1;
	    }
	  break;
	case UP:
	  if (check_mvmnt(p->x, p->y - 1, identity, (*manager)) == 1)
	    sprintf(log, "%s can't move up", identity);
	  else if (p->action < 0.5)
	    sprintf(log, "%s action point are too low", identity);
	  else
	    {
	      p->action = p->action - 0.5;
	      p->y = p->y - 1;
	      success = 1;
	    }
	  break;
	case RIGHT:
	  if (check_mvmnt(p->x + 1, p->y, identity, (*manager)) == 1)
	    sprintf(log, "%s can't move right", identity);
	  else if (p->action < 0.5)
	    sprintf(log, "%s action point are too low", identity);
	  else
	    {
	      p->action = p->action - 0.5;
	      p->x = p->x + 1;
	      success = 1;
	    }
	  break;
	case DOWN:
	  if (check_mvmnt(p->x, p->y + 1, identity, (*manager)) == 1)
	    sprintf(log, "%s can't move down", identity);
	  else if (p->action < 0.5)
	    sprintf(log, "%s action point are too low", identity);
	  else
	    {
	      p->action = p->action - 0.5;
	      p->y = p->y + 1;
	      success = 1;
	    }
	  break;
	default:
	  my_log(__func__, "falled in default in forward switch", 3);
	  break;
	}
    }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, log, 4);
  my_log(__func__, "call function forward", 3);
  my_log(__func__, optional, 3);
  (*manager)->set_change(1);
  return (generate_output(success));
}

char		*backward(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*p;

  success = 0;
  if ((*manager)->ready) {
    sprintf(log, "manager ready, parameter: %s", identity);
    p = (*manager)->get_player(identity);
    switch (p->looking)
      {
      case LEFT:
	if (check_mvmnt(p->x + 1, p->y, identity, (*manager)) == 1)
	  sprintf(log, "%s can't backward right", identity);
	else if (p->action < 0.5)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->action = p->action - 0.5;
	    p->x = p->x + 1;
	    success = 1;
	  }
	break;
      case UP:
	if (check_mvmnt(p->x, p->y + 1, identity, (*manager)) == 1)
	  sprintf(log, "%s can't backward down", identity);
	else if (p->action < 0.5)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->action = p->action - 0.5;
	    p->y = p->y + 1;
	    success = 1;
	  }
	break;
      case RIGHT:
	if (check_mvmnt(p->x - 1, p->y, identity, (*manager)) == 1)
	  sprintf(log, "%s can't backward left", identity);
	else if (p->action < 0.5)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->action = p->action - 0.5;
	    p->x = p->x - 1;
	    success = 1;
	  }
	break;
      case DOWN:
	if (check_mvmnt(p->x, p->y + 1, identity, (*manager)) == 1)
	  sprintf(log, "%s can't backward up", identity);
	else if (p->action < 0.5)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->action = p->action - 0.5;
	    p->y = p->y + 1;
	    success = 1;
	  }
	break;
      default:
	break;
      }
  }
 else
   sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function backward", 3);
  my_log(__func__, optional, 3);
  (*manager)->set_change(1);
  return (generate_output(success));
}

char		*left(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*player;

  success = 0;
  if ((*manager)->ready)
    {
      sprintf(log, "manager ready, parameter: %s", identity);
      player = (*manager)->get_player(identity);
      if (player->action < 0.5)
	sprintf(log, "%s action point are too low", identity);
      else
	{
	  player->action = player->action - 0.5;
	  player->looking = (player->looking == LEFT) ? DOWN : (player->looking - 1);
	  success = 1;
	}
    }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function left", 3);
  my_log(__func__, optional, 3);
  (*manager)->set_change(1);
  return (generate_output(success));
}

char		*right(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*player;

  success = 0;
  if ((*manager)->ready)
    {
      sprintf(log, "manager ready, parameter: %s", identity);
      player = (*manager)->get_player(identity);
      if (player->action < 0.5)
	sprintf(log, "%s action point are too low", identity);
      else
	{
	  player->action = player->action - 0.5;
	  player->looking = (player->looking == DOWN) ? LEFT : (player->looking + 1);
	  success = 1;
	}
    }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function right", 3);
  my_log(__func__, optional, 3);
  (*manager)->set_change(1);
  return (generate_output(success));
}

char		*leftfwd(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*player;

  success = 0;
  player = (*manager)->get_player(identity);
  if ((*manager)->ready)
    {
      sprintf(log, "manager ready, parameter: %s", identity);
      if (player->action < 1)
	sprintf(log, "%s action point are too low", identity);
      else
	{
	  if ((left(manager, identity, optional)) == NULL)
	    return (NULL);
	  if ((forward(manager,identity, optional)) == NULL)
	    return (NULL);
	  success = 1;
	}
    }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function leftfwd", 3);
  my_log(__func__, optional, 3);
  (*manager)->set_change(1);
  return (generate_output(success));
}

char		*rightfwd(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*player;

  success = 0;
  player = (*manager)->get_player(identity);
  if ((*manager)->ready)
    {
      sprintf(log, "manager ready, parameter: %s", identity);
      if (player->action < 1)
	sprintf(log, "%s action point are too low", identity);
      else
	{
	  if ((right(manager, identity, optional)) == NULL)
	    return (NULL);
	sprintf(log, "new looking value after right(): %d", player->looking);
	my_log(__func__, log, 4);
	  if ((forward(manager, identity, optional)) == NULL)
	    return (NULL);
	  success = 1;
	}
    }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function rightfwd", 3);
  my_log(__func__, optional, 3);
  return (generate_output(success));
}

/*
** looking: renvoit l'orientation du processus (voir include/enum.h pour concordance)
*/
char		*looking(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];
  t_player	*p;
  char		look[2];
  
  if ((*manager)->ready)
    {
      if ((p = (*manager)->get_player(identity)) == NULL)
	return (NULL);
      sprintf(look, "%d", p->looking);
      (*manager)->set_change(1);
      return (generate_output_param(SUCCESS, look));
    }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function looking", 3);
  my_log(__func__, optional, 3);
  return (generate_output(FAIL));
}

/*
** Choix de la gestion d'erreur:
** Gather est considérée successful dès lors que le player
** a récupéré l'énergie. Si la tentative de suppression échoue,
** aucun segfault n'est à redouté, du coup, l'énergie cell restera
** sur le plateau et le jeux ne plantera pas. Un warning est loggé
** en cas d'échec des opérations de passage de l'energy_cell au t_link
** et de suppression du t_link. On laisse la porte ouverte à un bug
** pour rendre le programme plus résiliant...
**
** Un point d'action est débité si la commande est successfull.
**
** Au final, gather fail quand il n'y a pas d'energy_cell sur la case ;). 
*/
char		*gather(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];
  t_player	*player;
  t_chain	*energy_cells;
  t_link	*link;
  t_energy_cell *ec;

  player = (*manager)->get_player(identity);
  energy_cells = (*manager)->get_energy_cells();
  if ((*manager)->ready)
    {
      sprintf(log, "manager ready, parameter: %s", identity);
      if ((ec = is_energy_cell(player->x, player->y, energy_cells)) != NULL)
	{
	  sprintf(log, "player gathered %d energy", ec->value);
	  my_log(__func__, log, 4);
	  player->energy += ec->value;
	  player->action -= 1;
	  if ((link = get_link_by_content(ec, energy_cells)) != NULL)
	    {
	      int removelink = remove_link(&energy_cells, link);
	      sprintf(log, "value of remove_link() return: %d", removelink);
	      my_log(__func__, log, 4);
	      if (removelink == 1)
		my_log(__func__, "delete energy cell failed", 3);
	      else if (removelink == -1)
		my_log(__func__, "no more energy_cell", 4);
	      else
		my_log(__func__, "energy cell consumed", 2);
	    }
	  else
	    my_log(__func__, "try retrieving t_link by energy_cell content failed", 3);
	  (*manager)->set_change(1);
	  return (generate_output(SUCCESS));
	}
    }
  else
      sprintf(log, "manager not ready, parameter: %s", optional);
  return (generate_output(FAIL));
}

/*
** Keep breathing...
*/
char		*watch(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];
  t_player	*player;
  uint		a;
  uint		b;
  int		operator_inv;
  int		swap;
  uint		**zone;
  char		*state;

  if ((*manager)->ready)
    {
      sprintf(log, "processus %s is watching", identity);
      my_log(__func__, log, 4);
      if ((player = (*manager)->get_player(identity)) == NULL)
	return (NULL);
      a = (player->looking == LEFT || player->looking == RIGHT) ? player->x : player->y;
      b = (player->looking == LEFT || player->looking == RIGHT) ? player->y : player->x;
      operator_inv = (player->looking == RIGHT || player->looking == DOWN) ? 1 : 0;
      swap = (player->looking == UP || player->looking == DOWN) ? 1 : 0;
      if ((zone = eq_watch(a, b, operator_inv, swap)) == NULL)
	return (NULL);
      if ((state = compile_watch_return(manager, zone)) == NULL)
	{
	  my_log(__func__, "compiling watch return failed", 4);
	  return (NULL);
	}
      return (generate_output_param(SUCCESS, state));
    }
  else
    sprintf(log, "manager not ready, processus %s wanted to watch", identity);
  my_log(__func__, "call function watch", 3);
  my_log(__func__, optional, 3);
  return (generate_output(FAIL));
}

char		*attack(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];

  if ((*manager)->ready) {
    sprintf(log, "manager ready, parameter: %s", identity);
  }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function attack", 3);
  my_log(__func__, optional, 3);
  return (identity);
}

char		*jump(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*p;

  success = 0;
  if ((*manager)->ready) {    
    sprintf(log, "manager ready, parameter: %s", identity);
    p = (*manager)->get_player(identity);
    switch (p->looking)
      {
      case LEFT:
	if (check_mvmnt(p->x - 2, p->y, identity, (*manager)) == 1)
	  sprintf(log, "%s can't jump left", identity);
	else if (p->action < 2)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->action = p->action - 2;
	    p->x = p->x - 2;
	    success = 1;
	  }
	break;
      case UP:
	if (check_mvmnt(p->x, p->y - 2, identity, (*manager)) == 1)
	  sprintf(log, "%s can't jump  up", identity);
	else if (p->action < 2)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->action = p->action - 2;
	    p->y = p->y - 2;
	    success = 1;
	  }
	break;
      case RIGHT:
	if (check_mvmnt(p->x + 2, p->y, identity, (*manager)) == 1)
	  sprintf(log, "%s can't jump right", identity);
	else if (p->action < 2)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->action = p->action - 2;
	    p->x = p->x + 2;
	    success = 1;
	  }
	break;
      case DOWN:
	if (check_mvmnt(p->x, p->y + 2, identity, (*manager)) == 1)
	  sprintf(log, "%s can't jump down", identity);
	else if (p->action < 2)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->action = p->action - 2;
	    p->y = p->y + 2;
	    success = 1;
	  }
	break;
      default:
	break;
    }
  }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function jump", 3);
  my_log(__func__, optional, 3);
  return (generate_output(success));
}

char		*self_stats(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];

  if ((*manager)->ready) {
    sprintf(log, "manager ready, parameter: %s", identity);
  }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function self_stat", 3);
  my_log(__func__, optional, 3);
  return (identity);
}

char		*inspect(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];

  if ((*manager)->ready) {
    sprintf(log, "manager ready, parameter: %s", identity);
  }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function inspect", 3);
  my_log(__func__, optional, 3);
  return (identity);
}

char		*next(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];
  t_player	*p;
  
  if ((*manager)->ready) {
    sprintf(log, "manager ready, parameter: %s", identity);
    sprintf("%s decided to pass", identity);
    p = (*manager)->get_player(identity);
    p->action = 0;
  }
  else
    sprintf(log, "manager not ready, parameter: %s", identity);
  my_log(__func__, "call function next", 3);
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
      commands[hash_command("looking")] = looking; // 10
      commands[hash_command("gather")] = gather; // 11
      commands[hash_command("attack")] = attack; // 12
      commands[hash_command("jump")] = jump; // 13
      commands[hash_command("watch")] = watch; // 14
      commands[hash_command("selfstats")] = self_stats; // 15
      commands[hash_command("inspect")] = inspect; // 16
      commands[hash_command("next")] = next; // 17
    }
  return (commands);
}
