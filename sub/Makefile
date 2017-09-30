##
## Makefile for libmy_extended in /home/aurelien/Documents/Lib_etna/castel_a/libmy_extended
## 
## Made by CASTELLARNAU Aurelien
## Login   <castel_a@etna-alternance.net>
## 
## Started on  Wed Feb  1 09:37:15 2017 CASTELLARNAU Aurelien
## Last update Wed Sep 13 12:23:03 2017 BILLAUD Jean
##

CC      =       gcc

NAME    =       test_serverpub

CFLAGS  =       -W -Wextra -Werror -Wall 

LDFLAGS =       -lzmq -lczmq

SRC     =       main.c

OBJ     =       ${SRC:%.c=%.o}

RM      =       rm -f

all	:	$(NAME)


re	:	clean	all

$(NAME)	:	$(OBJ)
		$(CC)	$(CFLAGS)	$(OBJ)	$(LDFLAGS)	-o	$(NAME)

clean	:
		$(RM)	$(OBJ)

fclean	:	clean
		$(RM)	$(NAME)

.PHONY	:	all	clean	fclean
