/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:53:17 by leochen           #+#    #+#             */
/*   Updated: 2024/06/29 15:03:43 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_error_msg(char *command, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	print_perror_msg(char *command, char *perror_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror(perror_msg);
}

void	exit_with_error(char *command, char *msg, int error)
{
	print_error_msg(command, msg);
	clean_up_exit(NULL, NULL, error, 0);
}

void	check_args_error(char **args)
{
	long long	exit_status;

	if (!args || !args[1]) // args[1]不存在说明直接退出exit 没有退出码 所以就是exit success?
	{
		if (args)
			free_str_array(args);
		close_all_fds();
		exit(EXIT_SUCCESS);
	}
	if (!str_to_ll(args[1], &exit_status))
	{
		free_str_array(args);
		exit_with_error("exit", "numeric argument required", BUILTIN_MISUSE);
	}
	if (args[2] != NULL)
	{
		free_str_array(args);
		exit_with_error("exit", "too many arguments", EXIT_FAILURE);
	}
}

void	exec_error(char **args, char *path, char **envp)
{
	int	exit_code;

	print_perror_msg("execve", args[0]);
	if (access(path, F_OK) == -1)
		exit_code = 127;
	else if (access(path, X_OK) == -1)
		exit_code = 126;
	else
		exit_code = 1;
	free(path);
	free_str_array(envp);
	free_str_array(args);
	exit(exit_code);
}
