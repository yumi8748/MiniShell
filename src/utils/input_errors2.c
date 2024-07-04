/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_errors2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:05:06 by yu-chen           #+#    #+#             */
/*   Updated: 2024/07/01 18:59:07 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	has_empty_pipe(char *input)
{
	char	*next_pipe;

	next_pipe = get_next_pipe(input);
	if (!next_pipe)
		return (FALSE);
	next_pipe++;
	while (*next_pipe == ' ' || *next_pipe == '\t')
		next_pipe++;
	if (*next_pipe == '|')
		return (print_syntax_error("|"));
	return (has_empty_pipe(next_pipe));
}

int	redirect_without_label(char *input)
{
	char	next_redirect;
	char	*redirect_position;

	next_redirect = get_redir_symbol(input);
	redirect_position = get_redirect_position(input, next_redirect);
	if (!redirect_position)
		return (FALSE);
	if (redirect_position[0] == '<' && redirect_position[1] == '<')
		redirect_position++;
	else if (redirect_position[0] == '>' && redirect_position[1] == '>')
		redirect_position++;
	redirect_position++;
	while (*redirect_position == ' ' || *redirect_position == '\t')
		redirect_position++;
	if (*redirect_position == '\0')
		return (print_syntax_error("newline"));
	if (is_invalid_token(*redirect_position))
		return (unexpected_token(redirect_position));
	return (redirect_without_label(redirect_position));
}

int	start_with_pipe(char *str)
{
	if (str[0] == '|')
		return (print_syntax_error("|"));
	return (0);
}

int	is_invalid_syntax(char *s)
{
	if (start_with_pipe(s))
		return (1);
	if (redirect_without_label(s))
		return (1);
	if (has_empty_pipe(s))
		return (1);
	return (0);
}
