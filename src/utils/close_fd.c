/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:54:12 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 13:55:20 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	close_extra_fds(void)
{
	int	last_open_fd;

	int i;

	i = 3;
	last_open_fd = open("/tmp/last_fd", O_RDWR | O_CREAT, 0666);
	if (last_open_fd == -1)
		print_perror_msg("open", "/tmp/last_fd");
	while (last_open_fd > STDERR_FILENO)
	{
		close(last_open_fd);
		last_open_fd--;
	}
		while (i < 1024)
	{
		close(i);
		i++;
	}
}

void	close_all_fds(void)
{
	int	i;

	close_extra_fds();
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	i = 0;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}

void	clean_exit(char **splited_cmds, t_env **minienv)
{
	rl_clear_history();
	free_str_array(splited_cmds);
	free_minienv(minienv);
	close_all_fds();
	exit(EXIT_FAILURE);
}
