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
#include <time.h>
#include "libmy.h"

/*
** https://stackoverflow.com/questions/1442116/how-to-get-date-and-time-value-in-c-program
*/
void	print_time(FILE *o)
{
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char s[64];
  strftime(s, sizeof(s), "%c", tm);
  fprintf(o, "%s ", s);
}

void    log_error(char *str, FILE *o)
{
  print_time(o);
  fprintf(o, "< ERROR > ");
  fprintf(o, str);
}

void    log_warning(char *str, FILE *o)
{
  print_time(o);
  fprintf(o, "< WARNING > ");
  fprintf(o, str);
}

void    log_info(char *str, FILE *o)
{
  print_time(o);
  fprintf(o, "< INFO > ");
  fprintf(o, str);
}

void    log_debug(char *str, FILE *o)
{
  print_time(o);
  fprintf(o, "< DEBUG > ");
  fprintf(o, str);
}
