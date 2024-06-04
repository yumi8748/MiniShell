/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:39:19 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/04 18:00:21 by leochen          ###   ########.fr       */
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

void	define_main_signals(void);

char	*value_only(char *key_pair);

char	*minienv_value(char *name, t_env *minienv);

t_env	*minienv_node(char *name, t_env *minienv);

char	*prompt_msg(t_env *minienv);

t_env	*init_minienv(char **env);

void	list_append(char *key_pair, t_env **list);

int	minishell(t_env *minienv);


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


void    free_str_array(char **s);
#endif