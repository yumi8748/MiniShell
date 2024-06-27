/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:28:42 by leochen           #+#    #+#             */
/*   Updated: 2024/06/27 18:29:43 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char *get_real_path(char *cmd, void *minienv)
{
    char path[1024];
    char tmp[1024];
	char *last_slash;

    if (strncmp(cmd, "./", 2) == 0)
	{
        ft_strlcpy(path, minienv_value("PWD", minienv), sizeof(path));
        ft_strlcat(path, cmd + 1, sizeof(path));  // Append cmd without the initial dot
    }
	else if (strncmp(cmd, "../", 3) == 0)
	{
        ft_strlcpy(tmp, minienv_value("PWD", minienv), sizeof(tmp));
       last_slash = strrchr(tmp, '/');
        if (last_slash)
		{
            *last_slash = '\0';  // Remove the last part of the path
            ft_strlcpy(path, tmp, sizeof(path));
            ft_strlcat(path, cmd + 2, sizeof(path));  // Append cmd without the initial two dots
        }
    }
	else if (*cmd == '/')
        ft_strlcpy(path, cmd, sizeof(path));
	else
        return NULL;
    return (strdup(path));  // Duplicate the path to return
}



char *find_executable_path(char *cmd, char **splited_paths)
{
	char *path;
	int i;
	char *tmp;

	i = 0;
	while (splited_paths[i] != NULL)
	{
		tmp = ft_strjoin(splited_paths[i], "/");
		if (!tmp)
		{
			free_str_array(splited_paths);
			return (NULL);
		}
		path = ft_strjoin(tmp, cmd);
        free_str(tmp);
		if (!path)
		{
			free_str_array(splited_paths);
			return (NULL);
		}
        if (access(path, F_OK) == 0)  //若存在 返回0
		{	
			//fprintf(stderr, "Found executable at: %s\n", path);
			return (path);
		}
		//fprintf(stderr, "Executable not found at: %s\n", path); // Log unsuccessful check
		free_str(path);
		i++;
	}
	return (NULL);
}
