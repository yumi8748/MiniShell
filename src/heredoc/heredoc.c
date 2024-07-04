/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:04:51 by leochen           #+#    #+#             */
/*   Updated: 2024/07/02 14:46:47 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_heredoc(char *input, int *exit_status, t_env *minienv)
{
	static int	heredoc_ref;
	char		*here_symbol_ptr;
	char		*delimiter;

	heredoc_ref = 0;
	here_symbol_ptr = find_here_pos(input);
	while (here_symbol_ptr != NULL)
	{
		heredoc_ref--;
		*here_symbol_ptr = heredoc_ref;
		here_symbol_ptr++;
		delimiter = name_after_redirect(here_symbol_ptr);
		if (!exec_heredoc(delimiter, heredoc_ref, exit_status, minienv))
		{
			free_str(delimiter);
			return (0);
		}
		free_str(delimiter);
		here_symbol_ptr = find_here_pos(here_symbol_ptr);
	}
	return (1);
}

int	exec_heredoc(char *delimiter, int heredoc_ref, int *exit_status,
		t_env *minienv)
{
	int	pid;

	pid = fork();
	define_heredoc_signals(pid);
	if (pid == -1)
	{
		print_perror_msg("fork - heredoc_prompt", delimiter);
		return (0);
	}
	else if (pid == 0)
		read_heredoc(exit_status, minienv, delimiter, heredoc_ref);
	else
	{
		*exit_status = wait_for_child(pid, TRUE);
		define_main_signals();
		if (*exit_status != EXIT_SUCCESS)
			return (0);
	}
	return (1);
}

static char	*msg(void)
{
	return ("warning: here-document delimited by end-of-file (wanted: ");
}

void	read_heredoc(int *exit_status, t_env *minienv,
			char *delimiter, int heredoc_number)
{
	char	*line_read;
	char	*filename;
	int		tmp_file_fd;

	filename = tmp_here_file(heredoc_number);
	tmp_file_fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	free(filename);
	line_read = readline("> ");
	while (line_read && !str_equal(line_read, delimiter))
	{
		expand_exit_status(&line_read, *exit_status);
		expand_variables(&line_read, minienv);
		ft_putstr_fd(line_read, tmp_file_fd);
		ft_putchar_fd('\n', tmp_file_fd);
		free(line_read);
		line_read = readline("> ");
	}
	if (!line_read)
		print_error_msg(msg(), delimiter);
	free(line_read);
	close(tmp_file_fd);
	free(delimiter);
	free_minienv(&minienv);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

char	*tmp_here_file(int heredoc_ref)
{
	char	file[30];
	char	*here_ref;

	ft_memset(file, 0, 30);
	here_ref = ft_itoa(heredoc_ref);
	ft_strlcat(file, "/tmp/heredoc", 30);
	ft_strlcat(file, here_ref, 30);
	free_str(here_ref);
	return (ft_strdup(file));
}
