/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_one_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:18:57 by leochen           #+#    #+#             */
/*   Updated: 2024/06/27 18:21:30 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"



/*
`handle_infile_redir` 函数的主要目的是处理输入重定向。它接收一个命令字符串和一个包含原始文件描述符的数组作为参数。

以下是函数的主要步骤：

1. 检查原始的标准输入文件描述符是否已经被保存，如果没有，则保存它。
2. 在命令字符串中查找输入重定向符号('<')的位置。
3. 如果没有找到重定向符号，函数返回1，表示没有进行输入重定向。
4. 如果找到了重定向符号，函数获取重定向符号后面的文件名。
5. 尝试以只读模式打开该文件，并设置 `FD_CLOEXEC` 标志，这意味着如果执行了 `exec()` 系列函数，这个文件描述符将会被自动关闭。
6. 如果打开文件失败，函数打印错误信息，释放文件名内存，恢复原始的标准输入文件描述符，然后返回0。
7. 如果打开文件成功，函数将标准输入重定向到这个文件，然后关闭文件描述符。

这个函数看起来是完整的，它处理了输入重定向的所有主要情况，包括没有重定向、打开文件失败和成功打开文件的情况。然而，是否完善还取决于 `get_redir_pos` 和 `name_after_redirect` 这两个函数的实现，以及它们如何处理错误。*/
int	handle_infile_redir(char *cmd, int original_fd[2])
{
	char *infile_redir;
	char *file;
	int fd;

	if (original_fd[0] == -1)  // 保存原始标准输入文件描述符
		original_fd[0] = dup(STDIN_FILENO);
	infile_redir = find_redir_pos(cmd, '<');
	if (!infile_redir)  //没有重新定向符 
		return (1);
	file = name_after_redirect(infile_redir); // 获取重定向文件名
	fd = open(file, O_RDONLY, FD_CLOEXEC); //`FD_CLOEXEC` 如果执行了一个 `exec()` 系列的函数，那么这个文件描述符将会被自动关闭。
	if (fd  == -1)   // 打开文件失败，打印错误信息并恢复原始标准输入文件描述符
	{
        print_perror_msg("open", file);
        free(file);
        dup2(original_fd[0], STDIN_FILENO);
        close(original_fd[0]);
        return (0);
	}
	else    // 重定向标准输入到指定文件
    {
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    free(file);
    return (1);
}

int handle_outfile_redir(char *cmd, int original_fd[2])
{
    char *outfile_redir;
    char *file;
    int fd;
	int append;

    if (original_fd[1] == -1) // 保存原始标准输出文件描述符
        original_fd[1] = dup(STDOUT_FILENO);
    outfile_redir = find_redir_pos(cmd, '>');  //// 获取输出重定向的位置
    if (!outfile_redir)
    {
		return (1); // 没有输出重定向，直接返回成功
	}
	if (outfile_redir[1] == '>')
	{
		append = 1;
	}
	else
	{
		append = 0;
	}
	file = name_after_redirect(outfile_redir);
	if (append == 1)  // 确定打开文件的标志
   		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644); // 追加模式
	else
    	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);  // 截断模式
    if (fd == -1)    // 打开文件失败，打印错误信息
    {
        print_perror_msg("open", file);
        free(file);
        dup2(original_fd[1], STDOUT_FILENO);
        close(original_fd[1]);
        return (0);
    }
    else    // 重定向标准输出到指定文件
    {
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    free(file);
    return (1);
}

int handle_redirects(char *cmd, int original_fds[2])
{
    char redir_symbol;

    original_fds[0] = -1;
    original_fds[1] = -1;
    redir_symbol= get_redir_symbol(cmd); //找到重定向符号ptr
    //printf("command after heredoc but before redir:%s\n", cmd);
	while (redir_symbol != 0)
    {
        if (redir_symbol== '<')
        {
            if (!handle_infile_redir(cmd, original_fds))
            {
                if (original_fds[0] != -1)
                {
                    dup2(original_fds[0], STDIN_FILENO);
                    close(original_fds[0]);
                }
                if (original_fds[1] != -1)
                {
                    dup2(original_fds[1], STDOUT_FILENO);
                    close(original_fds[1]);
                }
                return (0);
            }
        }
        if (redir_symbol== '>')
        {
            if (!handle_outfile_redir(cmd, original_fds))
            {
                if (original_fds[0] != -1)
                {
                    dup2(original_fds[0], STDIN_FILENO);
                    close(original_fds[0]);
                }
                if (original_fds[1] != -1)
                {
                    dup2(original_fds[1], STDOUT_FILENO);
                    close(original_fds[1]);
                }
                return (0);
            }
        }
        if (redir_symbol< 0)
        {
            if (original_fds[0] == -1)
                original_fds[0] = dup(STDIN_FILENO);
            if (original_fds[1] == -1)
                original_fds[1] = dup(STDOUT_FILENO);
            redirect_heredoc(cmd, redir_symbol);
        }
        redir_symbol= get_redir_symbol(cmd);
    }
    return (1);
}

void	redirect_heredoc(char *cmd, int heredoc_ref)
{
	char *file;
	int	file_fd;
	char *here_pos;

	file = tmp_here_file(heredoc_ref);
	//printf("redirect heredoc file:%s\n", file);
	file_fd = open(file, O_RDONLY);
	//printf("file_fd:%d\n", file_fd);
	if (file_fd == -1)
	{
		print_perror_msg("open", file);
		free(file);
		return ;
	}
	free(file);
	dup2(file_fd, STDIN_FILENO);
	close(file_fd);
	here_pos = find_redir_pos(cmd, heredoc_ref);
	ft_memmove(here_pos, here_pos + 1, ft_strlen(here_pos + 1) + 1);
}
