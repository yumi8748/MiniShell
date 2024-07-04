/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:59:36 by leochen           #+#    #+#             */
/*   Updated: 2024/07/01 19:07:24 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	expand_exit_status(char **input, int exit_status)
{
	int		i;
	char	*exit_str;
	char	*sub_input;
	char	*rest_input;
	char	*temp_input;

	exit_str = ft_itoa(exit_status);
	temp_input = *input;
	i = find_exit_marker(*input);
	while (i != -1)
	{
		temp_input[i] = '\0';
		sub_input = ft_strjoin(temp_input, exit_str);
		if (temp_input[i + 1] == '?')
			rest_input = ft_strjoin(sub_input, temp_input + i + 2);
		else if (temp_input[i + 1] == '{' && temp_input[i + 2] == '?'
			&& temp_input[i + 3] == '}')
			rest_input = ft_strjoin(sub_input, temp_input + i + 4);
		free(sub_input);
		free(*input);
		*input = rest_input;
		temp_input = *input;
		i = find_exit_marker(rest_input);
	}
	free(exit_str);
}

static void	extract_var_info(char *pos, char **var_name, char **after_var)
{
	int	name_size;

	if (*(pos + 1) == '{')
	{
		name_size = varname_size(pos + 2);
		*var_name = ft_substr(pos + 2, 0, name_size);
		*after_var = ft_strdup(pos + 3 + name_size);
	}
	else
	{
		name_size = varname_size(pos + 1);
		*var_name = ft_substr(pos + 1, 0, name_size);
		*after_var = ft_strdup(pos + 1 + name_size);
	}
}

static void	expand_single_variable(char **input, char **pos, t_env *minienv)
{
	char	*var_name;
	char	*var_value;
	char	*after_var;

	extract_var_info(*pos, &var_name, &after_var);
	var_value = minienv_value(var_name, minienv);
	free(var_name);
	if (*pos == *input)
	{
		var_at_start(input, var_value, after_var);
	}
	else
	{
		var_not_at_start(input, var_value, after_var, *pos);
	}
	free(after_var);
}

void	expand_variables(char **input, t_env *minienv)
{
	char	*pos;

	pos = find_var_pos(*input);
	while (pos)
	{
		expand_single_variable(input, &pos, minienv);
		pos = find_var_pos(*input);
	}
}

void	handle_expansions(char **input, t_env *minienv, int exit_status)
{
	expand_exit_status(input, exit_status);
	expand_variables(input, minienv);
}
