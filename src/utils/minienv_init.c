/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minienv_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:48:41 by yu-chen           #+#    #+#             */
/*   Updated: 2024/07/01 15:22:16 by yu-chen          ###   ########.fr       */
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
	t_env	*new_node;
	t_env	*current_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key_pair = ft_strdup(key_pair);
	if (!new_node->key_pair)
	{
		free(new_node); // Clean up if strdup fails
		return ;
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

void	minienv_update(char *key, char *value, t_env *minienv)
		//根据提供的key和value更新环境变量链表minienv
{
	t_env *i;
	char *new_keypair;

	i = find_node(key, minienv);              //找到key对应的node
	new_keypair = create_keypair(key, value); //根据key和value创建一个新的环境变量字符串
	if (!new_keypair)
		return ;
	if (!i) // 如果key对应的node不存在 则将新的环境变量字符串加入到环境变量链表minienv中
	{
		if (new_keypair)
		{
			list_append(new_keypair, &minienv); //把新的key_pair加入到minienv链表中
			free(new_keypair);                  //释放new_keypair
		}
	}
	else // 如果key对应的node存在 则更新node的value
	{
		free(i->key_pair);         //把原来的key_pair释放掉
		i->key_pair = new_keypair; //把新的key_pair赋值给node
	}
}
