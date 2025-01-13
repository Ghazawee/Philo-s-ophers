NAME = philo

SRC = mainlo.c \
	pharse.c \
	utils.c\
	free_error.c\
	philo_routine.c\

OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Werror -Wextra -fanalyzer -g3 -flto

DEL = rm -rf

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(DEL) $(OBJ)

fclean: clean
	$(DEL) $(NAME)

re: fclean all

.PHONY: all clean fclean re
