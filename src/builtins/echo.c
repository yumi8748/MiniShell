/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:03:44 by leochen           #+#    #+#             */
/*   Updated: 2024/06/29 14:16:04 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// echo
int	is_minus_n(char *s)
{
	if (s[0] != '-')
		return (0);
	s++;
	while (*s)
	{
		if (*s != 'n')
			return (0);
		s++;
	}
	return (1);
}

int	echo(char **args)
{
	int	i;
	int	print_new_line;

	print_new_line = 1;
	i = 1;
	while (args[i] && is_minus_n(args[i])) // Skip all initial '-n' options
	{
		print_new_line = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (print_new_line == 1)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
