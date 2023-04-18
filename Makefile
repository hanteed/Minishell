SRC =   main.c \
	include/mystring.c \
	include/shellcommands.c \
	include/shellwraps.c \
	include/shellencryption.c \

OBJ = $(SRC:.c=.o)

CFLAGS += -Wextra

NAME = minishell

$(NAME): $(OBJ)
	gcc -o $(NAME) $(OBJ) -lsodium

all:     $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: 	fclean all
