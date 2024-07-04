/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:53:46 by yu-chen           #+#    #+#             */
/*   Updated: 2024/07/01 18:58:58 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	print_unclosedquote_err(void)
{
	ft_putstr_fd("⚠️ There is/are unclosed quote(s)!!\n", 2);
	return (1);
}

int	is_unclosed_quotes(char *s)
{
	char	quote;

	quote = 0;
	while (*s)
	{
		if (*s == '\'' || *s == '\"')
		{
			if (quote == 0)
				quote = *s;
			else if (quote == *s)
				quote = 0;
		}
		s++;
	}
	if (quote)
		return (print_unclosedquote_err());
	return (0);
}

int	is_empty(char *str)
{
	if (!str || str[0] == '\0')
		return (1);
	while (*str)
	{
		if (ft_isspace(*str) == 0)
			return (0);
		str++;
	}
	return (1);
}

int	is_input_error(char *input, int *exit_status, t_env *minienv)
{
	int	is_error;

	is_error = 0;
	if (is_empty(input))
		is_error = 1;
	else if (is_unclosed_quotes(input))
		is_error = 1;
	else if (is_invalid_syntax(input))
	{
		*exit_status = 2;
		is_error = 1;
	}
	else if (handle_heredoc(input, exit_status, minienv) == 0)
		is_error = 1;
	if (is_error == 1)
		free_str(input);
	return (is_error);
}
