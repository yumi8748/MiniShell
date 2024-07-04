/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:03:42 by yu-chen           #+#    #+#             */
/*   Updated: 2024/07/01 19:20:06 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_invalid_token(char c)
{
	if (c == '|')
		return (1);
	if (c == '>')
		return (1);
	if (c == '<')
		return (1);
	return (0);
}

int	unexpected_token(char *input)
{
	if (input[0] == '<' && input[1] == '<')
		return (print_syntax_error("<<"));
	else if (input[0] == '>' && input[1] == '>')
		return (print_syntax_error(">>"));
	input[1] = '\0';
	return (print_syntax_error(input));
}
