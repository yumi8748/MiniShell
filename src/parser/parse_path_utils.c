/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:29:24 by leochen           #+#    #+#             */
/*   Updated: 2024/06/29 15:00:35 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
		print_error_msg(args[0], "Is a directory");
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
