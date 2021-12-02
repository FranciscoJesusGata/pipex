# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fgata-va <fgata-va@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/23 15:56:31 by fgata-va          #+#    #+#              #
#    Updated: 2021/12/02 16:33:01 by fgata-va         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := gcc

CFLAGS := -Wall -Werror -Wextra -I. -g

GNLDIR := 42_file-reader

GNL := $(addprefix $(GNLDIR)/, get_next_line.c)

SRC := pipex.c command_handler.c pipex_utils.c executer.c heredoc.c

NAME := pipex

LIBFT = -LLibft -lft

OBJS := $(SRC:.c=.o) $(GNL:$(GNLDIR)/%.c=%.o)

all: $(NAME)

bonus: CFLAGS += -D BONUS=1
bonus: all

echo:
	echo $(OBJS)

%.o: $(GNLDIR)/%.c
	$(CC) $(CFLAGS)   -c -o $@ $<

$(NAME): libft $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(GNL_OBJS) $(LIBFT) -o $(NAME)

libft:
	$(MAKE) -C Libft/

clean:
	rm -rf $(OBJS)
	$(MAKE) -C Libft/ clean

fclean: clean
	rm -rf $(NAME) $(NAME).dSYM
	$(MAKE) -C Libft/ fclean

re: fclean all

.PHONY: all libft clean fclean re
