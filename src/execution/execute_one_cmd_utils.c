/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_cmd_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 16:12:15 by leochen           #+#    #+#             */
/*   Updated: 2024/07/03 21:11:15 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	execute_in_child(char **args, t_env *minienv)
{
	char	*cmd;
	char	*path;
	char	**envp;

	cmd = args[0];
	if (!cmd)
		command_exit(args, minienv, 0);
	if (is_empty(args[0]))
		command_exit(&args[0], minienv, 127);
	if (is_folder(cmd))
		command_exit(args, minienv, 126);
	path = get_path(cmd, minienv);
	if (path == NULL && minienv_value("PATH", minienv))
		command_exit(args, minienv, 127);
	else if (path == NULL)
		path = ft_strdup(cmd);
	rl_clear_history();
	close_extra_fds();
	envp = from_minienv_to_env(minienv);
	free_minienv(&minienv);
	if (execve(path, args, envp) == -1)
		exec_error(args, path, envp);
	exit(EXIT_SUCCESS);
}

int	execute_normal_cmd(char **args, t_env *minienv)
{
	int	pid;

	pid = fork();
	define_execute_signals(pid);
	if (pid == 0)
		execute_in_child(args, minienv);
	else if (pid > 0)
		return (wait_for_child(pid, 1));
	else
		print_perror_msg("fork", "");
	exit(1);
}

int	wait_for_child(int pid, int is_last_child)
{
	int	status;

	if (pid == -1)
		return (EXIT_FAILURE);
	if (waitpid(pid, &status, 0) == -1)
		print_perror_msg("waitpid", ft_itoa(pid));
	if (WIFSIGNALED(status))
		return (handle_signal_interrupt(status, is_last_child));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

void	_restore_original_fds(int original_fds[2])
{
	if (original_fds[0] != -1)
		redirect_fd(original_fds[0], STDIN_FILENO);
	if (original_fds[1] != -1)
		redirect_fd(original_fds[1], STDOUT_FILENO);
}
