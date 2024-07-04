/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoniechen <leoniechen@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:16:11 by leochen           #+#    #+#             */
/*   Updated: 2024/07/01 23:56:34 by leoniechen       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*cd_path(char **args, t_env *minienv)
{
	char	*path;

	if (args[1] == NULL || ft_strncmp(args[1], "~", 2) == 0
		|| ft_strncmp(args[1], "--", 3) == 0)
	{
		path = minienv_value("HOME", minienv);
		if (path == NULL)
		{
			print_error_msg("cd", "HOME not set");
			return (NULL);
		}
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = minienv_value("OLDPWD", minienv);
		if (path == NULL)
		{
			print_error_msg("cd", "OLDPWD not set");
			return (NULL);
		}
	}
	else
		path = args[1];
	return (path);
}

void	update_env(char *oldpwd, t_env *minienv)
{
	char	cwd[1024];

	if (getcwd(cwd, 1024) == NULL)
	{
		print_error_msg("cd", "getcwd failed");
		return ;
	}
	minienv_update("OLDPWD", oldpwd, minienv);
	minienv_update("PWD", cwd, minienv);
}

int	cd(char **args, t_env *minienv)
{
	char	*path;
	char	*pwd;
	char	*oldpwd;

	if (args[1] && args[2])
	{
		print_error_msg("cd", "too many arguments");
		return (EXIT_FAILURE);
	}
	path = cd_path(args, minienv);
	pwd = minienv_value("PWD", minienv);
	oldpwd = pwd;
	if (path == NULL)
		return (EXIT_FAILURE);
	if (chdir(path) == -1)
	{
		print_perror_msg("cd", path);
		return (EXIT_FAILURE);
	}
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
		ft_putendl_fd(path, STDOUT_FILENO);
	update_env(oldpwd, minienv);
	return (EXIT_SUCCESS);
}
