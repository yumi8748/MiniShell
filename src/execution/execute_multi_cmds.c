/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multi_cmds.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 15:52:51 by yu-chen           #+#    #+#             */
/*   Updated: 2024/07/01 13:16:04 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_multi_cmds(char **splited_cmds, t_env **minienv)
{
	int	i;
	int	original_fds[2];
	int	*pid_array;

	i = 0;
	save_original_fds(original_fds);
	pid_array = init_pid_array(splited_cmds);
	while (splited_cmds[i] != NULL)
	{
		handle_pipe(splited_cmds, i, original_fds);
		pid_array[i] = fork();
		define_execute_signals(pid_array[i]);
		if (pid_array[i] == -1)
			print_error_msg("fork", splited_cmds[i]);
		else if (pid_array[i] == 0)
		{
			free(pid_array); //在子进程中释放pid_array 不影响父进程的pid_array
			_handle_redirects(splited_cmds[i], splited_cmds, minienv);
			_execute_cmd(splited_cmds[i], splited_cmds, minienv);
		}
		i++;
	}
	restore_original_fds(original_fds);
	return (wait_for_children(pid_array, splited_cmds));
}

void	_execute_cmd(char *cmd, char **cmds, t_env **minienv)
// cmd是splited_cmds[i] 也就是按照|分割的每个还未处理成可执行的命令 cmds是splited_cmds
{
	char **args;

	// printf("here\n");
	close_extra_fds();
	// printf("%s\n", cmd);
	args = split_one_arg(cmd);
	// printf("splited_cmd[i][0]:%s\n", args[0]);
	// printf("splited_cmd[1]:%s\n", args[1]);
	free_str_array(cmds);
	if (is_builtin(args[0]))
		_execute_builtin(args, minienv);
	else
		_execute_normal_cmd(args, *minienv);
}

int	_execute_builtin(char **args, t_env **minienv)
{
	int	exit_status;

	exit_status = execute_builtin(args, minienv);
	free_str_array(args);
	free_minienv(minienv);
	rl_clear_history();
	exit(exit_status);
}

int	_execute_normal_cmd(char **args, t_env *minienv) //没有fork(pid) signal
{
	char *cmd;
	char *path;
	char **envp;

	cmd = args[0];
	if (is_empty(cmd))
		command_exit(args, minienv, 0);
	if (is_folder(cmd))
		command_exit(args, minienv, 126);
	path = get_path(cmd, minienv);
	if (!path && minienv_value("PATH", minienv) != NULL)
		command_exit(args, minienv, 127);
	else if (!path)
		path = ft_strdup(cmd);
	envp = from_minienv_to_env(minienv);
	rl_clear_history();
	close_extra_fds();
	free_minienv(&minienv);
	if (execve(path, args, envp) == -1)
		exec_error(args, path, envp);
	exit(EXIT_SUCCESS);
}
