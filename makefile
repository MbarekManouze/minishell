# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmanouze <mmanouze@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/10 11:43:45 by ressalhi          #+#    #+#              #
#    Updated: 2022/08/21 13:19:31 by mmanouze         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -lreadline -Wall -Wextra -Werror #-g -fsanitize=address
SRC =	main.c ft_split.c expand.c env.c errors.c parsing.c redirections.c \
		remove_trash.c utils.c utils2.c dollar.c dollar2.c dollar_utils.c \
		unset.c echo.c ft_export.c ft_export2.c builtins.c pipes.c pipe_utils.c \
		here_doc.c ft_itoa.c ft_cd.c ft_exit.c wait.c files.c child_parent.c fullfill.c \
		valhala.c dedicate.c signal.c \

all : $(NAME)

$(NAME) :
	$(CC) $(CFLAGS) -L/Users/$(USER)/.brew/opt/readline/lib -I/Users/$(USER)/.brew/opt/readline/include $(SRC) -o $(NAME) 

clean :
	rm -rf $(NAME)

fclean : clean

re : fclean all

.PHONY : all clean fclean re
