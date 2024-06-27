
CC = cc
FLAGS = -Wall -Wextra -Werror 
NAME = minishell
LIBFT = libft/libft.a
LIBFT_DIR = ./libft

SRC = $(shell find src -name '*.c')


OBJ = $(SRC:.c=.o)


# 规则：编译所有文件并生成主程序
all: $(NAME)

# 通用规则：将 .c 文件编译为 .o 文件
%.o : %.c
	$(CC) $(FLAGS) -g -Iinclude -I$(LIBFT_DIR) -c $< -o $@

# 规则：编译 libft 库
$(LIBFT):
	make -C $(LIBFT_DIR)

# 规则：生成主程序
$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(FLAGS) -g $(OBJ) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

# 清理所有生成的 .o 文件
clean:
	rm -rf $(OBJ)
	make -C $(LIBFT_DIR) clean

# 清理所有生成的文件
fclean: clean
	rm -rf $(NAME) 
	make -C $(LIBFT_DIR) fclean

# 重新编译所有内容
re: fclean all

.PHONY: clean fclean re all