
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -D_DEFAULT_SOURCE -Iinclude
NAME = netcap
SRC = src/main.c src/ethernet.c src/ipv4.c src/protocol.c src/tcp.c

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm -f $(NAME)

re: clean all

.PHONY: all clean re
