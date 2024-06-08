/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chen_expansion_exit.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:59:36 by leochen           #+#    #+#             */
/*   Updated: 2024/06/08 17:54:26 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../inc/minishell.h"

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
	printf("expanded exit str:%s\n", *input);
}