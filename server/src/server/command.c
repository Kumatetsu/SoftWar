/*
** command.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul 16 00:15:51 2017 CASTELLARNAU Aurelien
** Last update Thu Oct  5 16:54:54 2017 BILLAUD Jean
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
  char	*output;

  if (success)
    sprintf(tmp, "ok|%s", param);
  else
    sprintf(tmp, "ko|%s", param);
  if ((output = my_strdup(tmp)) == NULL)
    return (NULL);
  return (output);
}

/*
** génère un unique id
** ALLOCATE MEMORY
*/
t_player	*check_uid(t_chain *players, char *id)
{
  char		log[50];
  t_link	*tmp;
  t_player	*p;

  tmp = players->first;
  while (tmp)
    {
      p = tmp->content;
      if (!my_strcmp(p->identity, id))
	{
	  sprintf(log, "Identity %s already taken", id);
	  my_log(__func__, log, 2);
	  return (p);
	}
      tmp = tmp->next;
    }
  return (NULL);
}

/*
**
*/
char		*identify(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];
  t_player	*new;
  t_game_info	**info;

  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  my_log(__func__, identity, 4);
  if ((*info)->game_status)
    {
      my_log(__func__, "a client attempt to connect but the server is full", 3);
      return (generate_output_param(FAIL, "game full"));
    }
  if ((new = check_uid((*manager)->get_players(), optional)) != NULL)
    {
      if (new->disabled == -1)
	{
	  sprintf(log, "client with id %s war disconnected, new client take his place", new->identity);
	  my_log(__func__, log, 3);
	  new->disabled = 0;
	}
      else {
	my_log(__func__, "before identity exist", 3);
	return (generate_output_param(FAIL, "identity already exists"));
      }
    }
  sprintf(log, "Server accept new client with id: %s", optional);
  my_log(__func__, log, 3);
  if (new != NULL && (*manager)->add_player(new))
    return (NULL);
  else if(new == NULL && (new = create_player(optional)) != NULL)
    {
      if ((*manager)->add_player(new))
	return (NULL);
    }
  else
    return (NULL);
  (*manager)->set_players_pos((*manager)->get_players(), (*manager)->get_map_size());
  sprintf(log, "number of client actually registered: %d", (*manager)->get_players()->index);
  my_log(__func__, log, 3);
  if ((*manager)->get_players()->index >= 4)
    {
      (*info)->game_status = 1;
      (*manager)->ready = 1;
      my_log(__func__, "GAME IS READY", 3);
    }
  return (generate_output_param(SUCCESS, optional));
}

char		*self_id(t_game_manager **manager, char *identity, char *optional)
{
  char		log[1024];
  t_player	*p;

  // pas obligatoire mais permet d'utiliser tout les paramètres
  // au passage, check l'id, c'est pas plus mal...
  if ((p = (*manager)->get_player(identity)) == NULL)
    return (generate_output(FAIL));
  sprintf(log, "player %s is asking for his own id, optional parameter: %s", identity, optional);
  my_log(__func__, log, 3);
  return (generate_output_param(SUCCESS, identity));
}

/*
** Si un joueur quitte en cours de partie
** sa propriété disabled passe à -1
** Cela permet de réatribuer le joueur
** si le même client propose la même id (voir identify())
** cela permet aussi de ne pas bloquer la partie sur un crash client.
*/
char		*leave(t_game_manager **manager, char *identity, char *optional)
{
  int		rm_link;
  char		log[80];
  t_link	*tmp;
  t_chain	*players;
  t_player	*p;
  t_game_info	**info;


  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  if ((players = (*manager)->get_players()) == NULL)
    return (NULL);
  if ((p = (*manager)->get_player(identity)) == NULL)
    {
      sprintf(log, "can't retrieve client with id %s", identity);
      my_log(__func__, log, 2);
      return (NULL);
    }
  if ((tmp = get_link_by_content(p, (*manager)->get_players())) == NULL)
    {
      my_log(__func__, "can't retrieve link by content", 2);
      return (generate_output_param(FAIL, "server_error"));
    }
  if ((*info)->game_status == 1)
    p->disabled = -1;
  else
    { 
      rm_link = remove_link(&players, tmp);
      if (rm_link == -1)
	my_log(__func__, "no more players", 3);
      else if(rm_link)
	{
	  sprintf(log, "can't delete player %s", identity);
	  my_log(__func__, log, 2);
	  return (NULL);
	}
      if ((*info)->game_status != 1 && players->index < 4) {
	(*manager)->ready = 0;
	(*info)->game_status = 0;
      }
      
    }
  my_log(__func__, optional, 4);
  return (generate_output(SUCCESS));
}

