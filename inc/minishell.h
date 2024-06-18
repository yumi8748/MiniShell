/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:39:19 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/17 14:50:53 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define TRUE 1
# define FALSE 0
# define SUCCESS 1
# define FAIL 0
# define IN 0
# define OUT 1
# define INTERRUPT 128
# define CMD_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define NOT_EXECUTABLE 126
# define OUT_OF_RANGE 255
# define BUILTIN_MISUSE 2
# define FORK_ERROR -1
# define CMD_NOT_FOUND_MSG "command not found"
# define NOT_EXECUTABLE_MSG "Is a directory"

# include "../libft/inc/libft.h"
# include "defines.h"
# include "structs.h"
# include <fcntl.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

t_env	*init_minienv(char **env);
t_env	*minienv_node(char *name, t_env *minienv);
int		*minishell(t_env *minienv);
void	define_main_signals(void);
int		has_input_error(char *input, int *exit_status, t_env *minienv);
int		is_invalid_syntax(char *str);
void	handle_expansions(char **input, t_env *minienv, int exit_status);
int		is_varname(char c);
int		is_valid_varname(char *name);
char	*minienv_value(char *name, t_env *minienv);
int		has_pipe(char *str);
char	**split_cmds(char *cmds);
int		exec_multi_cmds(char **cmds, t_env **minienv);
void	free_array(char **arr);

#endif