/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:16:11 by leochen           #+#    #+#             */
/*   Updated: 2024/06/29 14:16:32 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*skip_space(char **s)
{
	while (ft_isspace(**s) && **s)
		(*s)++;
	return (*s);
}

static int	is_llong_min(char *s, long long *nb)
{
	if (ft_strncmp(s, "-9223372036854775808", 21) == 0)
	{
		*nb = LLONG_MIN;
		return (1);
	}
	return (0);
}

static int	handle_sign(char **s)
{
	int	sign;

	sign = 1;
	if (**s == '-' || **s == '+')
	{
		if (**s == '-')
			sign = -1;
		(*s)++;
	}
	else
		sign = 1;
	return (sign);
}

int	str_to_ll(char *s, long long *nb)
{
	long long	res;
	int			sign;

	s = skip_space(&s);
	res = 0;
	if (!*s)
		return (0);
	sign = handle_sign(&s);
	if (is_llong_min(s, nb) == 1)
		return (1);
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (0);
		if (res > (LLONG_MAX - (*s - '0')) / 10) //优势：避免了乘法操作直接引起的溢出。
			return (0);
		res = res * 10 + (*s - '0');
		s++;
	}
	*nb = res * sign;
	return (1);
}

int	shell_exit(char **args, t_env **minienv)
{
	long long	exit_status;

	check_args_error(args);
	exit_status = 0;
	if (args && args[1])
	{
		str_to_ll(args[1], &exit_status);
		if (exit_status >= 0)
			exit_status = exit_status % 256;
		else
			exit_status = 256 + (exit_status % 256);
	}
	clean_up_resources(minienv, args, 1);
	exit((int)exit_status);
}
