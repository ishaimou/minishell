# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/29 02:50:05 by ishaimou          #+#    #+#              #
#    Updated: 2020/02/02 11:16:41 by ishaimou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BGREEN = \033[1;32m
BCYAN = \033[1;36m
RESET = \033[0m
BEER = 🍻
BOOKS = 📚i
CLEAN = ✅
FCLEAN = 😥

NAME = minishell
LIB_NAME = libft.a
LIB_DIR = ./libft
LIB = -L ./libft -lft
H_LIB_DIR = ./libft
H_DIR = ./includes
FLAGS = -Wall -Werror -Wextra

CC = gcc

SRCS = 	main.c init.c tools.c tools2.c utils.c free.c error.c diclist.c\
		builtins.c builtin_cd.c builtin_alias.c builtins_env.c\
		read_cmd.c parse_exec.c launch.c prompt.c expansion.c alias_utils.c

SRCS_DIR = $(addprefix srcs/, $(SRCS))

SRCS_O = $(SRCS_DIR: %.c=%.o)

all: $(NAME)

$(NAME): $(SRCS_O)
	@make -C $(LIB_DIR)
	@echo "$(BOOKS) $(BGREEN)$(LIB_NAME) has been created successfully.$(RESET)"
	@$(CC) -o $(NAME) $(SRCS_O) -I $(H_DIR) -I $(H_LIB_DIR) $(LIB) $(FLAGS)
	@echo "$(BEER)  $(BGREEN)$(NAME) has been created successfully.$(RESET)"

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@ 

clean:
	@make clean -C $(LIB_DIR)
	@echo "$(CLEAN)  $(BCYAN)Object files has been cleaned successfully.$(RESET)"

fclean: clean
	@make fclean -C $(LIB_DIR)
	@rm -rf $(NAME)
	@echo "$(FCLEAN)  $(BCYAN)$(NAME) cleaned successfully.$(RESET)"

re: fclean all
