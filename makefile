CFLAGS = -Wall -Wextra -Werror
CC = cc
SRC = philo.c ft_atoi.c philo_utils.c routine.c

NAME = philo
OBJ = $(SRC:.c=.o)

all : $(NAME)
$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

fclean :
	rm *.o philo
re : fclean all