/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expansions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:34:47 by leochen           #+#    #+#             */
/*   Updated: 2024/06/15 18:42:42 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

 int	is_valid_varchar(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

int	varname_size(char *s)
{
    int	i;

	i = 0;
	if (!ft_isalpha(s[0]) && s[0] != '_')   //variable name must start with a letter or an underscore
        return (0); 
    while (s[i])
	{
        if (!is_valid_varchar(s[i]))
            return (i);  //variable name must contain only letters, numbers, and underscores
        i++;
    }
    return (i);  
}

char	*find_var_pos(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
			i = skip_quotes(input, i, '\'');
		else if (input[i] == '\"')
			i = skip_quotes(input, i, '\"');
        if (input[i] == '$' && is_valid_varchar(input[i + 1]))
			return (input + 1);
		i++;
	}
	return (NULL);
}

void	update_input(char **input, char *var_value, char *after_var)
{
	char	*until_var;   //substring before the variable
	char	*updated_input;

	if (!*input[0] && !var_value) //if the input string is empty and the variable value is empty, then do nothing
		until_var = ft_strdup("");
	else if (!*input[0] && var_value) //if the input string is empty and the variable value is not empty, then add the variable value
		until_var = ft_strdup(var_value);
	else if (!var_value) //if the variable value is empty and the input string is not empty, then add the input string
		until_var = ft_strdup(*input);
	else  //if the variable value is not empty and the input string is not empty, then add the input
		until_var = ft_strjoin(*input, var_value);
	updated_input = ft_strjoin(until_var, after_var);
	free(until_var);
	free(*input);
	*input = updated_input;
}

void	expand_variables(char **input, t_env *minienv)
{
	char	*pos;
	char	*var_name;
	char	*var_value;
	int		name_size;

	pos = find_var_pos(*input);
	while (pos)
	{
		name_size = varname_size(pos);
		var_name = ft_substr(pos, 0, name_size);
		*(pos - 1) = '\0';
		var_value = minienv_value(var_name, minienv);
		update_input(input, var_value, (pos + name_size));
		free(var_name);
		pos = find_var_pos(*input);
	}
}

void	handle_expansions(char **input, t_env *minienv, int exit_status)
{
	expand_exit_status(input, exit_status);
	expand_variables(input, minienv);
}