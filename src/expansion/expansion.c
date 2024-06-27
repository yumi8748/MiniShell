/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:59:36 by leochen           #+#    #+#             */
/*   Updated: 2024/06/27 18:04:21 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../inc/minishell.h"

static  int find_exit_marker(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'')
			i = skip_quotes(s, i, '\'');
		else if (s[i] == '\"')
		{
			i++;
			while (s[i] && s[i] != '\"')
			{
				if (s[i] == '$' && s[i + 1] == '?')
					return (i);
				i++;
			}
			if (s[i])
				i++;
		}
		if(s[i] == '$' && s[i+1] == '?')
			return (i);
		i++;
	}
	return (-1);
}


void	expand_exit_status(char **input, int exit_status)
{
	int i;
	char	*exit_str;
	char *sub_input;
	char	*rest_input;
	char *temp_input;

 	exit_str = ft_itoa(exit_status);
	temp_input = *input;
	i = find_exit_marker(*input);
	while (i != -1)
	{
		temp_input[i] = '\0';  // 将 `$` 替换为字符串结束符，以分割 `input`
		sub_input = ft_strjoin(temp_input, exit_str);
		rest_input = ft_strjoin(sub_input, temp_input + i + 2);
		free(sub_input); // 释放临时分配的字符串
        free(*input);    // 释放旧的 `input`
		*input = rest_input; // 更新 `input` 指向新的字符串
		temp_input = *input;
		i = find_exit_marker(rest_input);
	}
	free(exit_str);
	//printf("expanded exit str:%s\n", *input);
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