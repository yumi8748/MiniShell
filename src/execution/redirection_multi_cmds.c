/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_multi_cmds.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:13:08 by leochen           #+#    #+#             */
/*   Updated: 2024/07/01 19:08:26 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_pipe(char **splited_cmds, int i, int original_fd[2])
{
	static int	pipefd[2];

	if (i == 0)
	{
		if (pipe(pipefd) == -1)
			print_error_msg("pipe", splited_cmds[i]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	else if (i < str_strlen(splited_cmds) - 1)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		if (pipe(pipefd) == -1)
			print_error_msg("pipe", splited_cmds[i]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	else
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		dup2(original_fd[1], STDOUT_FILENO);
		close(original_fd[1]);
	}
}

void	_handle_redirects(char *cmd, char **splited_cmds, t_env **minienv)
{
	char	redir_symbol;

	redir_symbol = get_redir_symbol(cmd);
	while (redir_symbol != 0)
	{
		if (redir_symbol == '<')
		{
			if (_handle_infile_redir(cmd) == 0)
				clean_exit(splited_cmds, minienv);
		}
		else if (redir_symbol == '>')
		{
			if (_handle_outfile_redir(cmd) == 0)
				clean_exit(splited_cmds, minienv);
		}
		else if (redir_symbol == -2)
		{
			if (_handle_outfile_redir(cmd) == 0)
				clean_exit(splited_cmds, minienv);
		}
		else if (redir_symbol == -1)
			redirect_heredoc(cmd, redir_symbol);
		redir_symbol = get_redir_symbol(cmd);
	}
}

int	_handle_infile_redir(char *command)
{
	char	*infile_redir;
	char	*file;
	int		fd;

	infile_redir = find_redir_pos(command, '<');
	if (!infile_redir)
		return (1);
	file = name_after_redirect(infile_redir);
	fd = open(file, O_RDONLY, FD_CLOEXEC);
	if (fd == -1)
	{
		print_perror_msg("open", file);
		free(file);
		return (0);
	}
	else
		redirect_fd(fd, STDIN_FILENO);
	free(file);
	return (1);
}

int	_handle_outfile_redir(char *command)
{
	char	*outfile_redir;
	char	*file;
	int		fd;
	int		open_flags;

	outfile_redir = find_redir_pos(command, '>');
	if (!outfile_redir)
		return (1);
	if (outfile_redir[1] == '>')
		open_flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		open_flags = O_WRONLY | O_CREAT | O_TRUNC;
	file = name_after_redirect(outfile_redir);
	fd = open(file, open_flags, 0644);
	if (fd == -1)
	{
		print_perror_msg("open", file);
		free(file);
		return (0);
	}
	else
		redirect_fd(fd, STDOUT_FILENO);
	free(file);
	return (1);
}
