NAME =		ft_ping

CC =		clang

CFLAGS =	-Wall -Wextra -Werror

SRCS =		src/main.c src/flags.c src/address.c src/socket.c src/signal.c \
			src/loop.c src/time.c src/display.c

LIBFT =		libft
INC =		ft_ping.h

OBJS =		$(SRCS:.c=.o)


.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	@make -C $(LIBFT)
	@ $(CC) $(CFLAGS) -o $(NAME) $(SRCS) -Iinclude $(LIBFT)/libft.a -lm
	@echo "\033[1;32mprogram...compiled\t✓\033[0m"

clean:
	@rm -rf $(OBJS)
	@make clean -C $(LIBFT)
	@echo "\033[1;34mft_ping\033[1;33m obj files removed\t\033[1;31m✓\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C $(LIBFT)
	@echo "\033[1;34mft_ping\033[1;33m files deleted\t\t\033[1;31m✓\033[0m"

re: fclean all