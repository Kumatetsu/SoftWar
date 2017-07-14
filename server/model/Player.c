/*
** Player.c for Softwar in /home/naej/soft_war/SoftWar/server/model
** 
** Made by BILLAUD Jean
** Login   <billau_j@etna-alternance.net>
** 
** Started on  Tue Jul 11 17:07:34 2017 BILLAUD Jean
** Last update Wed Jul 12 16:55:27 2017 BILLAUD Jean
*/

#include <stdio.h>
#include <json/json.h>
#include "libmy.h"
#include "Player.h"

/*
** Crée une structure player
** Set avec son nom et son
** energie.
 */
t_player	*create_player(char *identity)
{
  t_player	*p;

  if ((p = malloc(sizeof (*p))) == NULL)
    {
      my_log(__func__, MEM_ERR, 1);
      return (NULL);
    }
  if ((p->name = my_strdup(name)) == NULL)
    return (NULL);
  p->energy = 50;
  return (p);
}

void	free_player(t_player *p)
{
  free(p->name);
  free(p);
}

json_object	*player_to_json(t_player *player)
{
  json_object	*player_json;
  json_object	*identity_json;
  json_object	*x_json;
  json_object	*y_json;
  json_object	*energy_json;
  json_object	*looking_json;

  player_json	= json_object_new_object();
  identity_json = json_object_new_string(player->identity);
  x_json	= json_object_new_int(player->x);
  y_json	= json_object_new_int(player->y);
  energy_json	= json_object_new_int(player->energy);
  looking_json	= json_object_new_int(player->looking);
  json_object_object_add(player_json, "identity", identity_json);
  json_object_object_add(player_json, "x", x_json);
  json_object_object_add(player_json, "y", y_json);
  json_object_object_add(player_json, "energy", energy_json);
  json_object_object_add(player_json, "looking", looking_json);
  return (player_json);
}

json_object	*players_to_json(t_chain *players)
{
  t_link	*tmp;
  t_player	*p;
  json_object	*players_json;

  players_json = json_object_new_array();
  tmp = players->first;
  while(tmp)
    {
      p = (t_player*)(tmp->content);
      json_object_array_add(players_json, player_to_json(p));
      tmp = tmp->next;
    }
  return (players_json);
}
