/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_cmd_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:22:35 by leochen           #+#    #+#             */
/*   Updated: 2024/06/27 18:23:09 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int has_quote(char *cmd)
{
	while (cmd && *cmd)
	{
		if (*cmd == '\'' || *cmd == '\"')
			return (1);
		cmd++;
	}
	return (0);
}

void replace_inquote_spaces(char *cmd)  //在这段代码中，`cmd[i] = -1;` 是将字符串 `cmd` 中的空格字符（' '）替换为 ASCII 值为 -1 的字符。这样做的目的是为了在后续的代码中，将这些字符作为分隔符来分割字符串。
{
    int i;
    char quote;

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
                    cmd[i] = -1;
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
			if (args[i][j] == -1)
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