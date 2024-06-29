/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:54:12 by leochen           #+#    #+#             */
/*   Updated: 2024/06/29 15:02:29 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	close_extra_fds(void)
{
	int	last_open_fd;

	last_open_fd = open("/tmp/last_fd", O_RDWR | O_CREAT, 0666);
	if (last_open_fd == -1)
		print_perror_msg("open", "/tmp/last_fd");
	while (last_open_fd > STDERR_FILENO)
	{
		close(last_open_fd);
		last_open_fd--;
	}
}

void	close_all_fds(void)
{
	close_extra_fds();
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	clean_exit(char **splited_cmds, t_env **minienv)
{
	rl_clear_history();
	free_str_array(splited_cmds);
	free_minienv(minienv);
	close_all_fds();
	exit(EXIT_FAILURE);
}
