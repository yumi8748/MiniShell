/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chen_one_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:02:31 by leochen           #+#    #+#             */
/*   Updated: 2024/06/10 18:51:04 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	get_redir_arrow(char *s)
{
 	int	i;

	i = 0;
	while (s[i])
 	{
 		if (s[i] == '\'' || s[i] == '\"')
			skip_quotes(s, i, s[i]);
		if (s[i] == '<' || s[i] == '>' || s[i] < 0)
 			return (s[i]);
		i++;
 	}
 	return (0);
 }


int	handle_infile_redir(char *cmd, int original_fd[2])
{
	char *infile_redir;
	char *file;
	int fd;

	if (original_fd[0] == -1)  // 保存原始标准输入文件描述符
		original_fd[0] = dup(STDIN_FILENO);
	infile_redir = get_redir_pos(cmd, '<');
	if (!infile_redir)  //没有重新定向符 
		return (1);
	file = name_after_redirect(infile_redir); // 获取重定向文件名
	fd = open(file, O_RDONLY, FD_CLOEXEC);
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
    char *output_redirect;
    char *file_name;
    int fd;
    int open_flags;

    if (original_fd[1] == -1) // 保存原始标准输出文件描述符
        original_fd[1] = dup(STDOUT_FILENO);
    output_redirect = get_redirect_position(cmd, '>');  //// 获取输出重定向的位置
    if (!output_redirect)
        return (1); // 没有输出重定向，直接返回成功
    if (output_redirect[1] == '>')  // 确定打开文件的标志
        open_flags = O_WRONLY | O_CREAT | O_APPEND; // 追加模式
    else
        open_flags = O_WRONLY | O_CREAT | O_TRUNC;  // 截断模式
    file_name = get_label_name(output_redirect);   // 获取重定向文件名
    fd = open(file_name, open_flags, 0644); 
    if (fd == -1)    // 打开文件失败，打印错误信息
    {
        print_perror_msg("open", file_name);
        free(file_name);
        dup2(original_fd[1], STDOUT_FILENO);
        close(original_fd[1]);
        return (0);
    }
    else    // 重定向标准输出到指定文件
    {
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    free(file_name);
    return (1);
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
    if (is_builtin(args[0]))
        exit_status = execute_builtin(args, minienv);
    else
        exit_status = execute_forked_external(args, *minienv);
    free_array(args);
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

int handle_redirects(char *cmd, int original_fds[2])
{
    char arrow;

    original_fds[0] = -1;
    original_fds[1] = -1;
    arrow = get_next_redirect(cmd);
    while (arrow)
    {
        if (arrow == '<')
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
        if (arrow == '>')
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
        if (arrow < 0)
        {
            if (original_fds[0] == -1)
                original_fds[0] = dup(STDIN_FILENO);
            if (original_fds[1] == -1)
                original_fds[1] = dup(STDOUT_FILENO);
            redirect_heredoc(cmd, arrow);
        }
        arrow = get_next_redirect(cmd);
    }
    return (1);
}






/*get path里面有个126的情况 他和在一起了 要分开成为其中一个是127*/

// int is_builtin(char *cmd)
// {
// 	if (ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0')
// 		return (1);
// 	if (ft_strncmp(cmd, "cd", 2) == 0 && cmd[2] == '\0')
// 		return (1);
// 	if (ft_strncmp(cmd, "pwd", 3) == 0 && cmd[3] == '\0')
// 		return (1);
// 	if (ft_strncmp(cmd, "export", 6) == 0 && cmd[6] == '\0')
// 		return (1);
// 	if (ft_strncmp(cmd, "unset", 5) == 0 && cmd[5] == '\0')
// 		return (1);
// 	if (ft_strncmp(cmd, "env", 3) == 0 && cmd[3] == '\0')
// 		return (1);
// 	if (ft_strncmp(cmd, "exit", 4) == 0 && cmd[4] == '\0')
// 		return (1);	
// 	return (0);
// }

// char	**split_args(char *cmd)  //公共 
// {
// 	char	**exec_args;

// 	if (!contains_quote_mark(cmd))
// 		return (ft_split(cmd, ' '));
// 	replace_spaces(cmd, '"');
// 	replace_spaces(cmd, '\'');
// 	remove_quotes(cmd);
// 	exec_args = ft_split(cmd, ' ');
// 	restore_spaces(exec_args);
// 	return (exec_args);
// }

/

// int	execute_forked_builtin(char **args, t_env **minienv)
// {
// 	int	exit_status;

// 	exit_status = execute_builtin(args, minienv);
// 	free_array(args);
// 	free_minienv(minienv);
// 	rl_clear_history();
// 	exit(exit_status);
// }

// int	execute_builtin(char **args, t_env **minienv)
// {
// 	char	*cmd;

// 	cmd = args[0];
// 	if (str_equal(cmd, "echo"))
// 		return (echo(args));
// 	if (str_equal(cmd, "pwd"))
// 		return (pwd());
// 	if (str_equal(cmd, "env"))
// 		return (env(*minienv));
// 	if (str_equal(cmd, "export"))
// 		return (builtin_export(args, minienv));
// 	if (str_equal(cmd, "unset"))
// 		return (unset(args, minienv));
// 	if (str_equal(cmd, "cd"))
// 		return (cd(args, *minienv));
// 	if (str_equal(cmd, "exit"))
// 		return (builtin_exit(args, minienv));
// 	else
// 		return (EXIT_FAILURE);
// }



// int	execute_forked_external(char **args, t_env *minienv)
// {
// 	int		child_pid;
// 	char	*cmd;

// 	cmd = args[0];
// 	child_pid = fork();
// 	define_execute_signals(child_pid);
// 	if (child_pid == -1)
// 		print_perror_msg("fork", cmd);
// 	else if (child_pid == 0)
// 		execute_external(args, minienv);
// 	else
// 		return (wait_for_child(child_pid, TRUE));
// 	exit(EXIT_FAILURE);
// }

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