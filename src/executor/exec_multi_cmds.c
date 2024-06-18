/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multi_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 15:52:51 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/14 20:56:49 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	save_original_fds(int original_fds[2])
{
	original_fds[0] = dup(STDIN_FILENO);
	original_fds[1] = dup(STDOUT_FILENO);
}

static void	restore_original_fds(int original_fds[2])
{
	redirect_fd(original_fds[IN], STDIN_FILENO);
	redirect_fd(original_fds[OUT], STDOUT_FILENO);
}

static void	exec_forked_cmd(char *cmd, char **cmds, t_env **minienv)
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

static void	handle_redirects(char *cmd, char **cmds, t_env **minienv)
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
	restore_original_fds(original_fds);
	return (wait_for_children(children_pid));
}
