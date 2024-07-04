/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_arg_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 19:56:33 by yu-chen           #+#    #+#             */
/*   Updated: 2024/07/04 13:15:34 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	replace_single_quotes(char *str, char limiter)
{
	if (*str == limiter && *(str + 1) == limiter)
	{
		*(str + 1) = -2;
		*str = -2;
		str++;
	}
	while (*str && *str != limiter)
		str++;
	if (*str)
		str++;
	while (*str == limiter)
	{
		*(str - 1) = -2;
		*str = -2;
		str++;
	}
	if (*str)
		str++;
	if (*str)
		replace_single_quotes(str, limiter);
}

/*void	replace_double_quotes(char *str, char limiter)
{
	if (*str == limiter && *(str + 1) == limiter)
	{
		*(str + 1) = -3;
		*str = -3;
		str++;
	}
	while (*str && *str != limiter)
		str++;
	if (*str)
		str++;
	while (*str == limiter)
	{
		*(str - 1) = -3;
		*str = -3;
		str++;
	}
	if (*str)
		str++;
	if (*str)
		replace_double_quotes(str, limiter);
}*/

void replace_double_quotes(char *str, char limiter)
{
    while (*str)
    {
        if (*str == limiter && *(str + 1) == limiter)
        {
            *(str + 1) = -3;
            *str = -3;
            str += 2;
        }
        else
        {
            while (*str && *str != limiter)
                str++;
            if (*str)
                str++;
            while (*str == limiter)
            {
                *(str - 1) = -3;
                *str = -3;
                str++;
            }
        }
    }
}

static void	replace_spaces(char *str, char limiter)
{
	while (*str && *str != limiter)
		str++;
	if (*str)
		str++;
	while (*str && *str != limiter)
	{
		if (*str == ' ')
			*str = -1;
		str++;
	}
	if (*str)
		str++;
	if (*str)
		replace_spaces(str, limiter);
}

void	replace_space_and_quote(char *command)
{
	replace_spaces(command, '\'');
	replace_single_quotes(command, '\'');
	replace_spaces(command, '"');
	replace_double_quotes(command, '"');
}
