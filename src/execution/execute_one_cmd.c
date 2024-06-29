/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:02:31 by leochen           #+#    #+#             */
/*   Updated: 2024/06/29 14:45:56 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_one_cmd(char *cmd, t_env **minienv)
{
	char	**args;
	int		exit_status;
	int		original_fds[2];

	if (handle_redirects(cmd, &original_fds[0]) == 0)
	{
		_restore_original_fds(original_fds);
		free(cmd);
		return (EXIT_FAILURE);
	}
	args = split_one_arg(cmd);
	free(cmd);
	if (is_builtin(args[0]))
		exit_status = execute_builtin(args, minienv);
	else
		exit_status = execute_normal_cmd(args, *minienv);
	free_str_array(args);
	_restore_original_fds(original_fds);
	return (exit_status);
}

/*get path里面有个126的情况 他和在一起了 要分开成为其中一个是127*/

int	is_builtin(char *cmd)
{
	if (!cmd || !*cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0')
		return (1);
	if (ft_strncmp(cmd, "cd", 2) == 0 && cmd[2] == '\0')
		return (1);
	if (ft_strncmp(cmd, "pwd", 3) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "export", 6) == 0 && cmd[6] == '\0')
		return (1);
	if (ft_strncmp(cmd, "unset", 5) == 0 && cmd[5] == '\0')
		return (1);
	if (ft_strncmp(cmd, "env", 3) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "exit", 4) == 0 && cmd[4] == '\0')
		return (1);
	return (0);
}

int	execute_builtin(char **args, t_env **minienv)
{
	char	*cmd;
	int		exit_status;

	cmd = args[0];
	if (ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0')
		exit_status = echo(args);
	else if (ft_strncmp(cmd, "pwd", 3) == 0 && cmd[3] == '\0')
		exit_status = ft_pwd();
	else if (ft_strncmp(cmd, "env", 3) == 0 && cmd[3] == '\0')
		exit_status = env(*minienv);
	else if (ft_strncmp(cmd, "export", 6) == 0 && cmd[6] == '\0')
		exit_status = builtin_export(args, minienv);
	else if (ft_strncmp(cmd, "unset", 5) == 0 && cmd[5] == '\0')
		exit_status = unset(args, minienv);
	else if (ft_strncmp(cmd, "cd", 2) == 0 && cmd[2] == '\0')
		exit_status = cd(args, *minienv);
	else if (ft_strncmp(cmd, "exit", 4) == 0 && cmd[4] == '\0')
		exit_status = shell_exit(args, minienv);
	else
		exit_status = EXIT_FAILURE;
	return (exit_status);
}
