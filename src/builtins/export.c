/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:19:36 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/29 14:16:41 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_varname_error_msg(char *command, char *varname)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": `", STDERR_FILENO);
	ft_putstr_fd(varname, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

static int	export_without_arg(t_env *minienv)
{
	t_env	*cur;

	cur = minienv;
	while (cur != NULL)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(get_key(cur->key_pair), STDOUT_FILENO);
		if (ft_strchr(cur->key_pair, '='))
		{
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
			ft_putstr_fd(get_value(cur->key_pair), STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
		cur = cur->next;
	}
	return (0);
}

int	is_valid_varname(char *s)
{
	int	i;

	if (!s || !s[0] || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	return (s[i] == '\0');
}

int	builtin_export(char **args, t_env **minienv)
{
	char	*key_pair;
	char	*varname;
	int		exit_status;

	args++;
	exit_status = EXIT_SUCCESS;
	if (!*args)
		return (export_without_arg(*minienv));
	while (*args)
	{
		key_pair = *args;
		varname = get_key(key_pair);
		if (!is_valid_varname(varname) || ft_strncmp(key_pair, "=", 1) == 0)
		{
			print_varname_error_msg("export", key_pair);
			exit_status = EXIT_FAILURE;
		}
		else if (find_node(varname, *minienv))
			minienv_update(varname, get_value(key_pair), *minienv);
		else
			list_append(key_pair, minienv);
		free(varname);
		args++;
	}
	return (exit_status);
}
