/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:29:46 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/21 17:57:53 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// struct sigaction 结构体用于描述信号处理程序的行为。它包含以下成员：

//     sa_handler：信号处理函数的指针，或者是常量 SIG_IGN（忽略信号）和 SIG_DFL（默认处理）。
//     sa_sigaction：与 sa_handler 类似，但提供更多信息。
//     sa_mask：信号集，在处理该信号时需要阻塞的其他信号。
//     sa_flags：标志，控制信号处理行为。
//     sa_restorer：不常用，保留为恢复函数

//当用户在终端中按下 Ctrl+D 时，会向终端发送一个 EOF（End Of File）信号，表示输入已经结束。在 readline 库中，如果检测到用户按下了 Ctrl+D，会立即停止读取用户的输入，并返回一个空指针 NULL，表示没有读到任何内容。
//在当前的代码中，没有专门处理 Ctrl+D 的情况。当用户按下 Ctrl+D 时，readline 库会返回 NULL，然后代码会检查 line 是否为空，如果为空，就会退出循环，从而实现了退出 minishell 的功能

static void	handle_sigint(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line(); //用户已经在当前行输入了内容并且按下了回车键或者其他的行结束键（如 Ctrl-D），因此 Readline 库应该将光标移动到下一行，准备接受用户的新一行的输入。
	rl_replace_line("", 0); //替换当前行的内容为 “”   第二个参数clear_undo：如果为非零，则清除 undo 记录。
	rl_redisplay(); //刷新并重新显示当前输入行。这将在用户按下 Ctrl+C 键组合后，清除当前行并将光标移动到下一行。
}


void	define_main_signals(void)
{
	struct sigaction sa_sigint;   //ctrl+c
	struct sigaction sa_sigquit;  //ctrl+/ 
   

	sa_sigint.sa_handler = &handle_sigint;
	sa_sigint.sa_flags = 0;
	sigemptyset(&sa_sigint.sa_mask);
	sigaction(SIGINT, &sa_sigint, NULL);
	
	sa_sigquit.sa_handler = SIG_IGN;
	sa_sigquit.sa_flags = 0; //a_flags 成员设置为 0。sa_flags 成员用于指定信号处理程序的行为，例如是否重新启动系统调用，是否阻塞信号等。在这里，将其设置为 0 表示使用默认的行为
	sigemptyset(&sa_sigquit.sa_mask); //将 sa_sigint 结构体中的 sa_mask 成员清空。sa_mask 成员是一个信号集，用于指定在信号处理程序执行期间要阻塞的信号。在这里，将其清空表示不阻塞任何信号。
	sigaction(SIGQUIT, &sa_sigquit, NULL); //调用 sigaction 函数，用于注册 SIGINT 信号的处理程序
}


void	define_heredoc_signals(int child_pid)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	sa_sigint.sa_flags = 0;
	sigemptyset(&sa_sigint.sa_mask);
	if (child_pid == 0) //如果 child_pid 为 0，则表示当前进程是子进程，即正在运行 heredoc 输入过程。在这种情况下，将 sa_sigint.sa_handler 设置为 SIG_DFL，即将 SIGINT 信号的处理方式设置为默认的。这意味着，如果用户在 heredoc 输入过程中按下 Ctrl+C，将会发送 SIGINT 信号给子进程，子进程将会终止，从而中断 heredoc 输入过程。
		sa_sigint.sa_handler = SIG_DFL;
	else     //如果 child_pid 不为 0，则表示当前进程是父进程，即 heredoc 输入过程已经结束。在这种情况下，将 sa_sigint.sa_handler 设置为 SIG_IGN，即将 SIGINT 信号的处理方式设置为忽略。这意味着，如果用户在父进程中按下 Ctrl+C，将会发送 SIGINT 信号给父进程，但是父进程将会忽略此信号，从而不会中断父进程的运行。
		sa_sigint.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_sigint, NULL);
	
	sa_sigquit.sa_flags = 0;
	sigemptyset(&sa_sigquit.sa_mask);
	sa_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_sigquit, NULL);

}

void	define_execute_signals(int child_pid)
{
	struct sigaction	sa;

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (child_pid == 0)
		sa.sa_handler = SIG_DFL;
	else
		sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}