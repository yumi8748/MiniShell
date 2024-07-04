/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minienv_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoniechen <leoniechen@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:17:53 by leochen           #+#    #+#             */
/*   Updated: 2024/07/01 23:16:02 by leoniechen       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	minienv_size(t_env *minienv)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = minienv;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**from_minienv_to_env(t_env *minienv)
{
	char	**envp;
	t_env	*tmp;
	int		i;

	i = 0;
	envp = (char **)malloc(sizeof(char *) * (minienv_size(minienv) + 1));
	if (!envp)
		return (NULL);
	tmp = minienv;
	while (tmp)
	{
		envp[i] = ft_strdup(tmp->key_pair);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

char	*get_path_from_env(char *cmd, t_env *minienv)
{
	char	*path_line;
	char	**splited_paths;
	char	*path;

	path_line = minienv_value("PATH", minienv);
	if (!path_line)
		return (NULL);
	splited_paths = ft_split(path_line, ':');
	if (!splited_paths)
		return (NULL);
	path = find_executable_path(cmd, splited_paths);
	free_str_array(splited_paths);
	return (path);
}

int	str_equal(const char *str1, const char *str2)
{
	size_t	size;

	if (!str1 || !str2)
		return (0);
	size = ft_strlen(str1);
	if (size != ft_strlen(str2))
		return (0);
	return (ft_strncmp(str1, str2, size) == 0);
}
