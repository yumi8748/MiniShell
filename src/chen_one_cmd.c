/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chen_one_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:02:31 by leochen           #+#    #+#             */
/*   Updated: 2024/06/15 16:54:42 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	get_redir_symbol(char *s)
{
     int	i;

    i = 0;
    while (s[i])
     {
         if (s[i] == '\'' || s[i] == '\"')
            skip_quotes(s, i, s[i]);
        if (s[i] == '<' && s[i+1] == '<') // check for '<<'
            return (-1); // return -1 for '<<'
        else if (s[i] == '<' || s[i] == '>')
             return (s[i]);
        i++;
     }
     return (0);
 }

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

    if (original_fd[1] == -1) // 保存原始标准输出文件描述符
        original_fd[1] = dup(STDOUT_FILENO);
    outfile_redir = find_redir_pos(cmd, '>');  //// 获取输出重定向的位置
    if (!outfile_redir)
        return (1); // 没有输出重定向，直接返回成功
    file = name_after_redirect(outfile_redir);
	if (outfile_redir[1] == '>')  // 确定打开文件的标志
    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666); // 追加模式
	else
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);  // 截断模式
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
    redir_symbol= get_redir_symbol(cmd);
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
	char *here_symbol;

	here_symbol = find_here_symbol(cmd);
	file = tmp_here_file(heredoc_ref);
	file_fd = open(file, O_RDONLY);
	if (file_fd == -1)
	{
		print_perror_msg("open", file);
		free(file);
		return ;
	}
	free(file);
	dup2(file_fd, STDIN_FILENO);
	close(file_fd);
	ft_memmove(here_symbol, here_symbol + 1, ft_strlen(here_symbol + 1) + 1);
}


int execute_one_cmd(char *cmd, t_env **minienv)
{
    char **args;
    int exit_status;
    int original_fds[2];

    if (handle_redirects(cmd, &original_fds[0]) == 0)
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
        free(cmd);
        return (EXIT_FAILURE);
    }
    args = split_args(cmd);
    free(cmd);
   // if (is_builtin(args[0]))
      //  exit_status = execute_builtin(args, minienv);
   // else
        exit_status = execute_normal_cmd(args, *minienv);
    free_str_array(args);
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
    return (exit_status);
}

/*get path里面有个126的情况 他和在一起了 要分开成为其中一个是127*/

int is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0')
		return (1);
	if (ft_strncmp(cmd, "cd", 2) == 0 && cmd[2] == '\0')
		return (1);
	if (ft_strncmp(cmd, "pwd", 3) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "export", 6) == 0 && cmd[6] == '\0')
		return (1);
	if (ft_strncmp(cmd, "unset", 5) == 0 && cmd[5] == '\0')
		return (1);
	if (ft_strncmp(cmd, "env", 3) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "exit", 4) == 0 && cmd[4] == '\0')
		return (1);	
	return (0);
}

// int	execute_builtin(char **args, t_env **minienv)
// {
// 	char	*cmd;
// 	int		exit_status;

// 	cmd = args[0];
// 	if (ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0')
// 		exit_status = echo(args);
// 	else if (ft_strncmp(cmd, "pwd", 3) == 0 && cmd[3] == '\0')
// 		exit_status = pwd();
// 	else if (ft_strncmp(cmd, "env", 3) == 0 && cmd[3] == '\0')
// 		exit_status = env(*minienv);
// 	else if (ft_strncmp(cmd, "export", 6) == 0 && cmd[6] == '\0') 
// 		exit_status = builtin_export(args, minienv);
// 	else if (ft_strncmp(cmd, "unset", 5) == 0 && cmd[5] == '\0')
// 		exit_status = unset(args, minienv);
// 	else if (ft_strncmp(cmd, "cd", 2) == 0 && cmd[2] == '\0')
// 		exit_status = cd(args, *minienv);
// 	else if (ft_strncmp(cmd, "exit", 4) == 0 && cmd[4] == '\0')
// 		exit_status = builtin_exit(args, minienv);
// 	else
// 		exit_status = EXIT_FAILURE;
// 	return (exit_status);
// }


char	**split_args(char *cmd)   //这只是分解一个命令的函数 而不是分解包括pipe的多个命令的命令行
{
	char	**args;

	if (!has_quote(cmd))
		return (ft_split(cmd, ' '));
	replace_spaces(cmd, '"');   //处理成对出现的引号 把里面的空格替换成1
	replace_spaces(cmd, '\'');
	remove_quotes(cmd);  //去掉引号
	args = ft_split(cmd, ' ');
	restore_spaces(args); //把1替换成空格 
	return (args);
}

int has_quote(char *cmd)
{
	while (cmd && *cmd)
	{
		if (*cmd == '\'' || *cmd == '\"')
			return (1);
		cmd++;
	}
	return (0);
}

void replace_spaces(char *cmd, char quote)  //在这段代码中，`cmd[i] = 1;` 是将字符串 `cmd` 中的空格字符（' '）替换为 ASCII 值为 1 的字符。ASCII 值为 1 的字符是一个不可打印的控制字符，名为 "Start of Heading"。
{
	int	i;

    i = 0;
    while (cmd[i])
    {
        if (cmd[i] == quote)
        {
            i++;
            while (cmd[i] && cmd[i] != quote)
            {
                if (cmd[i] == ' ')
                    cmd[i] = 1;
                i++;
            }
			if (cmd[i])
				i++;
		}
        else
            i++;
    }
}

void	remove_quotes(char *cmd)
{
	char	quote;

	while (*cmd)
	{
		if (*cmd == '\'' || *cmd == '\"')
		{
			quote = *cmd;
			ft_memmove(cmd, cmd + 1, ft_strlen(cmd + 1) + 1);
			while (*cmd && *cmd != quote)
				cmd++;
			if (*cmd)
				ft_memmove(cmd, cmd + 1, ft_strlen(cmd + 1) + 1);
		}
		else
			cmd++;
	}
}

void	restore_spaces(char **args)
{
	int	i;
	int	j;

	i = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] == 1)
				args[i][j] = ' ';
			j++;
		}
		i++;
	}
}


int	has_pipe(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
			i = skip_quotes(s, i, s[i]);
		else 
		{
			if (s[i] == '|')
				return (1);
			i++;
		}
	}
	return (0);
}