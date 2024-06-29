/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_cmd_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 16:12:15 by leochen           #+#    #+#             */
/*   Updated: 2024/06/29 15:16:30 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_normal_cmd(char **args, t_env *minienv)
{
	char	*cmd;
	int		pid;
	char	*path;
	char	**envp;

	cmd = args[0];
	pid = fork();                //和_executet_normal_cmd相比 多了fork
	define_execute_signals(pid); //也多了信号处理 和pid的检查
	if (pid == 0)
	{
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
	}
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
			// waitpid 函数会将子进程的退出状态存储到 status 变量中，因此在调用 waitpid 函数之前，不需要对 status 变量进行初始化。
	if (WIFSIGNALED(status))                      
		//用于检查一个子进程的退出状态是否是由于接收到一个信号而导致的。 如果子进程是因为信号中断引起的，那么 WIFSIGNALED(status) 的返回值为非零， 如果子进程是由信号中断引起的，则函数调用 handle_signal_interrupt 函数来处理中断，并返回中断的状态码。
		return (handle_signal_interrupt(status, is_last_child));
	if (WIFEXITED(status)) //如果子进程是正常退出的，则函数返回子进程的退出状态码。
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE); // 1
}

void	_restore_original_fds(int original_fds[2])
{
	if (original_fds[0] != -1)
		redirect_fd(original_fds[0], STDIN_FILENO);
	if (original_fds[1] != -1)
		redirect_fd(original_fds[1], STDOUT_FILENO);
}

/*在 heredoc 中，通常会有一个父进程和一个子进程。父进程负责创建一个临时文件，并将其打开以进行写入。子进程则负责读取用户输入的内容，并将其写入到临时文件中。

以下是一个 heredoc 示例，并且简要说明了其中的父进程和子进程：

$ cat <<EOF > myfile.txt
> This is line 1
> This is line 2
> EOF

在这个示例中，cat 命令是父进程，它会创建一个名为 myfile.txt 的临时文件，并将其打开以进行写入。接着，cat 命令会等待用户输入，直到用户输入了 EOF 为止。

在用户输入的过程中，cat 命令会创建一个子进程，用于读取用户输入的内容，并将其写入到临时文件中。一旦用户输入了 EOF，子进程会结束，并且父进程会关闭临时文件，并将其重命名为 myfile.txt。

因此，在这个示例中，父进程是 cat 命令，子进程是由 cat 命令创建的，用于读取用户输入的内容。*/