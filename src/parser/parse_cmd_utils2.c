/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:12:23 by yu-chen           #+#    #+#             */
/*   Updated: 2024/07/04 12:50:06 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	replace_inquote_pipe(char *s)
{
	char	quote;

	quote = 0;
	while (*s)
	{
		if ((*s == '\"' || *s == '\'') && !quote)
			quote = *s;
		else if (*s == quote)
			quote = 0;
		else if (*s == '|' && quote)
			*s = '\x01';
		s++;
	}
}

void	reset_inquote_pipe(char **splited_cmds)
{
	char	*cmd;

	while (*splited_cmds)
	{
		cmd = *splited_cmds;
		while (*cmd)
		{
			if (*cmd == '\x01')
				*cmd = '|';
			cmd++;
		}
		splited_cmds++;
	}
}

char	**split_all_cmds(char *cmd_line)
{
	char	*cmd_copy;
	char	**splited_cmds;

	cmd_copy = ft_strdup(cmd_line);
	replace_inquote_pipe(cmd_copy);
	splited_cmds = ft_split(cmd_copy, '|');
	reset_inquote_pipe(splited_cmds);
	free(cmd_copy);
	return (splited_cmds);
}

char	**split_one_arg(char *cmd, t_env **minienv)
{
	char	**args;
	char	*command_copy;
	int		i;

	i = 1;
	replace_tabs_with_spaces(cmd);
	if (!has_quote(cmd))
	{
		command_copy = find_and_replace_tilde(cmd, *minienv);
		args = ft_split(command_copy, ' ');
		return (free(command_copy), args);
	}
	replace_inquote_spaces(cmd);
	args = ft_split(cmd, ' ');
	reset_spaces(args);
	replace_quote_by_null(args);
	if (contains_alpha(args[0]) || ((args[0][0] == '\'' || args[0][0] == '"')
				&& args[0][1] == ' '))
		remove_quotes(args[0]);
	while (args[i])
	{
		remove_quotes(args[i]);
		i++;
	}
	return (args);
}

int	contains_alpha(char *args)
{
	while (*args)
	{
		if (ft_isalpha(*args))
			return (1);
		args++;
	}
	return (0);
}
