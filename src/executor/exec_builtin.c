/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:36:41 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/17 16:18:33 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	exec_forked_builtin(char **args, t_env **minienv)
{
	int	exit_status;

	exit_status = exec_builtin(args, minienv);
	free_array(args);
	free_minienv(minienv);
	rl_clear_history();
	exit(exit_status);
}

int	exec_builtin(char **args, t_env **minienv)
{
	char	*cmd;

	cmd = args[0];
	if (ft_strcmp(cmd, "echo") == 0)
		return (builtin_echo(args));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(cmd, "env") == 0)
		return (builtin_env(*minienv));
	if (ft_strcmp(cmd, "export") == 0)
		return (builtin_export(args, minienv));
	if (ft_strcmp(cmd, "unset") == 0)
		return (builtin_unset(args, minienv));
	if (ft_strcmp(cmd, "cd") == 0)
		return (builtin_cd(args, *minienv));
	if (ft_strcmp(cmd, "exit") == 0)
		return (builtin_exit(args, minienv));
	else
		return (EXIT_FAILURE);
}
