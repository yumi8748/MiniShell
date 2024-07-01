/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:04:51 by leochen           #+#    #+#             */
/*   Updated: 2024/07/01 15:31:54 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


/*leochen@paul-f3Ar4s1:/sgoinfre/goinfre/Perso/leochen/minishell_$ <<'lim"'
> lim
> lim"     
leochen@paul-f3Ar4s1:/sgoinfre/goinfre/Perso/leochen/minishell_$ <<"lim' "
> lim'
> lim' 
leochen@paul-f3Ar4s1:/sgoinfre/goinfre/Perso/leochen/minishell_$ <<'lim'
> lim
leochen@paul-f3Ar4s1:/sgoinfre/goinfre/Perso/leochen/minishell_$ <<"'lim'" 
> lim
> 'lim'
leochen@paul-f3Ar4s1:/sgoinfre/goinfre/Perso/leochen/minishell_$ <<'"li"m'
> lim
> "li"m
leochen@paul-f3Ar4s1:/sgoinfre/goinfre/Perso/leochen/minishell_$ <<"$?"
> $?
"" 和 '' 之间的都是delimiter   包括之间的不一样种类的单个或多个因号  也没有expansion 
*/



int handle_heredoc(char *input, int *exit_status, t_env *minienv)
{
    static int heredoc_ref;
    char *here_symbol_ptr;
    char *delimiter;
	

    // 初始化 heredoc_ref
    heredoc_ref = 0;
    // 循环查找和处理每个 heredoc
    while ((here_symbol_ptr = find_here_pos(input)) != NULL)
    {
        heredoc_ref--; // 每次找到 heredoc 标志时递减编号
		*here_symbol_ptr = heredoc_ref; //用heredoc标识符 代替第一个< 也就是<<lim 里面的lim旁边的< 为的是execute后再次redir_heredoc时不会再次找到单独的<从而进行错误的redir 因为那时cmd已经被处理过了 只剩下cat< 也就是替换后的cat-1
		here_symbol_ptr ++;  //跳过第一个<
        // 获取 heredoc 的分隔符名称
        delimiter = name_after_redirect(here_symbol_ptr);  //这里传入的heresymbol其实是从第二个<开始的
        // 执行 heredoc 处理
       // printf("input:%s\n", input);
		if (!exec_heredoc(delimiter, heredoc_ref, exit_status, minienv))
        {
            free_str(delimiter); // 如果执行失败，释放内存并返回失败
            return (0);
        }
        free_str(delimiter); // 释放内存
    }
    return (1); // 成功处理所有 heredoc，返回成功
}


int exec_heredoc(char *delimiter, int heredoc_ref, int *exit_status, t_env *minienv)
{
    int pid;

    pid = fork();  // 创建子进程
    define_heredoc_signals(pid);  // 定义信号处理程序
    if (pid == -1)
    {
        print_perror_msg("fork - heredoc_prompt", delimiter);  // 打印错误信息
        return 0;
    }
    else if (pid == 0)
        read_heredoc(exit_status, minienv, delimiter, heredoc_ref);  // 子进程读取 heredoc
    else
    {
        *exit_status = wait_for_child(pid, TRUE);  // 父进程等待子进程完成
        define_main_signals();  // 重新定义信号处理程序

        if (*exit_status != EXIT_SUCCESS) //0
            return (0);
    }

    return (1);
}


void read_heredoc(int *exit_status, t_env *minienv, char *delimiter, int heredoc_number)
{
	int file_fd;
    char *line_read;
    char *file;

    file = tmp_here_file(heredoc_number);  // 生成临时文件名储存heredoc_ref  /tmp/heredoc-1
	file_fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);  // 打开该临时文件  0644 作为文件的权限参数，表示创建一个新文件，文件所有者有读写权限，文件所有组和其他用户只有读权限
	free_str(file);
	line_read = readline("> ");  // 读取用户输入  char *readline(const char *prompt);    char *readline(const char *prompt);
    while (line_read && ft_strncmp(line_read, delimiter, ft_strlen(delimiter)) != 0)
    {
		expand_exit_status(&line_read, *exit_status);  // 扩展退出状态
    	expand_variables(&line_read, minienv);  // 扩展变量
        ft_putendl_fd(line_read, file_fd);  // 写入临时文件
        free_str(line_read); //readline 函数会自动分配内存来存储用户输入的字符串，因此在使用完这块内存后，我们需要使用 free 函数将其释放
    }
	if (!line_read)
		print_error_msg("warning: here-document delimited by end-of-file. Wanted", delimiter);
    free_str(line_read);
    close(file_fd);
    free_str(delimiter);
    free_minienv(&minienv);
    rl_clear_history();
    exit(EXIT_SUCCESS);  // 退出子进程
}

char *tmp_here_file(int heredoc_ref)  //生成一个临时文件名，用于存储heredo编号 ex:/tmp/heredoc-1 ,-2, -3 
{
    char file[30];
    char *here_ref;

	ft_memset(file, 0, 30);  // 将 file 数组的所有元素设置为 0
    here_ref = ft_itoa(heredoc_ref);  // 将 heredoc_number 转换为字符串
    ft_strlcat(file, "/tmp/heredoc", 30);  // 将 "/tmp/heredoc" 拼接到 filename 中
    ft_strlcat(file, here_ref, 30);  // 将 number_str 拼接到 filename 中
    free_str(here_ref);  // 释放 number_str 内存
    return (ft_strdup(file));  // 返回 filename 的副本
}

// static int	is_control_c(int status)  //SIGINT：2，中断进程，通常是用户按下 Ctrl+C 时发送的信号
// {
// 	return (WTERMSIG(status) == SIGINT); //WTERMSIG 从一个子进程的退出状态中提取出导致其终止的信号号  Unix/Linux 系统上，信号号的范围是从 1 到 64 之间的整数 。
// }

// static int	is_control_slash(int status) //SIGQUIT：3，退出进程，通常是用户按下 Ctrl+\ 时发送的信号
// {
// 	return (WTERMSIG(status) == SIGQUIT);
// }

// static int	handle_signal_interrupt(int status, int is_last_child)
// {
// 	if (is_control_c(status))
// 		ft_putstr_fd("\n", STDOUT_FILENO);
// 	if (is_control_slash(status) && is_last_child)
// 		ft_putstr_fd("Quit\n", STDOUT_FILENO);
// 	return (128 + WTERMSIG(status));  //当一个子进程因为接收到一个信号而终止时，它的退出状态会被设置为一个特殊的值，该值是由信号号和一个常量 128 相加得到的
// }

