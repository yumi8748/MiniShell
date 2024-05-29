/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 19:03:39 by yu-chen           #+#    #+#             */
/*   Updated: 2024/02/22 16:40:32 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	size_t	i;

	if (len == 0 || dest == src)
		return (dest);
	if (dest < src)
	{
		i = 0;
		while (i < len)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	else if (dest > src)
	{
		i = len - 1;
		while (i < len)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i--;
		}
	}
	return (dest);
}
/*#include <stdio.h>
#include <string.h>

int main() {
    char data[] = "Hello, World!";
    char test[] = "Hello, World!";
    ft_memmove(data + 5, data, 7);
    memmove(test + 5, test, 7);

    printf("处理重叠内存块的结果: %s\n", data);
    printf("处理重叠内存块的结果: %s\n", test);

    return 0;
}*/
