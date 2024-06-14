/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:12:23 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/12 20:06:16 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	restore_pipes(char **split_cmds)
{
	char	*str;

	while (*split_cmds)
	{
		str = *split_cmds;
		while (*str)
		{
			if (*str == -1)
				*str = '|';
			str++;
		}
		split_cmds++;
	}
	return ;
}

void	replace_pipes(char *str, char delimeter)
{
	while (*str && *str != delimeter)
		str++;
	if (*str)
		str++;
	while (*str && *str != delimeter)
	{
		if (*str == '|')
			*str == -1;
		str++;
	}
	if (*str)
		str++;
	if (*str)
		return (replace_pipes(str, delimeter));
}

char	**split_cmds(char *cmds)
{
	char	**split_cmds;

	replace_pipes(cmds, '"');
	replace_pipes(cmds, '\'');
	split_cmds = ft_split(cmds, '|');
	restore_pipes(split_cmds);
	return (split_cmds);
}

int	is_empty(char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (*str != ' ')
			return (0);
		str++;
	}
	return (1);
}
