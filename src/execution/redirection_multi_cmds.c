/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:13:08 by leochen           #+#    #+#             */
/*   Updated: 2024/06/27 18:14:12 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


void handle_pipe(char **splited_cmds, int i, int original_fd[2])
{
    static int pipefd[2];

    if (i == 0) //如果`i`等于0，这意味着这是管道操作的第一个命令。函数创建一个新的管道，并将标准输出（STDOUT）重定向到管道的写端。
    {
        if (pipe(pipefd) == -1)
            print_error_msg("pipe", splited_cmds[i]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
    }
    else if (i < str_strlen(splited_cmds) - 1) //如果`i`小于`splited_cmds`的长度减1，这意味着这是管道操作的中间命令。先将上一个命令的管道读端复制到标准输入（STDIN），然后创建一个新的管道，并将标准输出（STDOUT）重定向到管道的写端。
    {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        if (pipe(pipefd) == -1)
            print_error_msg("pipe", splited_cmds[i]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
    }
    else //如果`i`等于`splited_cmds`的长度减1，这意味着这是管道操作的最后一个命令。将上一个命令的管道读端复制到标准输入（STDIN），然后将标准输出（STDOUT）重定向到原始的标准输出（STDOUT）。
    {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        dup2(original_fd[1], STDOUT_FILENO);
        close(original_fd[1]);
    }
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
			if (_handle_infile_redir(cmd) == 0) //返回0表示打开文件失败
				clean_exit(splited_cmds, minienv);
		}
		else if (redir_symbol == '>')
		{
			if (_handle_outfile_redir(cmd) == 0)   //返回0表示打开文件失败
			    clean_exit(splited_cmds, minienv);
		}
		else if (redir_symbol == -1)
			redirect_heredoc(cmd, redir_symbol);
		redir_symbol= get_redir_symbol(cmd);
	}				
}


int	_handle_infile_redir(char *command) //处理< 输入重定向 打开infile文件 没有修改command的值  成功返回1 失败返回0
{
	char	*infile_redir;
	char	*file;
	int		fd;

	infile_redir = find_redir_pos(command, '<'); //找到<符号的位置ptr
	if (!infile_redir)  //返回NULL说明没有<符号  但其实既然调用了这个函数说明一定有<符号
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
	int      append;
	
	outfile_redir = find_redir_pos(command, '>');
	if (!outfile_redir)
		return (1);
	if (outfile_redir[1] == '>')
		append = 1;
	else
		append = 0;
	file = name_after_redirect(outfile_redir); //找到文件名的位置ptr
	if (append == 1)  //如果符号是>> 则表示追加重定向
	{
		//printf("redir:>>append\n");
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		//printf("redir> trunc\n");
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
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

