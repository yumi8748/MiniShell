/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:39:28 by yu-chen           #+#    #+#             */
/*   Updated: 2024/05/28 16:46:19 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int ac, char **av, char **env)
{
	if (ac > 1 && av)
	{
		ft_putstr_fd("Minishell can't take arguments!\n", STDOUT_FILENO);
		return (EXIT_FAILURE);
	}
	return (minishell(init_minienv(env)));
}
