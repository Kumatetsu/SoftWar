/*
** main.c for SoftWar in /home/code/SoftWar/aurelien
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sat Jul  1 01:11:51 2017 CASTELLARNAU Aurelien
** Last update Sat Jul  1 02:29:20 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include <stdlib.h>
#include <czmq.h>
#include <json/json.h>
#include "libmy.h"
#include "argument.h"
#include "Softwar_ctx.h"
#include "Notification.h"
#include "Game_info.h"
#include "rep.h"
#include "pub.h"
#include "poll.h"

int		main(int argc, char *argv[])
{
  /*
  ** Server REP/REQ, PUB/SUB and polling management
  */
  t_swctx	*ctx;
  char		*message = NULL;
  zsock_t	*publisher;
  t_game_info	*game_info;

  if (sw_parse(argc, argv))
    my_log(__func__, "failed to parse SoftWar arguments", 2);
  ctx = get_swctx(); // à ce moment là, soit ctx est set par les arguments
		     // soit on récupère ici la valeur par défaut.
    /*
  ** Serialization example:
  */
  char *str = "{'name':'joys of promagramming'}";
  json_object *jobj = json_tokener_parse(str);
  enum json_type type = json_object_get_type(jobj);

  switch (type) {
  case json_type_null: printf("json_type_nulln");
    break;
  case json_type_boolean: printf("json_type_booleann");
    break;
  case json_type_double: printf("json_type_doublen");
    break;
  case json_type_int: printf("json_type_intn");
    break;
  case json_type_object: printf("json_type_objectn");
    break;
  case json_type_array: printf("json_type_arrayn");
    break;
  case json_type_string: printf("json_type_stringn");
    break;
  }

  /*
  ** Implémentation d'une sérialisation sur la structure game info:
  */
  if ((game_info = init_game_info(5, 0, "Poney Content", 8)) == NULL)
    return (1);
  printf("\nStringified game_info: %s", game_info_to_json(game_info));
  /*
  ** REP/REQ server init
  ** voir /src/server/rep.c et pub.c
  */
  if (add_link(&(ctx->sockets), create_socket("responder", init_rep(ctx->rep_port))))
    return (1);
  /*
  ** PUB/SUB server init
  */
  if (add_link(&(ctx->sockets), create_socket("publisher", init_pub(ctx->pub_port))))
    return (1);
  while(!zsys_interrupted)
    {
      /*
      ** Poll init
      */
      message = init_poll(&ctx);
      /*
      ** Publication to all clients:
      ** The sender should receive his message 2 times,
      ** other clients only once from publisher.
      */
      my_log(__func__, "publish message: ", 3);
      my_log(__func__, message, 3);
      /*
      ** Là on a besoin d'un moyen de récupérer les socket facilement...
      */
      publisher = get_socket("publisher", ctx);
      zstr_sendf(publisher, "SoftWar %s", message);
      zstr_free(&message);
    }
  /*
  ** libmy extended cleaning
  ** options are cleaned in /src/tools/argument.c
  */
  delete_logger();
  /*
  ** lol, on dégage tout avec une fonction
  ** re lol reste à bien vérifier que ca fait le taf ^^
  */
  free_ctx();
  return(0);
}
