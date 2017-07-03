/*
** log_panel.c for libmy_extended in /home/aurelien/Documents/demoLib/demoParse/libmy
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Apr 12 23:35:39 2017 CASTELLARNAU Aurelien
** Last update Mon Apr 17 21:07:41 2017 CASTELLARNAU Aurelien
*/

#include <stdio.h>
#include "libmy.h"

void    log_error(char *str, FILE *o)
{
  
  fprintf(o, "< ERROR > ");
  fprintf(o, str);
}

void    log_warning(char *str, FILE *o)
{
  fprintf(o, "< WARNING > ");
  fprintf(o, str);
}

void    log_info(char *str, FILE *o)
{
  fprintf(o, "< INFO > ");
  fprintf(o, str);
}

void    log_debug(char *str, FILE *o)
{
  fprintf(o, "< DEBUG > ");
  fprintf(o, str);
}
