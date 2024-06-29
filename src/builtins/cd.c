/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:16:11 by leochen           #+#    #+#             */
/*   Updated: 2024/06/28 14:37:21 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//cd 
/*`chdir` 是一个 Unix 系统调用，用于改变当前进程的工作目录。它的原型如下：
int chdir(const char *path);
这个函数接受一个参数，即你想要切换到的目录的路径。如果目录切换成功，`chdir` 返回 0。如果目录切换失败（例如，目录不存在，或者你没有权限访问该目录），`chdir` 返回 -1，并设置 `errno` 为相应的错误代码。
在你提供的 `cd` 函数中，`chdir` 被用来切换到用户指定的目录（存储在 `path` 变量中）。如果目录切换失败，函数打印一个错误消息并返回 `EXIT_FAILURE`。*/

char *cd_path(char **args, t_env *minienv)
{
	char *path;

	if (args[1] == NULL || ft_strncmp(args[1], "~", 2) == 0 || ft_strncmp(args[1], "--", 3) == 0)  //如果是 cd 或者 cd ~ 或者 cd -- 则切换到 HOME 目录
	{
		path = minienv_value("HOME", minienv);
		if (path == NULL)
		{
			print_error_msg("cd", "HOME not set");
			return (NULL);
		}
	}
	else if (ft_strncmp(args[1], "-", 2) == 0) //如果是 cd - 则切换到上一个工作目录 
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

void  update_env(char *oldpwd, t_env *minienv)
{
	char cwd[1024];

	if (getcwd(cwd, 1024) == NULL) //getcwd函数用于获取当前工作目录，它的原型如下 int getcwd(char *buf, size_t size);  size是buf的大小
	{
		print_error_msg("cd", "getcwd failed");
		return ;
	}
	minienv_update("OLDPWD", oldpwd, minienv); //把之前保存的pwd工作目录存到OLDPWD
	minienv_update("PWD", cwd, minienv); //把用getcwd得到的目录存到PWD
}

int cd(char **args, t_env *minienv)
{
	char *path;
	char *pwd;
	char *oldpwd;

	if (args[1] && args[2])
	{  // Check if there is more than one argument provided.
        print_error_msg("cd", "too many arguments");
        return (EXIT_FAILURE);
    }
	path = cd_path(args, minienv);
	pwd = minienv_value("PWD", minienv);
	oldpwd = pwd;   // Save the current working directory.
	if (path == NULL)
		return (EXIT_FAILURE);
	if (chdir(path) == -1) // Change the current working directory.
	{
		print_perror_msg("cd", path);
		return (EXIT_FAILURE);
	}
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)  //单独处理这个是因为标准的cd -会打印路径
		ft_putendl_fd(path, STDOUT_FILENO);
	update_env(oldpwd, minienv);
	return (EXIT_SUCCESS);
}

