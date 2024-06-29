/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:22:35 by leochen           #+#    #+#             */
/*   Updated: 2024/06/29 15:18:44 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	has_quote(char *cmd)
{
	while (cmd && *cmd)
	{
		if (*cmd == '\'' || *cmd == '\"')
			return (1);
		cmd++;
	}
	return (0);
}

void	replace_inquote_spaces(char *cmd)
{
	int		i;
	char	quote;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '"' || cmd[i] == '\'')
		{
			quote = cmd[i];
			i++;
			while (cmd[i] && cmd[i] != quote)
			{
				if (cmd[i] == ' ')
					cmd[i] = '\x01';
				i++;
			}
			if (cmd[i])
				i++;
		}
		else
			i++;
	}
}

void	remove_quotes(char *cmd)
{
	char	quote;

	while (*cmd)
	{
		if (*cmd == '\'' || *cmd == '\"')
		{
			quote = *cmd;
			ft_memmove(cmd, cmd + 1, ft_strlen(cmd + 1) + 1);
			while (*cmd && *cmd != quote)
				cmd++;
			if (*cmd)
				ft_memmove(cmd, cmd + 1, ft_strlen(cmd + 1) + 1);
		}
		else
			cmd++;
	}
}

void	reset_spaces(char **args)
{
	int	i;
	int	j;

	i = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] == '\x01')
				args[i][j] = ' ';
			j++;
		}
		i++;
	}
}

int	has_pipe(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
			i = skip_quotes(s, i, s[i]);
		else
		{
			if (s[i] == '|')
				return (1);
			i++;
		}
	}
	return (0);
}
