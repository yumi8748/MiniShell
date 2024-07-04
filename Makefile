# Compiler
CC = cc

# Compiler flags
FLAGS = -Wall -Wextra -Werror

# Executable name
NAME = minishell

# Libft
LIBFT = libft/libft.a
LIBFT_DIR = ./libft

# Source directories
SRC_DIR = ./src
BUILTINS_DIR = $(SRC_DIR)/builtins
EXECUTION_DIR = $(SRC_DIR)/execution
EXPANSION_DIR = $(SRC_DIR)/expansion
HEREDOC_DIR = $(SRC_DIR)/heredoc
PARSER_DIR = $(SRC_DIR)/parser
SIGNALS_DIR = $(SRC_DIR)/signals
UTILS_DIR = $(SRC_DIR)/utils

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/minishell.c \
       $(SRC_DIR)/prompt.c \
       $(BUILTINS_DIR)/cd.c \
       $(BUILTINS_DIR)/echo.c \
       $(BUILTINS_DIR)/env.c \
       $(BUILTINS_DIR)/exit.c \
       $(BUILTINS_DIR)/export.c \
       $(BUILTINS_DIR)/pwd.c \
       $(BUILTINS_DIR)/unset.c \
       $(EXECUTION_DIR)/execute_multi_cmds.c \
       $(EXECUTION_DIR)/execute_multi_cmds_utils.c \
       $(EXECUTION_DIR)/execute_one_cmd.c \
       $(EXECUTION_DIR)/execute_one_cmd_utils.c \
       $(EXECUTION_DIR)/redirection_multi_cmds.c \
       $(EXECUTION_DIR)/redirection_one_cmd.c \
       $(EXECUTION_DIR)/redirection_utils.c \
	   $(EXECUTION_DIR)/redirection_utils2.c \
       $(EXPANSION_DIR)/expansion.c \
       $(EXPANSION_DIR)/expansion_utils.c \
	   $(EXPANSION_DIR)/expansion_utils2.c \
       $(HEREDOC_DIR)/heredoc.c \
       $(HEREDOC_DIR)/heredoc_utils.c \
       $(PARSER_DIR)/parse_cmd_utils2.c \
	   $(PARSER_DIR)/parse_cmd_utils3.c \
       $(PARSER_DIR)/parse_cmd_utils.c \
       $(PARSER_DIR)/parse_path.c \
       $(PARSER_DIR)/parse_path_utils.c \
	   $(PARSER_DIR)/thild.c \
       $(SIGNALS_DIR)/signals.c \
       $(UTILS_DIR)/check_input_utils2.c \
       $(UTILS_DIR)/check_input_utils.c \
       $(UTILS_DIR)/cleaner.c \
       $(UTILS_DIR)/close_fd.c \
       $(UTILS_DIR)/input_errors.c \
	   $(UTILS_DIR)/input_errors2.c \
	   $(UTILS_DIR)/input_error3.c \
       $(UTILS_DIR)/minienv_init.c \
       $(UTILS_DIR)/minienv_utils2.c \
       $(UTILS_DIR)/minienv_utils.c \
       $(UTILS_DIR)/print_error.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default rule: compile all files and generate the main program
all: $(NAME)

# General rule: compile .c files to .o files
%.o: %.c
	$(CC) $(FLAGS) -g -Iinc -I$(LIBFT_DIR) -c $< -o $@

# Rule: compile the libft library
$(LIBFT):
	make -C $(LIBFT_DIR)

# Rule: generate the main program
$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(FLAGS) -g $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

# Clean all generated .o files
clean:
	rm -rf $(OBJS)
	make -C $(LIBFT_DIR) clean

# Clean all generated files
fclean: clean
	rm -rf $(NAME)
	make -C $(LIBFT_DIR) fclean

# Recompile all content
re: clean fclean all
