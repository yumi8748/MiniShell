/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 16:25:34 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/06 15:51:42 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*find_var_pos(char *input)
{
	while (*input)
	{
		if (*input == '\'')
		{
			input++;
			while (*input && *input != '\'')
				input++;
		}
		if (*input == '\"')
		{
			input++;
			while (*input && *input != '\"')
			{
				if (*input == '$' && is_varname(input[1]))
					return (input);
				input++;
			}
		}
		if (*input == '$' && is_varname(input[1]))
			return (input);
		input++;
	}
	return (NULL);
}

void	update_input(char **input, char *var_value, char *second_part)
{
	char	*first_part;
	char	*updated_input;

	if (!*input[0] && !var_value)
		first_part = ft_strdup("");
	else if (!*input[0] && var_value) // not likely
		first_part = ft_strdup(var_value);
	else if (!var_value)
		first_part = ft_strdup(*input);
	else
		first_part = ft_strjoin(*input, var_value);
	updated_input = ft_strjoin(first_part, second_part);
	free(first_part);
	free(*input);
	*input = updated_input;
}

void	expand_variables(char **input, t_env *minienv)
{
	char	*var_pos;
	char	*var_name;
	char	*var_value;
	int		name_size;

	var_pos = find_var_pos(*input);
	if (var_pos)
	{
		name_size = 0;
		while (is_varname(var_pos[name_size + 1]))
			name_size++;
		var_name = ft_substr(var_pos, 1, name_size);
		*var_pos = '\0';
		var_value = minienv_value(var_name, minienv);
		update_input(input, var_value, (var_pos + 1 + name_size));
		free(var_name);
		expand_variables(input, minienv);
	}
}

void	handle_expansions(char **input, t_env *minienv, int exit_status)
{
	expand_exit_status(input, exit_status);
	expand_variables(input, minienv);
}
