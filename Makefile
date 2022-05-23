##
## EPITECH PROJECT, 2021
## Makefile
## File description:
## Compile projects
##

SRC		=		$(shell find src -name "*.c")

T_SRC   =		$(shell find src -name "*.c")

FUNC_SRC =		$(shell find src -name "*.c" -not -name "main.c")

CFLAGS = -I ./include -Wall -Wextra -g3

OBJ		=		$(SRC:.c=.o)

NAME	=		quick_norme

DEBUG    =    0

ifeq ($(DEBUG), 1)
		CFLAGS += -g3
endif

all: 	$(NAME)

$(NAME):
		@$(MAKE) -s $(OBJ)
		gcc -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re: fclean all
