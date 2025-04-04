# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/28 18:07:11 by kbossio           #+#    #+#              #
#    Updated: 2025/03/28 18:25:31 by kbossio          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc -Wall -Wextra -Werror
INCLUDES = -I. -Ilibft
SRCS = philosophers.c
OBJS = $(SRCS:.c=.o)

%.o: %.c | make_libft
	$(CC) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS) | make_libft
	$(CC) $(OBJS) -Llibft -lft -o $(NAME)

all: $(NAME)

make_libft:
	make -C libft

clean:
	rm -f $(OBJS)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re