/*
**
*/
char		*forward(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*p;
  t_game_info	**info;


  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  success = 0;
  my_log(__func__, optional, 4);
  sprintf(log, "forward done");
  if ((p = (*manager)->get_player(identity)) == NULL)
    return (NULL);
  if ((*info)->game_status && !p->disabled)
    {    
      switch (p->looking)
	{
	case LEFT:
	  if (check_mvmnt(p->x - 1, p->y, identity, (*manager)) == 1)
	    sprintf(log, "%s can't move left", identity);
	  else if (p->action < 5)
	    sprintf(log, "%s action point are too low", identity);
	  else
	    {
	      p->action = p->action - 5;
	      p->x = p->x - 1;
	      success = 1;
	    }
	  break;
	case UP:
	  if (check_mvmnt(p->x, p->y - 1, identity, (*manager)) == 1)
	    sprintf(log, "%s can't move up", identity);
	  else if (p->action < 5)
	    sprintf(log, "%s action point are too low", identity);
	  else
	    {
	      p->action = p->action - 5;
	      p->y = p->y - 1;
	      success = 1;
	    }
	  break;
	case RIGHT:
	  if (check_mvmnt(p->x + 1, p->y, identity, (*manager)) == 1)
	    sprintf(log, "%s can't move right", identity);
	  else if (p->action < 5)
	    sprintf(log, "%s action point are too low", identity);
	  else
	    {
	      p->action = p->action - 5;
	      p->x = p->x + 1;
	      success = 1;
	    }
	  break;
	case DOWN:
	  if (check_mvmnt(p->x, p->y + 1, identity, (*manager)) == 1)
	    sprintf(log, "%s can't move down", identity);
	  else if (p->action < 5)
	    sprintf(log, "%s action point are too low", identity);
	  else
	    {
	      p->action = p->action - 5;
	      p->y = p->y + 1;
	      success = 1;
	    }
	  break;
	default:
	  break;
	}
      my_log(__func__, log, 4);
    }
  return (generate_output(success));
}

char		*backward(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*p;
  t_game_info	**info;


  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  success = 0;
  my_log(__func__, optional, 4);
  if ((p = (*manager)->get_player(identity)) == NULL)
    return (NULL);
  if ((*info)->game_status && !p->disabled) {
    switch (p->looking)
      {
      case LEFT:
	if (check_mvmnt(p->x + 1, p->y, identity, (*manager)) == 1)
	  sprintf(log, "%s can't backward right", identity);
	else if (p->action < 5)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->action = p->action - 5;
	    p->x = p->x + 1;
	    success = 1;
	  }
	break;
      case UP:
	if (check_mvmnt(p->x, p->y + 1, identity, (*manager)) == 1)
	  sprintf(log, "%s can't backward down", identity);
	else if (p->action < 5)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->action = p->action - 5;
	    p->y = p->y + 1;
	    success = 1;
	  }
	break;
      case RIGHT:
	if (check_mvmnt(p->x - 1, p->y, identity, (*manager)) == 1)
	  sprintf(log, "%s can't backward left", identity);
	else if (p->action < 5)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->action = p->action - 5;
	    p->x = p->x - 1;
	    success = 1;
	  }
	break;
      case DOWN:
	if (check_mvmnt(p->x, p->y + 1, identity, (*manager)) == 1)
	  sprintf(log, "%s can't backward up", identity);
	else if (p->action < 5)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->action = p->action - 5;
	    p->y = p->y + 1;
	    success = 1;
	  }
	break;
      default:
	break;
      }
  }
  return (generate_output(success));
}

char		*left(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*player;
  t_game_info	**info;


  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  success = 0;
  if ((player = (*manager)->get_player(identity)) == NULL)
    return (NULL);
  if ((*info)->game_status && !player->disabled)
    {
      if (player->action < 5)
	sprintf(log, "%s action point are too low", identity);
      else
	{
	  player->action = player->action - 5;
	  player->looking = (player->looking == LEFT) ? DOWN : (player->looking - 1);
	  success = 1;
	}
    }
  my_log(__func__, optional, 3);
  return (generate_output(success));
}

char		*right(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*player;
  t_game_info	**info;

  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  success = 0;
  if ((player = (*manager)->get_player(identity)) == NULL)
    return (NULL);
  if ((*info)->game_status && !player->disabled)
    {
      if (player->action < 5) {
	sprintf(log, "%s action point are too low", identity);
	my_log(__func__, log, 4);
      }
      else
	{
	  player->action = player->action - 5;
	  player->looking = (player->looking == DOWN) ? LEFT : (player->looking + 1);
	  success = 1;
	}
    }
  my_log(__func__, optional, 3);
  return (generate_output(success));
}

