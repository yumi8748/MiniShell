/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:53:17 by leochen           #+#    #+#             */
/*   Updated: 2024/07/02 14:50:06 by leochen          ###   ########.fr       */
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

	if (!args || !args[1])
	{
		if (args)
			free_str_array(args);
		close_all_fds();
		exit(EXIT_SUCCESS);
	}
	if (!str_to_ll(args[1], &exit_status))
	{
		free_str_array(args);
		exit_with_error("exit", "numeric argument required", 2);
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
