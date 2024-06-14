/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:17:19 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/11 14:43:10 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	redir_input(char *cmd)
{
	char	*input_redir;
	char	*file_name;
	int		fd;

	input_redir = get_redir_pos(cmd, '<');
	if (!input_redir)
		return (SUCCESS);
	file_name = get_label_name(input_redir);
	fd = open(file_name, O_RDONLY | FD_CLOEXEC);
	if (fd == -1)
	{
		print_perror_msg("open", file_name);
		free(file_name);
		return (FAIL);
	}
	else
		redirect_fd(fd, STDIN_FILENO);
	free(file_name);
	return (SUCCESS);
}

int	redir_output(char *cmd)
{
	char	*output_redir;
	char	*file_name;
	int		fd;
	int		open_flags;

	output_redir = get_redir_pos(cmd, '>');
	if (!output_redir)
		return (SUCCESS);
	if (output_redir[1] == '>')
		open_flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		open_flags = O_WRONLY | O_CREAT | O_TRUNC;
	file_name = get_label_name(output_redir);
	fd = open(file_name, open_flags, 0644);
	if (fd == -1)
	{
		print_perror_msg("open", file_name);
		free(file_name);
		return (FAIL);
	}
	else
		redirect_fd(fd, STDOUT_FILENO);
	free(file_name);
	return (SUCCESS);
}

void	redir_heredoc(char *cmd, int heredoc_number)
{
	
}