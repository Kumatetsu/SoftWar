/*
** hash.h for SoftWar in 
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Sun Jul 16 01:19:47 2017 CASTELLARNAU Aurelien
** Last update Sun Jul 16 01:36:40 2017 CASTELLARNAU Aurelien
*/

#ifndef  _HASH_H_
# define _HASH_H_
/*
** is_colision()
*/
# define NOT_COLISION 222
/*
** constante arbitraire pour la gestion
** erreur du processus de hashage (on ne peut
** gérer avec 0 et 1 qui sont des index).
*/
# define HCOMMANDERR 160 
/*
** nombre de fonctions dans command.c
*/
# define HCOMMANDSIZE 9
/*
** code pour signifier que ce n'est pas une commande
*/
# define HCOMMANDUNKNOW 61 

typedef struct	s_colision
{
  int		hash;
  char		*command;
}		t_colision;

int	hash_command(char *command);
void	free_hashes();

#endif   /* !_HASH_H_ */
