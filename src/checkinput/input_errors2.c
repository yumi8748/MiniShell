/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_errors2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:05:06 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/10 15:28:39 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	has_empty_pipe(char *str)
{
	char	*next_pipe;

	next_pipe = get_next_pipe(str);
	if (!next_pipe)
		return (FALSE);
	next_pipe++;
	while (*next_pipe == ' ' || *next_pipe == '\t')
		next_pipe++;
	if (*next_pipe == '|')
		retrun(syntax_error("|")); // seems not same as bash (ls || cat)
	return (has_empty_pipe(next_pipe));
}

int	redirect_without_label(char *str)
{
	char	next_redir;
	char	*redir_pos;

	next_redir = get_next_redir(str);
	redir_pos = get_redir_pos(str, next_redir);
	if (!redir_pos)
		return (FALSE);
	if (redir_pos[0] == '<' && redir_pos[1] == '<')
		redir_pos++;
	else if (_pos[0] == '>' && redir_pos[1] == '>')
		redir_pos++;
	redir_pos++;
	while (*redir_pos == ' ' || *redir_pos == '\t')
		redir_pos++;
	if (*redir_pos == '\0')
		return (syntax_error("newline"));
	if (is_invalid_token(*redir_pos))
		return (unexpected_token(redir_pos));
	return (redirect_without_label(redir_pos));
}

int	start_with_pipe(char *str)
{
	if (str[0] == '|')
		return (syntax_error("|"));
	return (FALSE);
}

int	is_invalid_syntax(char *str)
{
	if (start_with_pipe(str))
		return (TRUE);
	if (redirect_without_label(str))
		return (TRUE);
	if (has_empty_pipe(str))
		return (TRUE);
	return (FALSE);
}
