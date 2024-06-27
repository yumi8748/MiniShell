/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:19:36 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/26 19:10:35 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// static int  export_without_arg(t_env *minienv)
// {
// 	t_env	*cur;

// 	cur = minienv;
// 	while (cur != NULL)
// 	{
// 		ft_putstr_fd("declare -x ", STDOUT_FILENO);
// 		ft_putstr_fd(name_only(cur->key_pair), STDOUT_FILENO);
// 		if (ft_strchr(cur->key_pair, '='))
// 		{
// 			ft_putstr_fd("=", STDOUT_FILENO);
// 			ft_putstr_fd("\"", STDOUT_FILENO);
// 			ft_putstr_fd(get_value(cur->key_pair), STDOUT_FILENO);
// 			ft_putstr_fd("\"", STDOUT_FILENO);
// 		}
// 		ft_putstr_fd("\n", STDOUT_FILENO);
// 		cur = cur->next;
// 	}
// 	return (0);
// }

// int	builtin_export(char **args, t_env **minienv)
// {
// 	char	*key_pair;
// 	char	*varname;
// 	int		exit_status;

// 	args++;
// 	exit_status = EXIT_SUCCESS;
// 	if (!*args)
// 		return (declare_env(*minienv));
// 	while (*args)
// 	{
// 		key_pair = *args;
// 		varname = name_only(key_pair);
// 		if (!is_valid_varname(varname) || ft_strcmp(key_pair, "="))
// 		{
// 			print_varname_error_msg("export", key_pair);
// 			exit_status = EXIT_FAILURE;
// 		}
// 		else if (find_node(varname, *minienv))
// 			minienv_update(varname, get_value(key_pair), *minienv);
// 		else
// 			list_append(key_pair, minienv);
// 		free(varname);
// 		args++;
// 	}
// 	return (exit_status);
// }

