/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:29:24 by leochen           #+#    #+#             */
/*   Updated: 2024/07/01 18:14:05 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_folder(char *cmd)
{
	struct stat	buf;

	if (stat(cmd, &buf) == 0)
	{
		if (S_ISDIR(buf.st_mode))
		{
			if (*cmd == '.')
				cmd++;
			if (*cmd == '.')
				cmd++;
			if (*cmd == '/')
				return (1);
		}
	}
	return (0);
}

void	command_exit(char **args, t_env *minienv, int exit_status)
{
	if (exit_status == 126)
		print_error_msg(args[0], "Is a directory");
	if (exit_status == 127)
		print_error_msg(args[0], "command not found");
	rl_clear_history();
	free_minienv(&minienv);
	free_str_array(args);
	close_all_fds();
	exit(exit_status);
}

char	*get_path(char *cmd, t_env *minienv)
{
	if (is_path(cmd))
		return (get_real_path(cmd, minienv));
	else
		return (get_path_from_env(cmd, minienv));
}

int	is_path(char *cmd)
{
	if (ft_strncmp(cmd, "./", 2) == 0 || ft_strncmp(cmd, "../", 3) == 0
		|| *cmd == '/')
		return (1);
	return (0);
}
