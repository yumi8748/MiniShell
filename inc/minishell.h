/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:39:19 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/07 17:32:07 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h" // libft
# include <stdio.h> // perror
# include <readline/readline.h> // readline
# include <readline/history.h> // history
# include <stdlib.h> // getenv
# include <unistd.h> // getpwd
# include <sys/wait.h> // waitpid
# include <sys/stat.h> // stat
# include <signal.h> // sigaction
# include <fcntl.h> // open flags
# include <limits.h>  //llong max    
# include "defines.h"
# include "structs.h"


# define BUF_SIZE 2048
# define BUILTIN_MISUSE 2
# define BOLD    "\001\e[1m\002"
# define RESET_SIZE   "\001\e[0m\002"
# define GREEN	"\001\e[0;32m\002"
# define YELLOW	"\001\e[0;93m\002"
# define BLUE	"\001\e[0;34m\002"
# define PURPLE	"\001\e[0;35m\002"
# define CYAN	"\001\e[0;96m\002"
# define WHITE	"\001\e[0;37m\002"
# define RESET	"\001\e[0m\002"
# define TRUE 1
# define FALSE 0


//signals.c        yu
void	define_main_signals(void);
//leo加上如下
void	define_heredoc_signals(int child_pid);
void	define_execute_signals(int child_pid);


//minienv_utils.c      yu
char	*value_only(char *key_pair);
char	*minienv_value(char *name, t_env *minienv);
t_env	*minienv_node(char *name, t_env *minienv);

//init.c          yu 
t_env	*init_minienv(char **env);
void	list_append(char *key_pair, t_env **list);

int	minishell(t_env *minienv);


//check_input_utils.c       yu
int	has_pipe(char *str);
char	*get_next_pipe(char *str);
char	get_next_redir(char *str);
char	*get_redir_pos(char *str, char redir_char);
int	syntax_error(char *token);

//check_input_utils2.c       yu
int	is_invalid_token(char c);
int	unexpected_token(char *input);

//cmd_utils.c        yu
void	restore_pipes(char **split_cmds);
void	replace_pipes(char *str, char delimeter);
char	**split_cmds(char *cmds);

//input_errors.c
int	print_unclosedquote_err(void);
int	has_unclosed_quotes(char *str);
int	is_empty(char *str);
int	has_input_error(char *input, int *exit_status, t_env *minienv);

//input_errors2.c       yu
int	has_empty_pipe(char *str);
int	redirect_without_label(char *str);
int	start_with_pipe(char *str);
int	is_invalid_syntax(char *str);


// chen_prompt.c       leo
char	*prompt_msg(t_env *minienv);
char	*do_prompt(t_env *minienv);
void	shell_exit(char **args, t_env **minienv);
void	free_minienv(t_env **minienv);
void	check_args_error(char **args);
void	print_error_msg(char *command, char *msg);
void	exit_with_error(char *command, char *msg, int error);
void	print_perror_msg(char *command, char *perror_msg);
void	close_extra_fds(void);
void	close_all_fds(void);
int	str_to_ll(char *s, long long *nb);
void	clean_up_exit(char **args, t_env **minienv, int exit_status, int flag);


//chen_heredoc.c        leo
int handle_heredoc(char *input, int *exit_status, t_env *minienv);
int skip_quotes(char *s, int i, char quote_type);
char *find_here_symbol(char *str);
//int find_end(char *s, int start);
int find_end(char *s, int start, char quote);
char *name_after_redirect(char *s);
int	is_label_delimeter(char c);
int exec_heredoc(char *delimiter, int heredoc_ref, int *exit_status, t_env *minienv);
void read_heredoc(int *exit_status, t_env *minienv, char *delimiter, int heredoc_number);
char *tmp_here_file(int heredoc_ref);
int	wait_for_child(int pid, int is_last_child);

//chen_expansion.c
void	expand_exit_status(char **input, int exit_status);

//free.c         leo
void    free_str_array(char **s);


#endif