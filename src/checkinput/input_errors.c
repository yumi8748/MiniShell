/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:53:46 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/03 18:03:28 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	print_unclosedquote_err(void)
{
	ft_putstr_fd("⚠️ There is/are unclosed quote(s)!!\n", 2);
}

int	has_unclosed_quotes(char *str)
{
	char	last_opened;

	last_opened = 0;
	while (*str && !last_opened)
	{
		if (*str == '\'' || *str == '"')
			last_opened = *str;
		str++;
	}
	while (*str && last_opened)
	{
		if (*str && *str == last_opened)
			last_opened = 0;
		str++;
	}
	if (*str)
		return (has_unclosed_quotes);
	else if (!last_opened)
		return (FALSE);
	else
		return (print_unclosedquote_err());
}

int	is_empty(char *str)
{
	if (!str)
		return (TRUE);
	while (*str)
	{
		if (*str != ' ')
			return (FALSE);
		str++;
	}
	return (TRUE);
}

int	has_input_error(char *input, int *exit_status, t_env *minienv)
{
	int	res;

	res = FALSE;
	if (is_empty(input))
		res = TRUE;
	else if (has_unclosed_quotes(input))
		res = TRUE;
	else if (is_invalid_syntax(input))
	{
		*exit_status = 2; // why 2?
		res = TRUE;
	}
	else if (handle_heredoc(input, exit_status, minienv) == FAIL) // not done
		res = TRUE;
	if (res == TRUE)
		free(input);
	return (res);
}
