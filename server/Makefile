##
## Makefile for libmy_extended in /home/aurelien/Documents/Lib_etna/castel_a/libmy_extended
## 
## Made by CASTELLARNAU Aurelien
## Login   <castel_a@etna-alternance.net>
## 
## Started on  Wed Feb  1 09:37:15 2017 CASTELLARNAU Aurelien
## Last update Wed Sep 13 16:20:57 2017 BILLAUD Jean
##

CC      =       gcc

NAME    =       sw_server

LIB     =       ./libmy

CFLAGS  =       -W -Wextra -Werror -Wall -g -Iinclude/ -Ilibmy $(shell pkg-config --cflags json-c)

LDFLAGS =       -lzmq -lczmq -lpthread -L ./libmy -lmy $(shell pkg-config --libs json-c)

SRC     =       main.c \
		src/tools/argument.c \
		src/tools/hash.c \
		model/softwar_ctx.c \
		model/notification.c \
		model/player.c \
		model/energy_cell.c \
		model/game_info.c \
		model/game_manager.c \
		model/map_manager.c \
		model/thread_data.c \
		src/server/pub.c \
		src/server/rep.c \
		src/server/router.c \
		src/server/poll.c \
		src/server/utils.c \
		src/server/command.c \
		src/server/map.c \
		src/server/game_status.c \
		src/server/exec.c \
		src/server/runtime.c \
		src/server/thread.c \
		src/test/softwar_test.c \

OBJ     =       ${SRC:%.c=%.o}

RM      =       rm -f

all	:	lib	$(NAME)

lib	:
		$(MAKE)	-C	$(LIB)

libPar	:
		$(MAKE)	-C	$(LIBPAR)

re	:	clean	all

$(NAME)	:	$(OBJ)
		$(CC)	$(CFLAGS)	$(OBJ)	$(LDFLAGS)	-o	$(NAME)

clean	:
		$(RM)	$(OBJ)

fclean	:	clean
		$(RM)	$(NAME)

.PHONY	:	all	clean	fclean
