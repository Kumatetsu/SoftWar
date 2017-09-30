/*
** softwar_test.c for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Sep 27 13:25:49 2017 CASTELLARNAU Aurelien
** Last update Wed Sep 27 16:05:05 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include <stdlib.h>
#include "libmy.h"
#include "softwar_ctx.h"
#include "game_manager.h"
#include "exec.h"
#include "softwar_test.h"

void	init_players(t_game_manager **manager)
{
  char	*id;
  
  if ((id = my_strdup("0x01")) == NULL)
    return;
  identify(manager, "foo", id);
  id = "0x02\0";
  identify(manager, "foo", id);
  id = "0x03\0";
  identify(manager, "foo", id);
  id = "0x04\0";
  identify(manager, "foo", id);
  id = "0x05\0";
  identify(manager, "foo", id);
  (*manager)->set_players_pos((*manager)->get_players(), 4);
}

t_player	**make_test_context(t_game_manager **manager)
{
  t_game_info	**gi;
  t_command	**commands;
  t_player	**p_adv;
  t_player	*player;
  t_player	*adversary;
  t_energy_cell	*ec;
  
  /*                   
  ** pour init le hashage des commandes                     
  */
  commands = get_commands();
  if (commands == NULL)
    my_log(__func__, "im a shitty action to imply commands and compile", 4);
  if ((gi = (*manager)->get_info()) == NULL)
    return (NULL);
  /*                   
  ** on get un player, on le place en x2y0
  ** on le fait regarder en bas                     
  */
  if ((player = (*manager)->get_player("0x01")) == NULL)
    {
      my_log(__func__, "fail retrieving player by id 0x01", 4);
      return (NULL);
    }
  player->looking = 3;
  player->x = 2;
  player->y = 0;
  player->action = 5000;
  /*                   
  ** on ajoute une energy cell de value 2017 juste en face de lui
  ** position 1 dans le tableau de watch                     
  */
  if ((ec = create_energy_cell(2, 1, 2017)) == NULL)
    {
      my_log(__func__, "fail creating energy cell", 4);
      return (NULL);
    }
  if (add_link(&((*gi)->energy_cells), ec))
    {
      my_log(__func__, "failed adding energy cell in front of player", 4);
      return (NULL);
    }
  /*                  
  ** on déplace un joueur en diagonale droite de lui
  ** position 4 dans le tableau attendu après watch
  */
  if ((adversary = (*manager)->get_player("0x02")) == NULL)
    {
      my_log(__func__, "failed retrieving adversary with id 0x02!", 4);
      return (NULL);
    }
  adversary->x = 1;
  adversary->y = 2;
  if ((p_adv = malloc(sizeof(t_player*) * 2)) == NULL)
    return (NULL);
  int i;
  for (i = 0; i < 2; i++)
    if ((p_adv[i] = malloc(sizeof(t_player))) == NULL)
      return (NULL);
  p_adv[0] = player;
  p_adv[1] = adversary;
  return (p_adv);
}

int	assert_str(char *expected, char *must_be)
{
  int	valid;
  char	log[100];
  
  valid = my_strcmp(expected, must_be);
  if (!valid)
    sprintf(log, "TEST OK: %s == %s", expected, must_be);
  else
    sprintf(log, "TEST FAILED: %s != %s", expected, must_be);
  return (valid);
}

int	assert_command(t_game_manager **manager, char *command, char *target, char *must_be)
{
  char	*to_test;
  char	log[100];

  if ((to_test = exec(command, manager, target)) == NULL)
    return (1);
   if (my_strcmp(to_test, must_be) != 0)
     sprintf(log, "TEST on %s FAILED \nexpected: %s \noutput: %s", command, must_be, to_test);
  else
    sprintf(log, "TEST on %s SUCCEED", command);
   my_log(__func__, log, 3);
  return (0);
}

