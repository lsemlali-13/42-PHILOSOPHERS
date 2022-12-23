CFLAGS = -Wall -Wextra -Werror
CC = cc #-g -fsanitize=thread
SRC = philo.c libft/libft.a

NAME = philo
OBJ = $(SRC:.c=.o)

all : $(NAME)
$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

fclean :
	rm *.o philo
re : fclean all