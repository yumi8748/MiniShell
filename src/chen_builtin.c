/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chen_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:16:11 by leochen           #+#    #+#             */
/*   Updated: 2024/06/07 12:56:50 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// //echo
// int	echo(char **args)
// {
// 	int	i;
// 	int	new_line;

// 	new_line = 1;
// 	i = 1;
// 	if (args[1] && ft_strncmp(args[1], "-n", 3) == 0)
// 	{
// 		new_line = 0;
// 		i++;
// 	}
// 	while (args[i])
// 	{
// 		ft_putstr_fd(args[i], STDOUT_FILENO);
// 		if (args[i + 1])
// 			ft_putstr_fd(" ", STDOUT_FILENO);
// 		i++;
// 	}
// 	if (new_line)
// 		ft_putstr_fd("\n", STDOUT_FILENO);
// 	return (0);
// }


// //cd 
// int	cd_error(void)
// {
// 	print_error_msg("cd", "too many arguments");
// 	return (EXIT_FAILURE);
// }

// int	cd(char **args, t_env *minienv)
// {
// 	char	*path;
// 	char	*pwd;
// 	char	*oldpwd;
// 	char	cwd[PATH_MAX];

// 	if (args[1] && args[2])
// 		return (cd_error());
// 	if (args[1] && !str_equal(args[1], "~"))
// 		path = args[1];
// 	else
// 		path = minienv_value("__HOME", minienv);
// 	if (chdir(path) != 0)
// 	{
// 		print_perror_msg("cd", args[1]);
// 		return (EXIT_FAILURE);
// 	}
// 	pwd = minienv_value("PWD", minienv);
// 	oldpwd = minienv_value("OLDPWD", minienv);
// 	if (oldpwd && pwd && *pwd)
// 		minienv_update("OLDPWD", pwd, minienv);
// 	if (pwd && *pwd)
// 		minienv_update("PWD", getcwd(cwd, PATH_MAX), minienv);
// 	return (EXIT_SUCCESS);
// }


// //pwd
// int	pwd(void)
// {
// 	char	cwd[PATH_MAX];

// 	getcwd(cwd, PATH_MAX);
// 	ft_putstr_fd(cwd, 1);
// 	ft_putstr_fd("\n", 1);
// 	return (0);
// }

// //exit  prompt里面有写
// int	fits_in_long_long(char *str)
// {
// 	long long	out;
// 	int			c;

// 	if (ft_strlen(str) > 20)
// 		return (0);
// 	if (ft_strncmp(str, "-9223372036854775808", 21) == 0)
// 		return (1);
// 	out = 0;
// 	if (*str == '-' || *str == '+')
// 		str++;
// 	while (*str)
// 	{
// 		if (*str < '0' || *str > '9')
// 			return (0);
// 		c = *str - '0';
// 		if (out > (LLONG_MAX - c) / 10)
// 			return (0);
// 		out = out * 10 + c;
// 		str++;
// 	}
// 	return (1);
// }

// static void	check_args_error(char **args)
// {
// 	char	*exit_status;

// 	if (!args || !args[1])
// 	{
// 		if (args)
// 			free_array(args);
// 		close_all_fds();
// 		exit(EXIT_SUCCESS);
// 	}
// 	exit_status = args[1];
// 	if (!fits_in_long_long(exit_status))
// 	{
// 		free_array(args);
// 		exit_with_error("exit", "numeric argument required", BUILTIN_MISUSE);
// 	}
// 	if (args[2] != NULL)
// 	{
// 		free_array(args);
// 		exit_with_error("exit", "too many arguments", EXIT_FAILURE);
// 	}
// }

// int	builtin_exit(char **args, t_env **minienv)
// {
// 	int	exit_status;

// 	rl_clear_history();
// 	free_minienv(minienv);
// 	ft_putstr_fd("exit\n", STDOUT_FILENO);
// 	check_args_error(args);
// 	close_all_fds();
// 	exit_status = ft_atoll(args[1]);
// 	free_array(args);
// 	exit(exit_status);
// }