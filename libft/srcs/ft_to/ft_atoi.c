/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 17:32:23 by yu-chen           #+#    #+#             */
/*   Updated: 2023/11/16 17:02:04 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *s)
{
	int	res;
	int	mul;

	res = 0;
	mul = 1;
	while (*s == 32 || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-')
	{
		mul *= (-1);
		s++;
	}
	else if (*s == '+')
		s++;
	while (*s >= '0' && *s <= '9')
	{
		res = (res * 10) + (*s - '0');
		s++;
	}
	return (res * mul);
}
/*#include <stdio.h>
int main() 
{
    const char *number_str = "123";
    int result = ft_atoi(number_str);
    printf("%d\n", result);  // 输出: 123

    const char *negative_str = "-456";
    result = ft_atoi(negative_str);
    printf("%d\n", result);  // 输出: -456

    const char *with_spaces = " 789 ";
    result = ft_atoi(with_spaces);
    printf("%d\n", result);  // 输出: 789

    return 0;
}*/
