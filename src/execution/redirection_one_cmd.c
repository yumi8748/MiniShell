/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_one_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:18:57 by leochen           #+#    #+#             */
/*   Updated: 2024/07/01 18:09:51 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_infile_redir(char *cmd, int original_fd[2])
{
	char	*infile_redir;
	char	*file;
	int		fd;

	if (original_fd[0] == -1)
		original_fd[0] = dup(STDIN_FILENO);
	infile_redir = find_redir_pos(cmd, '<');
	if (!infile_redir)
		return (1);
	file = name_after_redirect(infile_redir);
	fd = open(file, O_RDONLY, FD_CLOEXEC);
	if (fd == -1)
	{
		print_perror_msg("open", file);
		free(file);
		duplicate_fd(original_fd[0], STDIN_FILENO);
		return (0);
	}
	else
		duplicate_fd(fd, STDIN_FILENO);
	free(file);
	return (1);
}

int	handle_outfile_redir(char *cmd, int original_fd[2])
{
	char	*outfile_redir;
	char	*file;
	int		fd;
	int		append;

	if (original_fd[1] == -1)
		original_fd[1] = dup(STDOUT_FILENO);
	outfile_redir = find_redir_pos(cmd, '>');
	if (!outfile_redir)
		return (1);
	append = if_append(*(outfile_redir + 1));
	file = name_after_redirect(outfile_redir);
	if (append == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_fd_error(file, original_fd[1], STDOUT_FILENO);
		return (0);
	}
	else
		duplicate_fd(fd, STDOUT_FILENO);
	free(file);
	return (1);
}

int	restore_fds(int original_fds[2])
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

int	handle_redirects(char *cmd, int original_fds[2])
{
	initialize_original_fds(original_fds);
	return (process_redirects(cmd, original_fds));
}

void	redirect_heredoc(char *cmd, int heredoc_ref)
{
	char	*file;
	int		file_fd;
	char	*here_pos;

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
	here_pos = find_redir_pos(cmd, heredoc_ref);
	ft_memmove(here_pos, here_pos + 1, ft_strlen(here_pos + 1) + 1);
}
