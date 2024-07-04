/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:20:55 by leochen           #+#    #+#             */
/*   Updated: 2024/07/01 19:06:56 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	get_redir_symbol(char *s)
{
	while (*s)
	{
		if (*s == '\'')
		{
			s++;
			while (*s != '\'')
				s++;
		}
		if (*s == '"')
		{
			s++;
			while (*s != '"')
				s++;
		}
		if (*s == '<' || *s == '>' || *s < 0)
			return (*s);
		s++;
	}
	return (0);
}

void	redirect_fd(int fd_to_redirect, int fd_location)
{
	dup2(fd_to_redirect, fd_location);
	close(fd_to_redirect);
}

int	if_append(char c)
{
	if (c == '>')
		return (1);
	return (0);
}

void	initialize_original_fds(int original_fds[2])
{
	original_fds[0] = -1;
	original_fds[1] = -1;
}

int	process_redirects(char *cmd, int original_fds[2])
{
	char	redir_symbol;
	int		result;

	redir_symbol = get_redir_symbol(cmd);
	while (redir_symbol != 0)
	{
		result = process_single_redirect(cmd, redir_symbol, original_fds);
		if (result == 0)
			return (result);
		redir_symbol = get_redir_symbol(cmd);
	}
	return (1);
}
