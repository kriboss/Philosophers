# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/28 18:07:11 by kbossio           #+#    #+#              #
#    Updated: 2025/09/24 10:48:56 by kbossio          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc -Wall -Wextra -Werror
INCLUDES = -I.
SRCS = philosophers.c utils.c routine.c monitor.c routils.c
OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(OBJS) -lpthread -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
