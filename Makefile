# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nagiorgi <nagiorgi@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/01 10:48:43 by nagiorgi          #+#    #+#              #
#    Updated: 2023/11/01 10:50:32 by nagiorgi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Get_Next_Line
SRC = source.c autre_source.c
OBJ = $(SRC:.c=.o)
LIB = -L./libft -lft
FLAGS = -Wall -Wextra -Werror
INCLUDES = -I./libft

all: $(NAME)

$(NAME): $(OBJ)
    make -C ./libft
    gcc $(FLAGS) -o $(NAME) $(OBJ) $(LIB)

%.o: %.c
    gcc $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
    make clean -C ./libft
    rm -f $(OBJ)

fclean: clean
    make fclean -C ./libft
    rm -f $(NAME)

re: fclean all