int		test_softwar(t_game_manager **manager)
{
  char		*id;
  t_player	**p_adv;
  t_player	*player;
  t_player	*adversary;
  char		expected[100];

  my_log(__func__, "SOFTWAR TESTING PROCESS", 3);
  init_players(manager);
  if ((p_adv = make_test_context(manager)) == NULL)
    return (1);
  player = p_adv[0];
  adversary = p_adv[1];
  id = player->identity;

  // WATCH
  assert_command(manager, "watch", id, "ok|[\"energy\", \"empty\", \"empty\", \"0x02\"]");
  // FORWARD
  assert_command(manager, "forward", id, "ok|");
  // CHECK ENERGY
  sprintf(expected, "%d", player->energy);
  assert_str(expected, "50");
  // GATHER
  assert_command(manager, "gather", id, "ok|");
  // CHECK ENERGY
  sprintf(expected, "%d", (player->energy + 2017));
  assert_str(expected, "2067");
  // CHECK POSITION AND LOOKING
  sprintf(expected, "%d", player->x);
  assert_str(expected, "2");
  sprintf(expected, "%d", player->y);
  assert_str(expected, "1");
  sprintf(expected, "%d", player->looking);
  assert_str(expected, "3");
  // LEFTFWD
  assert_command(manager, "leftfwd", id, "ok|");
  // CHECK POSITION AND LOOKING
  sprintf(expected, "%d", player->x);
  assert_str(expected, "3");
  sprintf(expected, "%d", player->y);
  assert_str(expected, "1");
  sprintf(expected, "%d", player->looking);
  assert_str(expected, "2");
  // RIGHTFWD
  assert_command(manager, "rightfwd", id, "ok|");
  // CHECK POSITION AND LOOKING
  sprintf(expected, "%d", player->x);
  assert_str(expected, "3");
  sprintf(expected, "%d", player->y);
  assert_str(expected, "2");
  sprintf(expected, "%d", player->looking);
  assert_str(expected, "3");
  // RIGHT
  assert_command(manager, "right", id, "ok|");
  // CHECK POSITION AND LOOKING
  sprintf(expected, "%d", player->x);
  assert_str(expected, "3");
  sprintf(expected, "%d", player->y);
  assert_str(expected, "2");
  sprintf(expected, "%d", player->looking);
  assert_str(expected, "0");
  // FORWARD
  assert_command(manager, "forward", id, "ok|");
  // CHECK POSITION AND LOOKING
  sprintf(expected, "%d", player->x);
  assert_str(expected, "2");
  sprintf(expected, "%d", player->y);
  assert_str(expected, "2");
  sprintf(expected, "%d", player->looking);
  assert_str(expected, "0");
  // WATCH
  assert_command(manager, "watch", id, "ok|[\"0x02\", \"empty\", \"empty\", \"empty\"]");
  // INSPECT|0x02
  assert_command(manager, "inspect|0x02", id, "ok|50");
  // ATTACK
  assert_command(manager, "attack", id, "ok|");
  // JUMP
  assert_command(manager, "jump", id, "ok|"); 
  // CHECK POSITION AND LOOKING
  sprintf(expected, "%d", player->x);
  assert_str(expected, "0");
  sprintf(expected, "%d", player->y);
  assert_str(expected, "2");
  sprintf(expected, "%d", player->looking);
  assert_str(expected, "0");
  // CHECK ENERGY
  sprintf(expected, "%d", (player->energy - 2));
  assert_str(expected, "2065");
  // ERROR ON JUMP
  player->looking = 1;
  adversary->x = 0;
  adversary->y = 0;
  assert_command(manager, "jump", id, "ko|"); 
  // CHECK POSITION AND LOOKING
  sprintf(expected, "%d", player->x);
  assert_str(expected, "0");
  sprintf(expected, "%d", player->y);
  assert_str(expected, "2");
  sprintf(expected, "%d", player->looking);
  assert_str(expected, "1");
  // NEXT
  assert_command(manager, "next", id, "ok|");
  // CHECK ACTIONS PTS
  sprintf(expected, "%d", player->action);
  assert_str(expected, "0");
  // LEAVE
  assert_command(manager, "leave", id, "ok|");
  // SELFSTAT
  assert_command(manager, "selfstat", id, "ok|2065");
  return (0);
}
