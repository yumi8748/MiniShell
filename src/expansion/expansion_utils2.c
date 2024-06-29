/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 12:29:11 by leochen           #+#    #+#             */
/*   Updated: 2024/06/28 12:53:31 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

 int	is_valid_varchar(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

int find_exit_marker(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'')
			i = skip_quotes(s, i, '\'');
		else if (s[i] == '\"')
		{
			i++;
			while (s[i] && s[i] != '\"')
			{
				if ((s[i] == '$' && s[i + 1] == '?')
					||(s[i] == '$' && s[i + 1] == '{' && s[i + 2] == '?'
					&& s[i + 3] == '}'))
					return (i);
				i++;
			}
			if (s[i])
				i++;
		}
		if((s[i] == '$' && s[i+1] == '?')
			||(s[i] == '$' && s[i + 1] == '{' && s[i + 2] == '?'
					&& s[i + 3] == '}'))
			return (i);
		i++;
	}
	return (-1);
}


