/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:19:38 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/29 14:17:33 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	minienv_del_next_node(t_env **node)
{
	t_env	*tmp;

	tmp = (*node)->next;
	(*node)->next = (*node)->next->next;
	free(tmp->key_pair);
	free(tmp);
}

static void	remove_from_minienv(char *varname, t_env **minienv)
{
	t_env	*aux;

	aux = *minienv;
	if (aux && ft_strncmp(aux->key_pair, varname, ft_strlen(varname)) == 0
		&& aux->key_pair[ft_strlen(varname)] == '=')
	{
		*minienv = aux->next;
		free(aux->key_pair);
		free(aux);
		return ;
	}
	while (aux && aux->next)
	{
		if (ft_strncmp((aux->next)->key_pair, varname, ft_strlen(varname)) == 0
			&& (aux->next)->key_pair[ft_strlen(varname)] == '=')
		{
			return (minienv_del_next_node(&aux));
		}
		aux = aux->next;
	}
}

int	unset(char **argv, t_env **minienv)
{
	char	*varname;

	argv++;
	if (!argv)
		return (EXIT_SUCCESS);
	while (*argv)
	{
		varname = *argv;
		if (is_valid_varname(varname))
			remove_from_minienv(varname, minienv);
		argv++;
	}
	return (EXIT_SUCCESS);
}
