/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:12:23 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/05 16:05:42 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
			*str = -1;
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