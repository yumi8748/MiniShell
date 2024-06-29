/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:20:55 by leochen           #+#    #+#             */
/*   Updated: 2024/06/29 15:16:09 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*char	get_redir_symbol(char *s)  //返回的是具体的定向符号 或者返回-1(<<)
{
		int	i;

	i = 0;
	while (s[i])
		{
			if (s[i] == '\'' || s[i] == '\"')
			skip_quotes(s, i, s[i]);
		else if (s[i] == '<' && s[i+1] == '<') // check for '<<'
			return (-1); // return -1 for '<<'
		else if (s[i] == '<' || s[i] == '>')
				return (s[i]);
		i++;
		}
		return (0);
 }*/
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
