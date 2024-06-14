/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multi_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 15:52:51 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/12 18:56:06 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exec_forked_cmd(char *cmd, char **cmds, t_env **minienv)
{
	char	**args;

	close_extra_fds();
	args = split_args(cmd);
	free_array(cmds);
	if (is_builtin(args[0]))
		exec_forked_builtin(args, minienv);
	else
		exec_external(args, *minienv);
}

void	handle_redirects(char *cmd, char **cmds, t_env **minienv)
{
	char	redir;

	redir = get_next_redir(cmd);
	while (redir)
	{
		if (redir == '<')
		{
			if (redir_input(cmd) == FAIL)
				quit_child(cmds, minienv);
		}
		if (redir == '>')
		{
			redir_output();
		}
		if (redir < 0)
		{
			redir_heredoc();
		}
		redir = get_next_redir(cmd);
	}
}

void	handle_pipe(int original_fd_out, char *current_cmd, char **cmds)
{
	int		is_first_cmd;
	int		has_next_cmd;
	char	*last_cmd;
	int		pipe_fd[2];

	is_first_cmd = (current_cmd == cmds[0]);
	last_cmd = cmds[arr_len(cmds) - 1];
	has_next_cmd = (current_cmd != last_cmd);
	if (!is_first_cmd)
		redirect_fd(pipe_fd[IN], STDIN_FILENO);
	if (has_next_cmd)
	{
		if (pipe(pipe_fd) == -1)
			print_perror_msg("pipe", current_cmd);
		redirect_fd(pipe_fd[OUT], STDOUT_FILENO);
	}
	else
		redirect_fd(original_fd_out, STDOUT_FILENO);
}

int	exec_multi_cmds(char **cmds, t_env **minienv)
{
	int	original_fds[2];
	int	*children_pid;
	int	i;

	save_original_fds(original_fds);
	children_pid = init_children_pid(cmds);
	i = 0;
	while (cmds[i])
	{
		handle_pipe(original_fds[OUT], cmds[i], cmds);
		children_pid[i] = fork();
		define_exec_signals(children_pid[i]);
		if (children_pid[i] == -1)
			print_perror_msg("fork", children_pid[i]);
		else if (children_pid[i] == 0)
		{
			free(children_pid);
			handle_redirects(cmds[i], cmds, minienv);
			exec_forked_cmd(cmds[i], cmds, minienv);
		}
		i++;
	}
}
