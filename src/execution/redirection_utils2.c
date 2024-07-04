/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:06:01 by leochen           #+#    #+#             */
/*   Updated: 2024/07/01 18:10:13 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	duplicate_fd(int fd, int new_fd)
{
	dup2(fd, new_fd);
	close(fd);
}

void	stock_original_fds(int fd, int new_fd)
{
	if (fd == -1)
		fd = dup(new_fd);
}

void	print_fd_error(char *file, int fd, int new_fd)
{
	print_perror_msg("open", file);
	free(file);
	duplicate_fd(fd, new_fd);
}

int	process_single_redirect(char *cmd, char redir_symbol, int original_fds[2])
{
	if (redir_symbol == '<')
	{
		if (!handle_infile_redir(cmd, original_fds))
			return (restore_fds(original_fds));
	}
	else if (redir_symbol == '>')
	{
		if (!handle_outfile_redir(cmd, original_fds))
			return (restore_fds(original_fds));
	}
	else if (redir_symbol == -2)
	{
		if (!handle_outfile_redir(cmd, original_fds))
			return (restore_fds(original_fds));
	}
	else if (redir_symbol == -1)
	{
		stock_original_fds(original_fds[0], STDIN_FILENO);
		stock_original_fds(original_fds[1], STDOUT_FILENO);
		redirect_heredoc(cmd, redir_symbol);
	}
	return (1);
}
