/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 19:07:44 by yu-chen           #+#    #+#             */
/*   Updated: 2024/07/03 20:05:14 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	replace_tabs_with_spaces(char *str)
{
	while (*str)
	{
		if (*str == '\t')
			*str = ' ';
		str++;
	}
}

void	replace_quote_by_null(char **exec_args)
{
	int	i;

	i = 0;
	while (exec_args[i])
	{
		if (exec_args[i][0] == '"' && exec_args[i][1] == '"')
		{
			exec_args[i][0] = '\'';
			exec_args[i][1] = '\'';
		}
		if (exec_args[i][0] == '\'' && i != 0
			&& exec_args[i][0] == exec_args[i][1] && exec_args[i][2] == '\0')
			exec_args[i][0] = '\0';
		i++;
	}
}
