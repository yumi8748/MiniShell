/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:47:20 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/14 20:46:50 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	external_exit(char **args, t_env *minienv, int exit_status)
{
	if (exit_status == NOT_EXECUTABLE)
		print_error_msg(args[0], NOT_EXECUTABLE_MSG);
	if (exit_status == CMD_NOT_FOUND)
		print_error_msg(args[0], CMD_NOT_FOUND_MSG);
	rl_clear_history();
	free_minienv(&minienv);
	free_array(args);
	close_all_fds();
	exit(exit_status);
}

static void	handle_execve_errors(char **args, char *path, char **envp)
{
	int	error;

	error = EXIT_FAILURE;
	print_perror_msg("execve", args[0]);
	if (access(path, F_OK) != 0)
		error = CMD_NOT_FOUND;
	else if (access(path, X_OK) != 0)
		error = NOT_EXECUTABLE;
	free_array(args);
	free_array(envp);
	free(path);
	exit(error);
}

int	exec_external(char **args, t_env *minienv)
{
	char	*path;
	char	*cmd;
	char	**envp;

	cmd = args[0];
	if (is_empty(cmd))
		external_exit(args, minienv, EXIT_SUCCESS);
	if (is_folder(cmd))
		external_exit(args, minienv, NOT_EXECUTABLE);
	path = get_path(cmd, minienv);
	if (path == NULL && minienv_has_path(minienv))
		external_exit(args, minienv, CMD_NOT_FOUND);
	else if (path == NULL)
		path = ft_strdup(cmd);
	rl_clear_history();
	close_extra_fds();
	envp = minienv_to_envp(minienv);
	free_minienv(&minienv);
	if (execve(path, args, envp) == -1)
		handle_execve_errors(args, path, envp);
	exit(EXIT_SUCCESS);
}
