/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 20:14:51 by yu-chen           #+#    #+#             */
/*   Updated: 2024/03/01 19:17:04 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (*(s + i) != (unsigned char)c)
	{
		if (*(s + i) == '\0')
			return (NULL);
		i++;
	}
	return ((char *)s + i);
}

/*#include <stdio.h>
int	main(void)
{
	char	str[] = "Hello!";
	printf("%s", ft_strchr(str, 'l'));
}*/
