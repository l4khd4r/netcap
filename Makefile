
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -D_DEFAULT_SOURCE -Iinclude
NAME = netcap
SRC = src/main.c src/ethernet.c src/ipv4.c src/protocol.c src/tcp.c src/udp.c src/icmp.c src/stats.c


TEST_NAME = test_ipv4
TEST_SRC = tests/test_ipv4.c src/ipv4.c

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

$(TEST_NAME): $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_NAME)

test: $(TEST_NAME)
	./$(TEST_NAME)



clean:
	rm -f $(NAME) $(TEST_NAME)

re: clean all

.PHONY: all clean re test
