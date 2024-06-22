/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minienv_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:37:20 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/17 18:22:16 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*value_only(char *key_pair)
{
	int	i;

	i = 0;
	while (key_pair[i] != '=' && key_pair[i])
		i++;
	if (!key_pair[i])
		return (NULL);
	return (&key_pair[i + 1]);
}

char	*minienv_value(char *key, t_env *minienv)
{
	t_env	*aux;

	aux = minienv_node(key, minienv);
	if (!aux)
		return (NULL);
	return (value_only(aux->key_pair));
}

t_env	*minienv_node(char *key, t_env *minienv)  //根据提供的key查找某一个环境变量（node）
{
	t_env	*aux;
	int		size;

	aux = minienv;
	size = ft_strlen(key);
	while (aux)
	{
		if (ft_strncmp(aux->key_pair, key, size) == 0)
		{
			if (aux->key_pair[size] == '=')
				return (aux);
		}
		aux = aux->next;
	}
	return (NULL);
}


static char	*create_keypair(char *key, char *value)  //根据提供的key和value创建一个新的环境变量字符串 但还没有加入到环境变量链表minienv中
{
	char	*key_pair;
	int		key_pair_size;
	int		i;

	key_pair_size = ft_strlen(key) + ft_strlen(value) + 2;
	key_pair = malloc(key_pair_size * sizeof(char));
	i = 0;
	while (*key)
		key_pair[i++] = *key++;
	key_pair[i++] = '=';
	while (*value)
		key_pair[i++] = *value++;
	key_pair[i] = '\0';
	return (key_pair);
}

void minienv_update(char *key, char *value, t_env *minienv) //根据提供的key和value更新环境变量链表minienv
{
    t_env *aux;
    char *new_keypair;
	
	aux = minienv_node(key, minienv); //找到key对应的node
	new_keypair = create_keypair(key, value);  //根据key和value创建一个新的环境变量字符串
	if (!new_keypair)
		return ;
    if (!aux) // 如果key对应的node不存在 则将新的环境变量字符串加入到环境变量链表minienv中
	{ 
        if (new_keypair)
            list_append(new_keypair, &minienv); //把新的key_pair加入到minienv链表中
    }
	else // 如果key对应的node存在 则更新node的value
	{ 
        free(aux->key_pair); //把原来的key_pair释放掉
        aux->key_pair = new_keypair; //把新的key_pair赋值给node
	}
}

