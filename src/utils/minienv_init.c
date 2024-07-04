/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minienv_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoniechen <leoniechen@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:48:41 by yu-chen           #+#    #+#             */
/*   Updated: 2024/07/01 22:47:31 by leoniechen       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_env	*init_minienv(char **env)
{
	t_env	*list;
	char	*home;
	int		i;

	list = NULL;
	i = 0;
	while (env[i])
	{
		list_append(env[i], &list);
		i++;
	}
	if (!find_node("OLDPWD", list))
		list_append("OLDPWD", &list);
	home = ft_strjoin("__HOME=", minienv_value("HOME", list));
	list_append(home, &list);
	free(home);
	return (list);
}

void	list_append(char *key_pair, t_env **list)
{
	struct s_env	*new_node;
	struct s_env	*aux_node;

	new_node = malloc(sizeof(struct s_env));
	new_node->key_pair = ft_strdup(key_pair);
	new_node->next = NULL;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	aux_node = *list;
	while (aux_node->next)
		aux_node = aux_node->next;
	aux_node->next = new_node;
}

void	minienv_update(char *key, char *value, t_env *minienv)
{
	t_env	*i;
	char	*new_keypair;

	i = find_node(key, minienv);
	new_keypair = create_keypair(key, value);
	if (!new_keypair)
		return ;
	if (!i)
	{
		if (new_keypair)
		{
			list_append(new_keypair, &minienv);
			free(new_keypair);
		}
	}
	else
	{
		free(i->key_pair);
		i->key_pair = new_keypair;
	}
}
