# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fgata-va <fgata-va@student.42madrid>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/23 15:56:31 by fgata-va          #+#    #+#              #
#    Updated: 2021/06/23 16:27:18 by fgata-va         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc

CFLAGS := -Wall -Werror -Wextra -I. -g

SRC = pipex.c

NAME = pipex

LIBFT = -LLibft -lft

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): libft $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

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
