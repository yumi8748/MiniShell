/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_error3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 20:49:31 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/28 21:02:09 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_next_pipe(char *str)
{
	while (*str)
	{
		if (*str == '\'')
		{
			str++;
			while (*str != '\'')
				str++;
		}
		if (*str == '"')
		{
			str++;
			while (*str != '"')
				str++;
		}
		if (*str == '|')
			return (str);
		str++;
	}
	return (NULL);
}

char	*get_redirect_position(char *str, char redirect_char)
{
	while (*str)
	{
		if (*str == '\'')
		{
			str++;
			while (*str != '\'')
				str++;
		}
		if (*str == '"')
		{
			str++;
			while (*str != '"')
				str++;
		}
		if (*str == redirect_char)
			return (str);
		str++;
	}
	return (NULL);
}

char	get_next_redirect(char *str)
{
	while (*str)
	{
		if (*str == '\'')
		{
			str++;
			while (*str != '\'')
				str++;
		}
		if (*str == '"')
		{
			str++;
			while (*str != '"')
				str++;
		}
		if (*str == '<' || *str == '>' || *str < 0)
			return (*str);
		str++;
	}
	return (0);
}
