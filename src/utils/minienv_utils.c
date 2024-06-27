/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minienv_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:37:20 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/27 13:20:15 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_value(char *keypair)
{
	int	i;

	i = 0;
	while (keypair[i] && keypair[i] != '=')
		i++;
	if (!keypair[i])
		return (NULL);
	return (&keypair[i + 1]);
}

char *get_key(char *keypair)
{
	int	i;

	i = 0;
	while (keypair[i] && keypair[i] != '=')
		i++;
	if (!keypair[i])
		return (NULL);
	return (ft_substr(keypair, 0, i));
}


char	*minienv_value(char *key, t_env *minienv)
{
	t_env	*i;

	i = find_node(key, minienv);
	if (!i)
		return (NULL);
	return (get_value(i->key_pair));
}

t_env	*find_node(char *key, t_env *minienv)  //根据提供的key查找某一个环境变量（node）
{
	t_env	*i;
	int		size;

	i = minienv;
	size = ft_strlen(key);
	while (i)
	{
		if (ft_strncmp(i->key_pair, key, size) == 0)
		{
			if (i->key_pair[size] == '=')
				return (i);
		}
		i = i->next;
	}
	return (NULL);
}


char *create_keypair(char *key,  char *value)
{
    char *keypair;
    size_t total_len;
	
	total_len = ft_strlen(key) + ft_strlen(value) + 2; // '=' and '\0'
    keypair = malloc(total_len);
    if (!keypair)
        return NULL;
    ft_strlcpy(keypair, key, total_len);
    ft_strlcat(keypair, "=", total_len);
    ft_strlcat(keypair, value, total_len);
    return (keypair);
}

void minienv_update(char *key, char *value, t_env *minienv) //根据提供的key和value更新环境变量链表minienv
{
    t_env *i;
    char *new_keypair;
	
	i = find_node(key, minienv); //找到key对应的node
	new_keypair = create_keypair(key, value);  //根据key和value创建一个新的环境变量字符串
	if (!new_keypair)
		return ;
    if (!i) // 如果key对应的node不存在 则将新的环境变量字符串加入到环境变量链表minienv中
	{ 
        if (new_keypair)
        {
			list_append(new_keypair, &minienv); //把新的key_pair加入到minienv链表中
			free(new_keypair); //释放new_keypair
		}
	}
	else // 如果key对应的node存在 则更新node的value
	{ 
        free(i->key_pair); //把原来的key_pair释放掉
        i->key_pair = new_keypair; //把新的key_pair赋值给node
	}
}
