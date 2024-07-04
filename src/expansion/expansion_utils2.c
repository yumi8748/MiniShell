/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 12:29:11 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 13:58:57 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_valid_varchar(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

int	is_exit_marker(char *s, int i)
{
	if (s[i] == '$' && s[i + 1] == '?')
		return (1);
	if (s[i] == '$' && s[i + 1] == '{' && s[i + 2] == '?' && s[i + 3] == '}')
		return (1);
	return (0);
}


static int handle_quoted_section(char *s, int i)
{
    if (s[i] == '\'')
    {
        i = skip_quotes(s, i, '\'');
    }
    else if (s[i] == '\"')
    {
        i++;
        while (s[i] && s[i] != '\"')
        {
            if (is_exit_marker(s, i))
                return (i);
            i++;
        }
        if (s[i])
            i++;
    }
    return i;
}

int find_exit_marker(char *s)
{
    int i = 0;
    while (s[i])
    {
        if (s[i] == '\'' || s[i] == '\"')
        {
            i = handle_quoted_section(s, i);
            if (s[i] == '\0' || is_exit_marker(s, i))
                break;
        }
        else if (is_exit_marker(s, i))
        {
            return (i);
        }
        if (s[i])
            i++;
        else
            break;
    }
    return (s[i] && is_exit_marker(s, i)) ? i : -1;
}

