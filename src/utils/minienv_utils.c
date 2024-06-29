/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minienv_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:37:20 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/29 15:03:22 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char *get_value(char *keypair)
{
    int i;

    i = 0;
    while (keypair[i] && keypair[i] != '=')
        i++;
    if (!keypair[i])
        return (NULL);
    return (&keypair[i + 1]);
}

char *get_key(char *keypair)
{
    int i;

    i = 0;
    while (keypair[i] && keypair[i] != '=')
        i++;
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

