/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 20:56:42 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/14 20:57:02 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
