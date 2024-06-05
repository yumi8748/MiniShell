/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:27:23 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/03 19:20:50 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

static int	count_words(const char *str, char sep)
{
	int	count;
	int	in_word;

	in_word = 0;
	count = 0;
	while (*str)
	{
		if (*str == sep)
			in_word = 0;
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		str++;
	}
	return (count);
}

static char	*word_duplicate(char const *str, int start, int end)
{
	int		i;
	char	*word;

	i = 0;
	word = malloc((end - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (start < end)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	int		index;
	char	**res;

	res = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!s || !res)
		return (NULL);
	i = 0;
	j = 0;
	index = -1;
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && index < 0)
			index = i;
		else if ((s[i] == c || i == ft_strlen(s)) && index >= 0)
		{
			res[j++] = word_duplicate(s, index, i);
			index = -1;
		}
		i++;
	}
	res[j] = 0;
	return (res);
}
/*
#include "../../inc/libft.h"
#include <stdio.h>

int	main(void)
{
	char const *inputString = "ahhhhhh-eeeee-noooo-umm";
	char delimiter = '-';

	char **result = ft_split(inputString, delimiter);

	if (result) {
		// 打印分割后的单词
		for (int i = 0; result[i] != NULL; i++) {
			printf("Word %d: %s\n", i + 1, result[i]);
		}

		// 释放分配的内存
		free_arr(result);
	} else {
		printf("Memory allocation failed.\n");
	}

	return (0);
}*/
