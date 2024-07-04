/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:03:44 by leochen           #+#    #+#             */
/*   Updated: 2024/07/01 17:50:26 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	while (args[i] && is_minus_n(args[i]))
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