char		*leftfwd(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*player;
  t_game_info	**info;


  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  success = 0;
  if ((player = (*manager)->get_player(identity)) == NULL)
    return (NULL);
  if ((*info)->game_status && !player->disabled)
    {
      if (player->action < 10)
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
  my_log(__func__, optional, 3);
  return (generate_output(success));
}

char		*rightfwd(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*player;
  t_game_info	**info;

  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  success = 0;
  if ((player = (*manager)->get_player(identity)) == NULL)
    return (NULL);
  if ((*info)->game_status && !player->disabled)
    {
      if (player->action < 10)
	sprintf(log, "%s action point are too low", identity);
      else
	{
	  sprintf(log, "manager before right in rightfwd, parameter: %s", identity);
	  my_log(__func__, log, 4);
	  if ((right(manager, identity, optional)) == NULL)
	    return (NULL);
	  sprintf(log, "new looking value after right(): %d", player->looking);
	  my_log(__func__, log, 4);
	  if ((forward(manager, identity, optional)) == NULL)
	    return (NULL);
	  success = 1;
	}
    }
  my_log(__func__, optional, 3);
  return (generate_output(success));
}

/*
** looking: renvoit l'orientation du processus (voir include/enum.h pour concordance)
*/
char		*looking(t_game_manager **manager, char *identity, char *optional)
{
  t_player	*p;
  char		look[2];
  t_game_info	**info;


  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  if ((p = (*manager)->get_player(identity)) == NULL)
    return (NULL);
  if ((*manager)->ready && !p->disabled)
    {
      sprintf(look, "%d", p->looking);
      return (generate_output_param(SUCCESS, look));
    }
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
  int		removelink;
  t_game_info	**info;


  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  sprintf(log, "start gather");
  my_log(__func__, log, 3);
  if ((player = (*manager)->get_player(identity)) == NULL){
    return (generate_output(FAIL));
  };
  sprintf(log, "before if");
  my_log(__func__, log, 3);
  if (player->action < 10 || player->disabled)
    return (generate_output(FAIL));
  sprintf(log, "before los energy");
  my_log(__func__, log, 3);
  energy_cells = (*manager)->get_energy_cells();
  sprintf(log, "before ready");
  my_log(__func__, log, 3);
  if ((*info)->game_status)
    {
      sprintf(log, "before is energy cell");
      my_log(__func__, log, 3);
      if ((ec = is_energy_cell(player->x, player->y, energy_cells)) != NULL)
	{
	  sprintf(log, "player %s gathered %d energy", player->identity, ec->value);
	  my_log(__func__, log, 3);
	  player->energy += ec->value;
	  player->action -= 10;
	  sprintf(log, "before get link");
	  my_log(__func__, log, 3);
	  if ((link = get_link_by_content(ec, energy_cells)) != NULL)
	    {
	      removelink = remove_link(&energy_cells, link);
	      if (removelink == 1)
		my_log(__func__, "delete energy cell failed", 2);
	      else if (removelink == -1)
		my_log(__func__, "no more energy_cell on map", 3);
	      else
		my_log(__func__, "energy cell consumed", 3);
	    }
	  else
	    my_log(__func__, "try retrieving t_link by energy_cell content failed", 2);
	  return (generate_output(SUCCESS));
	}
    }
  my_log(__func__, optional, 4);
  return (generate_output(FAIL));
}

/*
** Keep breathing...
*/
char		*watch(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];
  t_player	*p;
  uint		a;
  uint		b;
  int		operator_inv;
  int		swap;
  uint		**zone;
  char		*state;
  t_game_info	**info;

  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  if ((p = (*manager)->get_player(identity)) == NULL)
    return (NULL);
  if ((*info)->game_status && !p->disabled)
    {
      sprintf(log, "processus %s is watching", identity);
      my_log(__func__, log, 4);
      a = (p->looking == LEFT || p->looking == RIGHT) ? p->x : p->y;
      b = (p->looking == LEFT || p->looking == RIGHT) ? p->y : p->x;
      operator_inv = (p->looking == RIGHT || p->looking == DOWN) ? 1 : 0;
      swap = (p->looking == UP || p->looking == DOWN) ? 1 : 0;
      if ((zone = eq_watch(a, b, operator_inv, swap)) == NULL)
	return (NULL);
      if ((state = compile_watch_return(manager, zone)) == NULL)
	{
	  my_log(__func__, "compiling watch return failed", 2);
	  return (NULL);
	}
      return (generate_output_param(SUCCESS, state));
    }
  my_log(__func__, optional, 4);
  return (generate_output(FAIL));
}

