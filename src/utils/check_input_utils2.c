/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:03:42 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/05 15:28:53 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_invalid_token(char c)
{
	if (c == '|')
		return (TRUE);
	if (c == '>')
		return (TRUE);
	if (c == '<')
		return (TRUE);
	return (FALSE);
}

int	unexpected_token(char *input)
{
	if (input[0] == '<' && input[1] == '<')
		return (syntax_error("<<"));
	else if (input[0] == '>' && input[1] == '>')
		return (syntax_error(">>"));
	input[1] = '\0';
	return (syntax_error(input));
}