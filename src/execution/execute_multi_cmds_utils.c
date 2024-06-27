/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multi_cmds2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:11:11 by leochen           #+#    #+#             */
/*   Updated: 2024/06/27 18:11:11 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int str_strlen(char **splited_cmds)
{
	int i;

	i = 0;
	while (splited_cmds[i] != NULL)
		i++;
	return (i);
}
	

int  *init_pid_array(char **splited_cmds)
{
	int len;
	int *pid_arry;
	
	len = str_strlen(splited_cmds);
	pid_arry = (int *)malloc(sizeof(int) * len);
	if (!pid_arry)
		return (NULL);
	ft_memset(pid_arry, 0, sizeof(int) * len);
	//printf("len = %d\n", len);
	return (pid_arry);
}

int wait_for_children(int *pid_array, char **splited_cmds)
{
	int i;
	int exit_status;
	int is_last_pid;

	i = 0;
	exit_status = 0;
	while (splited_cmds[i] != NULL)
	{
		if (i == str_strlen(splited_cmds) - 1)
			is_last_pid = 1;
		else
		    is_last_pid = 0;
		exit_status = wait_for_child(pid_array[i], is_last_pid);
		i++;
	}
	close_extra_fds();
	free(pid_array);
	return (exit_status);
}

