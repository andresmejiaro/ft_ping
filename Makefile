NAME = ft_ping

MAIN = main.c

SRCS = ping.c packages.c parsing.c send_recieve.c

CC = gcc

OBJS = ${SRCS:.c=.o}

MAINOBJ = ${MAIN:.c=.o}

CFLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJS) $(MAINOBJ)
	$(CC) $(CFLAGS) $(OBJS) $(MAINOBJ) -o $@
	$(MAKE) cap

cap: $(NAME)
	sudo setcap cap_net_raw+ep $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(MAINOBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re cap