char		*attack(t_game_manager **manager, char *identity, char *optional)
{
  char		*watched;
  char		*front;
  t_player	*p;
  t_player	*adv;
  t_game_info	**info;

  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  if ((p = (*manager)->get_player(identity)) == NULL)
    return (NULL);
  if ((*info)->game_status && !p->disabled) {
     if ((watched = watch(manager, identity, optional)) == NULL)
	return (NULL);
      if ((front = extract_front_from_watched(watched)) == NULL)
	return (NULL);
      if (p->action < 5)
	return (generate_output(FAIL));
      p->action -= 5;
      p->energy -= 2;
      if ((adv = (*manager)->get_player(front)) == NULL)
	return (NULL);
      adv->disabled = 2;
      return (generate_output(SUCCESS));
    }
  my_log(__func__, optional, 4);
  return (generate_output(FAIL));
}

char		*jump(t_game_manager **manager, char *identity, char *optional)
{
  int		success;
  char		log[50];
  t_player	*p;
  t_game_info	**info;

  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  success = 0;
  if ((p = (*manager)->get_player(identity)) == NULL)
    return (NULL);
  if ((*info)->game_status && !p->disabled) {    
    if ((p = (*manager)->get_player(identity)) == NULL)
      return (NULL);
    switch (p->looking)
      {
      case LEFT:
	if (check_mvmnt(p->x - 2, p->y, identity, (*manager)) == 1)
	  sprintf(log, "%s can't jump left", identity);
	else if (p->energy < 2)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->energy = p->energy - 2;
	    p->x = p->x - 2;
	    success = 1;
	  }
	break;
      case UP:
	if (check_mvmnt(p->x, p->y - 2, identity, (*manager)) == 1)
	  sprintf(log, "%s can't jump  up", identity);
	else if (p->energy < 2)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->energy = p->energy - 2;
	    p->y = p->y - 2;
	    success = 1;
	  }
	break;
      case RIGHT:
	if (check_mvmnt(p->x + 2, p->y, identity, (*manager)) == 1)
	  sprintf(log, "%s can't jump right", identity);
	else if (p->energy < 2)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->energy = p->energy - 2;
	    p->x = p->x + 2;
	    success = 1;
	  }
	break;
      case DOWN:
	if (check_mvmnt(p->x, p->y + 2, identity, (*manager)) == 1)
	  sprintf(log, "%s can't jump down", identity);
	else if (p->energy < 2)
	  sprintf(log, "%s action point are too low", identity);
	else
	  {
	    p->energy = p->energy - 2;
	    p->y = p->y + 2;
	    success = 1;
	  }
	break;
      default:
	break;
      }
  }
  my_log(__func__, optional, 3);
  return (generate_output(success));
}

char		*self_stats(t_game_manager **manager, char *identity, char *optional)
{
  char		str[15];
  t_player	*p;
  t_game_info	**info;

  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  if ((p = (*manager)->get_player(identity)) == NULL)
    return (NULL);
  if ((*info)->game_status)
    {
      sprintf(str, "%d", p->energy);
      return (generate_output_param(SUCCESS, str));
    }
  my_log(__func__, optional, 4);
  return (generate_output(FAIL));
}

char		*inspect(t_game_manager **manager, char *identity, char *optional)
{
  char		log[50];
  t_player	*p;
  t_player	*adv;
  t_game_info	**info;


  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  if ((p = (*manager)->get_player(identity)) == NULL)
	return (NULL);
  if ((*info)->game_status && p->action > 5 && !p->disabled)
    {
      if ((p = (*manager)->get_player(identity)) == NULL)
	return (NULL);
      if ((adv = (*manager)->get_player(optional)) == NULL)
	{
	  sprintf(log, "%s?", optional);
	  return (generate_output_param(FAIL, log));
	}
      else
	{
	  sprintf(log, "%d", adv->energy);
	  return (generate_output_param(SUCCESS, log));
	}
    }
  return (generate_output(FAIL));
}

char		*next(t_game_manager **manager, char *identity, char *optional)
{
  t_player	*p;
  t_game_info	**info;

  if ((info = (*manager)->get_info()) == NULL)
    return (NULL);
  if ((*info)->game_status)
    {
      if ((p = (*manager)->get_player(identity)) == NULL)
	return (NULL);
      p->action = 0;
      return (generate_output(SUCCESS));
    }
  my_log(__func__, optional, 4);
  return (generate_output(FAIL));
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
