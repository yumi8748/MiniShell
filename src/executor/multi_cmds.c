/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 15:52:51 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/22 17:12:58 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_multi_cmds(char **splited_cmds, t_env **minienv)
{
	int		i;
	int original_fd[2];
	int        *pid_array;

	i = 0;
	original_fd[0] = dup(STDIN_FILENO);
	original_fd[1] = dup(STDOUT_FILENO);
	pid_array = init_pid_array(splited_cmds);	
	while (splited_cmds[i] != NULL)
	{
		handle_pipe(splited_cmds, i, original_fd);
		pid_array[i] = fork();
		define_execute_signals(pid_array[i]);
		if (pid_array[i] == -1)
			print_error_msg("fork", splited_cmds[i]);
		else if (pid_array[i] == 0)
		{
			free(pid_array);   //在子进程中释放pid_array 不影响父进程的pid_array
			_handle_redirects(splited_cmds[i], splited_cmds, minienv); //处理重定向种类 打开文件 
			printf("here1\n");
			_execute_cmd(splited_cmds[i], splited_cmds, minienv);
		}
		i++;
	}
	dup2(original_fd[0], STDIN_FILENO);
	close(original_fd[0]);
	dup2(original_fd[1], STDOUT_FILENO);
	close(original_fd[1]);
	return (wait_for_children(pid_array, splited_cmds));
}

void    handle_pipe(char **splited_cmds, int i, int original_fd[2])
{
	static int		pipefd[2];

		if (i == 0)
		{
			if (pipe(pipefd) == -1)
				print_error_msg("pipe", splited_cmds[i]);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
		}
		else if (i < str_strlen(splited_cmds) - 2 && i > 0)
		{
			if (pipe(pipefd) == -1)
				print_error_msg("pipe", splited_cmds[i]);
			dup2(pipefd[0], STDIN_FILENO);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
		}
		else
		{
			dup2(original_fd[1], STDIN_FILENO);
			close(original_fd[0]);
			close(original_fd[1]);
		}
}

int str_strlen(char **splited_cmds)
{
	int i;

	i = 0;
	while (splited_cmds[i] != NULL)
		i++;
	return (i);
}
	

int  *init_pid_array(char **splited_cmds)
{
	int len;
	int *pid_arry;
	
	len = str_strlen(splited_cmds);
	pid_arry = (int *)malloc(sizeof(int) * len);
	if (!pid_arry)
		return (NULL);
	ft_memset(pid_arry, 0, sizeof(int) * len);
	printf("len = %d\n", len);
	return (pid_arry);
}

int wait_for_children(int *pid_array, char **splited_cmds)
{
	int i;
	int exit_status;
	int is_last_pid;

	i = 0;
	exit_status = 0;
	while (splited_cmds[i] != NULL)
	{
		if (i == str_strlen(splited_cmds) - 1)
			is_last_pid = 1;
		else
		    is_last_pid = 0;
		exit_status = wait_for_child(pid_array[i], is_last_pid);
		i++;
	}
	close_extra_fds();
	free(pid_array);
	return (exit_status);
}

void clean_exit(char **splited_cmds, t_env **minienv)
{
	rl_clear_history();
	free_str_array(splited_cmds);
	free_minienv(minienv);
	close_all_fds();
	exit(EXIT_FAILURE);
}


/* 
** 函数作用：处理重定向的函数 重定向的符号有三种  < > << 为什么没有>> 因为>>是追加重定向 会在_handle_outfile_redir函数中处理 
** 1. 通过get_redir_symbol函数获取重定向符号 2. 通过_handle_infile_redir函数处理输入重定向 3. 通过_handle_outfile_redir函数处理输出重定向
** 4. 通过redirect_heredoc函数处理<<重定向 5. 通过get_redir_symbol函数获取重定向符号 6. 重复2-5步骤直到没有重定向符号
*/
void _handle_redirects(char *cmd, char **splited_cmds, t_env **minienv)  //cmd其实是splited_cmds[i] 也就是按照|分割的每个还未处理成可执行的命令
{
    char redir_symbol;

    redir_symbol= get_redir_symbol(cmd); //获取重定向符号 < > << 如果返回0表示没有重定向符号
    while (redir_symbol != 0)
    {
        if (redir_symbol== '<')
        {
			if (_handle_infile_redir(cmd) == 0)
				clean_exit(splited_cmds, minienv);
		}
		else if (redir_symbol == '>')
		{
			if (_handle_outfile_redir(cmd) == 0)
			    clean_exit(splited_cmds, minienv);
		}
		else if (redir_symbol < 0)
			redirect_heredoc(cmd, redir_symbol);
		redir_symbol= get_redir_symbol(cmd);
	}
				
}


int	_handle_infile_redir(char *command) //处理< 输入重定向 打开infile文件 没有修改command的值
{
	char	*infile_redir;
	char	*file;
	int		fd;

	infile_redir = find_redir_pos(command, '<'); //找到<符号的位置ptr
	if (!infile_redir)
		return (1);
	file= name_after_redirect(infile_redir); //找到文件名的位置ptr
	fd = open(file, O_RDONLY, FD_CLOEXEC); //打开文件
	if (fd == -1)
	{
		print_perror_msg("open", file);
		free(file);
		return (0);
	}
	else
	 {
        dup2(fd, STDIN_FILENO); //将文件描述符复制到stdin
        close(fd);
    }
	free(file);
	return (1);
}

int	_handle_outfile_redir(char *command) //处理> 输出重定向 打开outfile文件 没有修改command的值
{
	char	*outfile_redir;
	char	*file;
	int		fd;
	
	outfile_redir = find_redir_pos(command, '>');
	if (!outfile_redir)
		return (1);
	file = name_after_redirect(outfile_redir); //找到文件名的位置ptr
	if (outfile_redir[1] == '>')  //如果符号是>> 则表示追加重定向
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_perror_msg("open", file);
		free(file);
		return (0);
	}
	else
	{
        dup2(fd, STDOUT_FILENO);  //将文件描述符复制到stdout
        close(fd);
    }
	free(file);
	return (1);
}

void	_execute_cmd(char *cmd, char **cmds, t_env **minienv) //cmd是splited_cmds[i] 也就是按照|分割的每个还未处理成可执行的命令 cmds是splited_cmds
{
	char	**args;

	printf("here\n");
	close_extra_fds();
	printf("%s\n", cmd);
	args = split_one_arg(cmd);
	printf("args[0]:%s\n", args[0]);
	printf("args[1]:%s\n", args[1]);
	if (is_builtin(args[0]))
		_execute_builtin(args, minienv);
	else
		_execute_normal_cmd(args, *minienv);
	free_str_array(cmds);
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

int	_execute_normal_cmd(char **args, t_env *minienv)  //没有fork(pid) signal
{
	char *cmd;
	char	*path;
	char	**envp;

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