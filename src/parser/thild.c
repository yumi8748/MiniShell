/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thild.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 19:51:57 by yu-chen           #+#    #+#             */
/*   Updated: 2024/07/03 20:00:57 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

size_t	calculate_command_len(char *command, t_env *minienv)
{
	char	*tilde_pos;
	size_t	new_command_len;
	size_t	home_len;

	tilde_pos = command;
	new_command_len = 0;
	home_len = ft_strlen(minienv_value("__HOME", minienv));
	while (*tilde_pos)
	{
		if (*tilde_pos == '~')
			new_command_len += home_len;
		else
			new_command_len++;
		tilde_pos++;
	}
	return (new_command_len);
}

void	expand_tilde_path(char *new_cmd_ptr, char *cmd_ptr,
	char *expanded_path, t_env *minienv)
{
	size_t	home_len;
	int		escape_flag;

	home_len = ft_strlen(minienv_value("__HOME", minienv));
	escape_flag = 0;
	while (*cmd_ptr)
	{
		if (*cmd_ptr == '\\' && !escape_flag)
		{
			escape_flag = 1;
			cmd_ptr++;
		}
		else if (*cmd_ptr == '~' && !escape_flag && expanded_path != NULL)
		{
			ft_memcpy(new_cmd_ptr, expanded_path, home_len);
			new_cmd_ptr += home_len;
			cmd_ptr++;
		}
		else
		{
			*new_cmd_ptr++ = *cmd_ptr++;
			escape_flag = 0;
		}
	}
	*new_cmd_ptr = '\0';
}

char	*find_and_replace_tilde(char *command, t_env *minienv)
{
	char	*command_ptr;
	char	*new_command;
	char	*new_command_ptr;
	char	*expanded_path;
	size_t	new_command_len;

	new_command_len = calculate_command_len(command, minienv);
	new_command = malloc(new_command_len + 1);
	if (!new_command)
		return (command);
	new_command_ptr = new_command;
	command_ptr = command;
	expanded_path = minienv_value("__HOME", minienv);
	expand_tilde_path(new_command_ptr, command_ptr, expanded_path, minienv);
	return (new_command);
}
