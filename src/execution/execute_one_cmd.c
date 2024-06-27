/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chen_one_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:02:31 by leochen           #+#    #+#             */
/*   Updated: 2024/06/27 17:22:10 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"




int execute_one_cmd(char *cmd, t_env **minienv)
{
    char **args;
    int exit_status;
    int original_fds[2];

    if (handle_redirects(cmd, &original_fds[0]) == 0)
    {
        if (original_fds[0] != -1)
        {
            dup2(original_fds[0], STDIN_FILENO);
            close(original_fds[0]);
        }
        if (original_fds[1] != -1)
        {
            dup2(original_fds[1], STDOUT_FILENO);
            close(original_fds[1]);
        }
        free(cmd);
        return (EXIT_FAILURE);
    }
    //printf("command before split args:%s\n", cmd);
	args = split_one_arg(cmd);
	//printf("args[0] after split:%s\n", args[0]);
    free(cmd);
    if (is_builtin(args[0]))
        exit_status = execute_builtin(args, minienv);
    else
        exit_status = execute_normal_cmd(args, *minienv);
    free_str_array(args);
    if (original_fds[0] != -1)
    {
        dup2(original_fds[0], STDIN_FILENO);
        close(original_fds[0]);
    }
    if (original_fds[1] != -1)
    {
        dup2(original_fds[1], STDOUT_FILENO);
        close(original_fds[1]);
    }
    return (exit_status);
}

/*get path里面有个126的情况 他和在一起了 要分开成为其中一个是127*/

int is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0')
		return (1);
	if (ft_strncmp(cmd, "cd", 2) == 0 && cmd[2] == '\0')
		return (1);
	if (ft_strncmp(cmd, "pwd", 3) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "export", 6) == 0 && cmd[6] == '\0')
		return (1);
	if (ft_strncmp(cmd, "unset", 5) == 0 && cmd[5] == '\0')
		return (1);
	if (ft_strncmp(cmd, "env", 3) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "exit", 4) == 0 && cmd[4] == '\0')
		return (1);	
	return (0);
}

int	execute_builtin(char **args, t_env **minienv)
{
	char	*cmd;
	int		exit_status;

	cmd = args[0];
	if (ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0')
		exit_status = echo(args);
	else if (ft_strncmp(cmd, "pwd", 3) == 0 && cmd[3] == '\0')
		exit_status = ft_pwd();
	else if (ft_strncmp(cmd, "env", 3) == 0 && cmd[3] == '\0')
		exit_status = env(*minienv);
	else if (ft_strncmp(cmd, "export", 6) == 0 && cmd[6] == '\0') 
		exit_status = builtin_export(args, minienv);
	else if (ft_strncmp(cmd, "unset", 5) == 0 && cmd[5] == '\0')
		exit_status = unset(args, minienv);
	else if (ft_strncmp(cmd, "cd", 2) == 0 && cmd[2] == '\0')
		exit_status = cd(args, *minienv);
	else if (ft_strncmp(cmd, "exit", 4) == 0 && cmd[4] == '\0')
		exit_status = shell_exit(args, minienv);
	else
		exit_status = EXIT_FAILURE;
	return (exit_status);
}



