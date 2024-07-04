/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:34:47 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 13:57:52 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	varname_size(char *s)
{
	int	i;

	i = 0;
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (1);
	while (s[i])
	{
		if (!is_valid_varchar(s[i]))
			return (i);
		i++;
	}
	return (i);
}

static int	check_expansion_format(char *s)
{
	int	i;

	i = 0;
	if (!s || !*s)
		return (FALSE);
	if (s[i] == '$')
	{
		if (s[i + 1] == '{')
		{
			i += 2;
			while (s[i] && s[i] != '}')
			{
				if (!is_valid_varchar(s[i]))
					return (FALSE);
				i++;
			}
			if (s[i] == '}')
				return (TRUE);
		}
		else if (is_valid_varchar(s[i + 1]))
			return (TRUE);
		return (FALSE);
	}
	return (FALSE);
}

// char	*find_var_pos(char *input)
// {
// 	int	i;

// 	i = 0;
// 	while (input[i])
// 	{
// 		if (input[i] == '\'')
// 			i = skip_quotes(input, i, '\'');
// 		else if (input[i] == '\"')
// 		{
// 			i++;
// 			while (input[i] && input[i] != '\"')
// 			{
// 				if (input[i] && check_expansion_format(input + i) == TRUE)
// 					return (input + i);
// 				i++;
// 			}
// 			if (input[i])
// 				i++;
// 		}
// 		if (input[i] && check_expansion_format(input + i) == TRUE)
// 			return (input + i);
// 		i++;
// 	}
// 	return (NULL);
// }



static int find_in_double_quotes(char *input, int index)
{
    int result = index + 1; 
    while (input[result] && input[result] != '\"')
	{
        if (check_expansion_format(input + result))
		{
            return (result);
        }
        result++;
    }
    return (result); 
}

static int skip_single_quotes(char *input, int index) {
    index++;
    while (input[index] && input[index] != '\'')
	{
        index++;
    }
    if (input[index])
	{
        index++; 
    }
    return (index);
}

char *find_var_pos(char *input)
{
    int i;
    int result;

	i = 0;
    while (input[i]) {
        if (input[i] == '\'') 
            i = skip_single_quotes(input, i);
        else if (input[i] == '\"')
		{
            result = find_in_double_quotes(input, i);
            if (input[result] != '\"' && input[result] != '\0') 
                return (input + result);
            i = result;
        } else
		{
            if (check_expansion_format(input + i))
                return (input + i);
        }
        if (!input[i])
            break;
        i++;
    }
    return (NULL);
}
void	var_at_start(char **input, char *var_value, char *after_var)
{
	char	*updated_input;

	if (!var_value)
		updated_input = ft_strdup(after_var);
	else
		updated_input = ft_strjoin(var_value, after_var);
	free(*input);
	*input = updated_input;
}

void	var_not_at_start(char **input, char *var_value, char *after_var,
		char *pos)
{
	char	*updated_input;
	char	*until_var;
	char	*tmp;

	until_var = ft_substr(*input, 0, pos - *input);
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
