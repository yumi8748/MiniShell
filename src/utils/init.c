/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:48:41 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/26 17:59:17 by leochen          ###   ########.fr       */
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

void list_append(char *key_pair, t_env **list)
{
	t_env	*new_node;
	t_env	*current_node;

	new_node = malloc(sizeof(t_env));
	 if (!new_node)
        return;
	new_node->key_pair = ft_strdup(key_pair);
	 if (!new_node->key_pair)
	 {
        free(new_node); // Clean up if strdup fails
        return;
    }
	new_node->next = NULL;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	current_node = *list;
	while (current_node->next)
		current_node = current_node->next;
	current_node->next = new_node;
}
