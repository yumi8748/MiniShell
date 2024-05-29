/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 20:06:48 by yu-chen           #+#    #+#             */
/*   Updated: 2024/02/22 16:40:32 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	sublen;
	char	*sub;

	if (!s)
		return (NULL);
	sublen = ft_strlen(s);
	if (sublen < start)
		return (ft_calloc(1, 1));
	if (sublen - start < len)
		len = sublen - start;
	sub = malloc(len + 1);
	if (!sub)
		return (NULL);
	ft_strlcpy(sub, s + start, len + 1);
	return (sub);
}
/*
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	newlen;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
	{
		sub = ft_calloc(1, sizeof(char));
		if (!sub)
			return (NULL);
		return (sub);
	}
	else
	{
		newlen = ft_strlen(s + start);
		if (!(len > newlen))
			newlen = len;
		sub = malloc(sizeof(char) * (newlen + 1));
		if (!sub)
			return (NULL);
		sub[newlen] = '\0';
		while (newlen-- > 0)
			sub[newlen] = s[start + newlen];
	}
	return (sub);
}*/
