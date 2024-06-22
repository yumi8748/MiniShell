/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expansions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:34:47 by leochen           #+#    #+#             */
/*   Updated: 2024/06/17 14:25:38 by leochen          ###   ########.fr       */
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
        return (1); 
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
			return (input + i + 1);
		i++;
	}
	return (NULL);
}

void var_at_start(char **input, char *var_value, char *after_var)
{
	char	*updated_input;

	if (!var_value)
		updated_input = ft_strdup(after_var);
	else
		updated_input = ft_strjoin(var_value, after_var);
	free(*input);
	*input = updated_input;
}

void	var_not_at_start(char **input, char *var_value, char *after_var, char *pos)
{
	char *updated_input;
	char *until_var;
	char *tmp;

    until_var = ft_substr(*input, 0, pos - *input - 1);
	if (!var_value)
		updated_input = ft_strjoin(until_var, after_var);
	else
	{
		tmp = ft_strjoin(until_var, var_value);
		updated_input = ft_strjoin(tmp, after_var);
		free(tmp);
	}
	free(until_var);
	free(*input);
	*input = updated_input;	
}
void	update_input(char **input, char *var_value, char *after_var)
{
	char	*pos;

	pos = find_var_pos(*input);
	if (pos == *input + 1) //Variable is at the start of the string
		var_at_start(input, var_value, after_var);
	else
		var_not_at_start(input, var_value, after_var, pos);
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