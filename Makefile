NAME = ft_ping

MAIN = main.c

SRCS = 

CC = gcc

OBJS = ${SRCS:.c=.o}

MAINOBJ = ${MAIN:.c=.o}

CFLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJS) $(MAINOBJ) 
	$(CC) $(CFLAGS) $(OBJS) $(MAINOBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(MAINOBJ) $(TESTOBJ)

fclean: clean
	rm -f $(NAME) $(TEST) $(NAME_DEBUG)

re: fclean all

.PHONY: all clean fclean re