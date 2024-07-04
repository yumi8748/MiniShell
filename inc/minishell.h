/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:39:19 by yu-chen           #+#    #+#             */
/*   Updated: 2024/07/03 20:07:00 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h" // libft
# include "structs.h"
# include <fcntl.h>             // open flags
# include <limits.h>            //llong max
# include <readline/history.h>  // history
# include <readline/readline.h> // readline
# include <signal.h>            // sigaction
# include <stdio.h>             // perror
# include <stdlib.h>            // getenv
# include <sys/stat.h>          // stat
# include <sys/wait.h>          // waitpid
# include <unistd.h>            // getpwd

# define BUF_SIZE 2048
# define MISUSE 2
# define BOLD "\001\e[1m\002"
# define RESET_SIZE "\001\e[0m\002"
# define GREEN "\001\e[0;32m\002"
# define YELLOW "\001\e[0;93m\002"
# define BLUE "\001\e[0;34m\002"
# define PURPLE "\001\e[0;35m\002"
# define CYAN "\001\e[0;96m\002"
# define WHITE "\001\e[0;37m\002"
# define RESET "\001\e[0m\002"
# define TRUE 1
# define FALSE 0

// signals.c
void	define_main_signals(void);
void	define_heredoc_signals(int child_pid);
void	define_execute_signals(int child_pid);
int		handle_signal_interrupt(int status, int is_last_child);

// minienv_utils.c      yu
char	*get_value(char *key_pair);
char	*minienv_value(char *name, t_env *minienv);
t_env	*find_node(char *name, t_env *minienv);
void	minienv_update(char *key, char *value, t_env *minienv);
char	*get_key(char *keypair);

// init.c          yu
t_env	*init_minienv(char **env);
void	list_append(char *key_pair, t_env **list);
char	*create_keypair(char *key, char *value);

int		minishell(t_env *minienv);

// check_input_utils.c       yu
// int	has_pipe(char *str);
char	*find_pipe(char *str);
char	*find_redir_pos(char *str, char redir_char);
int		print_syntax_error(char *token);

// check_input_utils2.c       yu
int		is_invalid_token(char c);
int		unexpected_token(char *input);

// cmd_utils.c        yu
void	reset_inquote_pipe(char **splited_cmds);
void	replace_inquote_pipe(char *s);
char	**split_all_cmds(char *cmd_line);

// input_errors.c
int		print_unclosedquote_err(void);
int		is_unclosed_quotes(char *str);
int		is_empty(char *str);
int		is_input_error(char *input, int *exit_status, t_env *minienv);

// input_errors2.c       yu
// int	empty_pipe(char *str);
int		has_empty_pipe(char *input);
// int	redirect_without_label(char *str);
int		redirect_without_label(char *input);
int		start_with_pipe(char *str);
int		is_invalid_syntax(char *str);

// input_error3.c
char	*get_next_pipe(char *str);
char	*get_redirect_position(char *str, char redirect_char);
char	get_next_redirect(char *str);

// chen_prompt.c       leo
char	*prompt_msg(t_env *minienv);
char	*do_prompt(t_env *minienv);
int		shell_exit(char **args, t_env **minienv);
void	free_minienv(t_env **minienv);
void	check_args_error(char **args);
void	print_error_msg(char *command, char *msg);
void	exit_with_error(char *command, char *msg, int error);
void	print_perror_msg(char *command, char *perror_msg);
void	close_extra_fds(void);
void	close_all_fds(void);
int		str_to_ll(char *s, long long *nb);
void	clean_up_exit(char **args, t_env **minienv, int exit_status, int flag);
void	clean_up_resources(t_env **minienv, char **args, int flag);

// chen_heredoc.c        leo
int		handle_heredoc(char *input, int *exit_status, t_env *minienv);
int		skip_quotes(char *s, int i, char quote_type);
char	*find_here_pos(char *str);
int		find_end(char *s, int start);
char	*name_after_redirect(char *s);
int		is_label_delimiter(char c);
int		exec_heredoc(char *delimiter, int heredoc_ref, int *exit_status,
			t_env *minienv);
void	read_heredoc(int *exit_status, t_env *minienv, char *delimiter,
			int heredoc_number);
char	*tmp_here_file(int heredoc_ref);
int		wait_for_child(int pid, int is_last_child);
void	_restore_original_fds(int original_fds[2]);

// handle_expansions.c     yu
int		is_valid_varname(char *s);
int		is_valid_varchar(char c);
int		varname_size(char *s);
char	*find_var_pos(char *input);
void	var_at_start(char **input, char *var_value, char *after_var);
void	var_not_at_start(char **input, char *var_value, char *after_var,
			char *pos);
void	expand_variables(char **input, t_env *minienv);
void	handle_expansions(char **input, t_env *minienv, int exit_status);

// chen_expansion.c
void	expand_exit_status(char **input, int exit_status);
int		find_exit_marker(char *s);

// free.c         leo
void	free_str_array(char **s);

// chen_one_cmd.c    leoc
char	get_redir_symbol(char *s);
int		handle_infile_redir(char *cmd, int original_fd[2]);
int		handle_outfile_redir(char *cmd, int original_fd[2]);
int		handle_redirects(char *cmd, int original_fds[2]);
void	redirect_heredoc(char *cmd, int heredoc_ref);
int		execute_one_cmd(char *cmd, t_env **minienv);
int		is_builtin(char *cmd);
int		execute_builtin(char **args, t_env **minienv);
char	**split_one_arg(char *cmd, t_env **minienv);
int		has_quote(char *cmd);
void	replace_inquote_spaces(char *cmd);
void	remove_quotes(char *cmd);
void	reset_spaces(char **args);
int		has_pipe(char *s);
void	redirect_fd(int fd_to_redirect, int fd_location);

// chen_one_cmd2.c      leo
int		execute_normal_cmd(char **args, t_env *minienv);
void	exec_error(char **args, char *path, char **envp);
int		minienv_size(t_env *minienv);
char	**from_minienv_to_env(t_env *minienv);
int		is_folder(char *cmd);
void	command_exit(char **args, t_env *minienv, int exit_status);
char	*get_path(char *cmd, t_env *minienv);
int		is_path(char *cmd);
char	*get_real_path(char *cmd, void *minienv);
char	*get_path_from_env(char *cmd, t_env *minienv);
char	*find_executable_path(char *cmd, char **splited_paths);

// chen_builtin.c      leo
int		is_minus_n(char *s);
int		echo(char **args);
char	*cd_path(char **args, t_env *minienv);
int		cd(char **args, t_env *minienv);
int		pwd(void);

// builtins
int		unset(char **argv, t_env **minienv);
int		env(t_env *minienv);
int		export(char **args, t_env **minienv);
int		unset(char **argv, t_env **minienv);

// multi_cmds.c   yu leo
int		execute_multi_cmds(char **splited_cmds, t_env **minienv);
void	handle_pipe(char **splited_cmds, int i, int original_fd[2]);
int		str_strlen(char **splited_cmds);
int		*init_pid_array(char **splited_cmds);
int		wait_for_children(int *pid_array, char **splited_cmds);
void	clean_exit(char **splited_cmds, t_env **minienv);
void	_handle_redirects(char *cmd, char **splited_cmds, t_env **minienv);
int		_handle_infile_redir(char *command);
int		_handle_outfile_redir(char *command);
void	_execute_cmd(char *cmd, char **cmds, t_env **minienv);
int		_execute_builtin(char **args, t_env **minienv);
int		_execute_normal_cmd(char **args, t_env *minienv);
void	save_original_fds(int original_fds[2]);
void	restore_original_fds(int original_fds[2]);

char	*extract_delimiter(char *s, int start);
void	free_str(char *s);

int		if_append(char c);

int		restore_fds(int original_fds[2]);

void	initialize_original_fds(int original_fds[2]);

int		process_redirects(char *cmd, int original_fds[2]);

void	redirect_fd(int fd_to_redirect, int fd_location);
void	duplicate_fd(int fd, int new_fd);

void	stock_original_fds(int fd, int new_fd);

void	print_fd_error(char *file, int fd, int new_fd);
int		process_single_redirect(char *cmd, char redir_symbol,
			int original_fds[2]);
int		str_equal(const char *str1, const char *str2);
int		builtin_export(char **args, t_env **minienv);

int		contains_alpha(char *args);
void	replace_tabs_with_spaces(char *str);
void	replace_space_and_quote(char *command);
char	*find_and_replace_tilde(char *command, t_env *minienv);
void	replace_quote_by_null(char **exec_args);
size_t	calculate_command_len(char *command, t_env *minienv);
void	expand_tilde_path(char *new_cmd_ptr, char *cmd_ptr, char *expanded_path,
			t_env *minienv);
#endif