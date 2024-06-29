/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:38:06 by leochen           #+#    #+#             */
/*   Updated: 2024/06/29 15:02:24 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_str(char *s)
{
	if (s)
		free(s);
}

void	free_str_array(char **s) // changed name  void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (s != NULL)
	{
		while (s[i] != NULL)
		{
			free_str(s[i]);
			s[i] = NULL;
			i++;
		}
		free(s);
	}
}

void	clean_up_resources(t_env **minienv, char **args, int flag)
{
	if (flag == 1)
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	rl_clear_history();
	if (minienv)
		free_minienv(minienv);
	close_all_fds();
	if (args)
		free_str_array(args);
}

void	clean_up_exit(char **args, t_env **minienv, int exit_status, int flag)
{
	if (flag == 1)
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	rl_clear_history();
	if (minienv)
		free_minienv(minienv);
	close_all_fds();
	// check_args_error(args);
	if (args)
		free_str_array(args);
	exit(exit_status);
}

void	free_minienv(t_env **minienv)
{
	t_env	*aux;
	t_env	*next;

	aux = *minienv;
	while (aux)
	{
		free(aux->key_pair);
		next = aux->next;
		free(aux);
		aux = next;
	}
	minienv = NULL;
}
