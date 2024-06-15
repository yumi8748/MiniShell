/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chen_one_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 16:12:15 by leochen           #+#    #+#             */
/*   Updated: 2024/06/15 12:43:49 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	execute_normal_cmd(char **args, t_env *minienv)
{
	char	*cmd;
	int	pid;
	char *path;
	char **envp;

	cmd = args[0];
	pid = fork();
	define_execute_signals(pid);
	if (pid == 0)
	{
		if (is_empty(cmd))
			command_exit(args, minienv, 0);
		if (is_folder(cmd))
			command_exit(args, minienv, 126);
		path = get_path(cmd, minienv);
		if (!path && minienv_value("PATH", minienv) != NULL)
			command_exit(args, minienv, 127);
		else if (!path)
			path = ft_strdup(cmd);
		envp = from_minienv_to_env(minienv);
		rl_clear_history();
		close_extra_fds();
		free_minienv(&minienv);
		if (execve(path, args, envp) == -1)
			exec_error(args, path, envp);
	}
	else if (pid > 0)
		return (wait_for_child(pid, 1));
	else
		print_perror_msg("fork", "");
	exit(1);
}

void exec_error(char **args, char *path, char **envp)
{
	int exit_code;

	print_perror_msg("execve", args[0]);
	if (access(path, F_OK) == -1)
		 exit_code = 127;
	else if (access(path, X_OK) == -1)
		exit_code = 126;
	else
		exit_code = 1;
	free(path);
	free_str_array(envp);
	free_str_array(args);
	exit(exit_code);
}
int  minienv_size(t_env *minienv)
{
	int i;
	t_env *tmp;

	i = 0;
	tmp = minienv;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}


char **from_minienv_to_env(t_env *minienv)
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


/*在C语言中，`stat` 是一个系统调用，用于获取文件或目录的属性。它接受两个参数：一个是文件或目录的路径，
另一个是一个 `struct stat` 类型的指针，用于存储获取到的属性信息。
`struct stat` 结构体包含了文件或目录的各种属性，如大小、创建时间、修改时间等。
其中，`st_mode` 成员用于存储文件的类型和权限信息。
`S_ISDIR` 是一个宏，用于检查 `st_mode` 是否表示一个目录。如果 `st_mode` 表示一个目录，
`S_ISDIR` 就会返回非零值；否则，它会返回零。
在你的 `is_folder` 函数中，首先调用 `stat` 获取 `cmd` 对应的文件或目录的属性，然后使用 `S_ISDIR` 检查 `st_mode` 
是否表示一个目录。如果 `cmd` 是一个目录，函数就会返回1；否则，函数就会返回0。*/
int	is_folder(char *cmd)
{
	struct stat	buf;

	if (stat(cmd, &buf) == 0) // stat() returns 0 on success, or -1 if an error occurs
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
		print_error_msg(args[0], "is a directory");
	if (exit_status == 127)
		print_error_msg(args[0], "command not found");
	rl_clear_history();
	free_minienv(&minienv);
	free_str_array(args);
	close_all_fds();
	exit(exit_status);
}

char *get_path(char *cmd, t_env *minienv)
{
	if (is_path(cmd))
		return (get_real_path(cmd, minienv));
	else
		return (get_path_from_env(cmd, minienv));	
}

int	is_path(char *cmd)
{
	if (ft_strncmp(cmd, "./", 2) == 0 || ft_strncmp(cmd, "../", 3) == 0 || *cmd == '/')
		return (1);
	return (0);
}


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


char *get_path_from_env(char *cmd, t_env *minienv)
{
	char *path_line;
	char **splited_paths;
	char *path;
	int i;	

	i = 0;
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
			   fprintf(stderr, "Failed to allocate memory for tmp\n");
			free_str_array(splited_paths);
			return (NULL);
		}
		path = ft_strjoin(tmp, cmd);
        free_str(tmp);
		if (!path)
		{
			fprintf(stderr, "Failed to allocate memory for path\n");
			free_str_array(splited_paths);
			return (NULL);
		}
        if (access(path, F_OK) == 0)  //若存在 返回0
		{	
			fprintf(stderr, "Found executable at: %s\n", path);
			return (path);
		}
		//fprintf(stderr, "Executable not found at: %s\n", path); // Log unsuccessful check
		free_str(path);
		i++;
	}
	return (NULL);
}