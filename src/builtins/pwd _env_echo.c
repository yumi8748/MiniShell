/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd _env_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:16:11 by leochen           #+#    #+#             */
/*   Updated: 2024/06/26 17:05:08 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pwd(void)
{
	char	pwd[1024];

	if (getcwd(pwd, 1024) == NULL)
	{
        perror("getcwd failed");  // Properly handle and report the error if getcwd fails
        return (1);  // Return a non-zero value to indicate failure
    }
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	return (0);
}
int ft_env(t_env *minienv)
{
	t_env	*i;

	i = minienv;
	while (i)
	{
		if (ft_strchr(i->key_pair, '='))
			ft_putendl_fd(i->key_pair, STDOUT_FILENO);
		i = i->next;
	}
	return (0);
}

//echo
int is_minus_n(char *s)
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