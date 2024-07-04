/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:59:43 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 14:00:00 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	skip_quotes(char *s, int i, char quote_type)
{
	i++;
	while (s[i] && s[i] != quote_type)
		i++;
	if (s[i])
		i++;
	return (i);
}

char	*find_here_pos(char *str)
{
    int	i;

    i = 0;
    while (str[i] && str[i + 1])
    {
        if (str[i] == '\'' || str[i] == '\"')
        {
            i = skip_quotes(str, i, str[i]);
            if (str[i] == '\0') 
                 break;
        }
        if (str[i] == '<' && str[i + 1] == '<')
            return (&str[i]);
        else
            i++;
    }
    return (NULL);
}

char	*name_after_redirect(char *s)
{
	int		start;
	int		end;
	char	*name_after_redirect;
	char	*rest_cmd;

	start = 0;
	ft_memmove(s, s + 1, ft_strlen(s + 1) + 1);
	if (s[start] == '>')
		ft_memmove(s, s + 1, ft_strlen(s + 1) + 1);
	while (s[start] == ' ' || s[start] == '\t')
		start++;
	end = find_end(s, start);
	name_after_redirect = ft_substr(&s[start], 0, end - start);
	rest_cmd = &s[end];
	ft_memmove(s, rest_cmd, ft_strlen(rest_cmd) + 2);
	return (name_after_redirect);
}

int	find_end(char *s, int start)
{
	int	end;

	end = start;
	while (s[end] && !is_label_delimiter(s[end]))
	{
		if (s[end] == '\'')
		{
			ft_memmove(&s[end], &s[end] + 1, ft_strlen(&s[end] + 1) + 1);
			while (s[end] && s[end] != '\'')
				end++;
			ft_memmove(&s[end], &s[end] + 1, ft_strlen(&s[end] + 1) + 1);
		}
		else if (s[end] == '\"')
		{
			ft_memmove(&s[end], &s[end] + 1, ft_strlen(&s[end] + 1) + 1);
			while (s[end] && s[end] != '\"')
				end++;
			ft_memmove(&s[end], &s[end] + 1, ft_strlen(&s[end] + 1) + 1);
		}
		else if (s[end] && !is_label_delimiter(s[end]))
			end++;
	}
	return (end);
}

int	is_label_delimiter(char c)
{
	if (c == ' ' || c == '>' || c == '<' || c == '|' || c == '\t')
		return (1);
	return (0);
}
