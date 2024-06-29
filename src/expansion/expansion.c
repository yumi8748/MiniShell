/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:59:36 by leochen           #+#    #+#             */
/*   Updated: 2024/06/28 12:53:29 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../inc/minishell.h"


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
		if (temp_input[i + 1] == '?')
			rest_input = ft_strjoin(sub_input, temp_input + i + 2);
		else if (temp_input[i + 1] == '{' && temp_input[i + 2] == '?' && temp_input[i + 3] == '}') // ${?}
			rest_input = ft_strjoin(sub_input, temp_input + i + 4);
		free(sub_input); // 释放临时分配的字符串
        free(*input);    // 释放旧的 `input`
		*input = rest_input; // 更新 `input` 指向新的字符串
		temp_input = *input;
		i = find_exit_marker(rest_input);
	}
	free(exit_str);
	//printf("expanded exit str:%s\n", *input);
}




void expand_variables(char **input, t_env *minienv)
{
    char *pos;
	char *var_name;
	char *var_value;
	char *after_var;
	int name_size;

	pos = find_var_pos(*input);
    while (pos)
	{
        if (*(pos + 1) == '{')
		{
            name_size = varname_size(pos + 2);
            var_name = ft_substr(pos + 2, 0, name_size);
            after_var = ft_strdup(pos + 3 + name_size);  // Assuming '}' is immediately after the name
        }
		else
		{
            name_size = varname_size(pos + 1);
            var_name = ft_substr(pos + 1, 0, name_size);
            after_var = ft_strdup(pos + 1 + name_size);
        }
        var_value = minienv_value(var_name, minienv);
        free(var_name);
        if (pos == *input)
            var_at_start(input, var_value, after_var);
        else 
            var_not_at_start(input, var_value, after_var, pos);
        free(after_var);
        pos = find_var_pos(*input); // Re-find position as the input has changed
    }
}

void	handle_expansions(char **input, t_env *minienv, int exit_status)
{
	expand_exit_status(input, exit_status);
	expand_variables(input, minienv);
}