/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_errors2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:05:06 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/15 17:58:28 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int empty_pipe(char *str)
{
    char *current;
	
	current = str;
    while (*current) 
	{
        if (*current == '|')
		{
            current++;
            while (*current == ' ' || *current == '\t')
                current++;
            if (*current == '|')
                return (print_syntax_error("|"));
        } 
		else
            current++;
    }
    return (0);
}


int redirect_without_label(char *str)
{
    char *current;
    
	current = str;
	while (*current)
	{
        if (*current == '<' || *current == '>')
		{
            if (*(current + 1) == *current) // Handle '<<' or '>>'
                current++;
            current++;
            while (*current == ' ' || *current == '\t')
                current++;
            if (*current == '\0')
                return (print_syntax_error("newline"));
            if (is_invalid_token(*current))
                return (unexpected_token(current));
        }
		else
            current++;
    }
    return (0);
}

int	start_with_pipe(char *str)
{
	if (str[0] == '|')
		return (print_syntax_error("|"));
	return (0);
}

int is_invalid_syntax(char *s) 
{
    if (start_with_pipe(s))
        return (1);
    if (redirect_without_label(s))
        return (1);
    if (empty_pipe(s))
        return (1);
    return (0);
}
