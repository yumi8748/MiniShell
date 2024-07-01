/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:59:43 by leochen           #+#    #+#             */
/*   Updated: 2024/07/01 15:23:45 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


int skip_quotes(char *s, int i, char quote_type)
{
	i++;
	while(s[i] && s[i] != quote_type)
		i++;
	if (s[i])
		i++;
	return (i);
}

char *find_here_pos(char *str)
{
    int i;

	i = 0;
    while (str[i] && str[i + 1])
    {
        if (str[i] == '\'')
			i = skip_quotes(str, i, '\'');
        else if (str[i] == '\"')
        {
           i = skip_quotes(str, i, '\"');
        }
        if (str[i] == '<' && str[i + 1] == '<')
            return (&str[i]); // 返回指向 heredoc 标志的指针年
        else
            i++; // 继续遍历下一个字符
    }
    return (NULL); // 未找到 heredoc 标志，返回 NULL
}



//mememove字符串 s 的第二个字符开始复制整个字符串内容（包括末尾的 '\0' 结束字符）到字符串的开始位置，即从 s[1] 到 s[0]。这通常用于移除字符串的第一个字符。
char *name_after_redirect(char *s)
{
    int start;
	int	end;
	char *name_after_redirect;
	char *rest_cmd;

	start = 0; 
	ft_memmove(s, s + 1, ft_strlen(s + 1) + 1); //删除本来s开头的定向符 s是dest 指向放复制内容的目标数组 s+1指向要复制的数据源的指针 ft_strlen(s + 1) + 1是要复制的字节数 它会从源位置 src 复制 n 个字节到目标位置 des
    if (s[start] == '>')    //删除了开头定向符的s 如果还是有另外一个> 说明是>> 再次删除
		ft_memmove(s, s + 1, ft_strlen(s + 1) + 1);
	while (s[start] == ' ' || s[start] == '\t')  //跳过空格
        start++;
	end = find_end(s, start); //找到分隔符的位置 分割符包括空格 > < | \t  这里的s是redirect符号后面的字符串 可能包括空格 start是文件名开始位置
	name_after_redirect = ft_substr(&s[start], 0, end - start);//从s的start位置0开始截取end长度的字符串
	rest_cmd = &s[end];
	ft_memmove(s, rest_cmd, ft_strlen(rest_cmd) + 2);
   // printf("Extracted delimiter: '%s'\n", name_after_redirect);
	//printf("res cmd: '%s'\n", s);
    return (name_after_redirect);
}

int	find_end(char *s, int start) //找到文件名结束的位置 也就是遇到分割符 分割符包括空格 > < | \t  返回的是分割符的index
{
	int	end;

	end = start;
	while (s[end] && !is_label_delimiter(s[end]))
	{
		if (s[end] == '\'') //如果是单引号 删除单引号 跳过单引号内的内容 也就是单引号内的内容不作为分割符 遇到单引号结束的位置 再次删除单引号
		{
			ft_memmove(&s[end], &s[end] + 1, ft_strlen(&s[end] + 1) + 1); //删除单引号
			while (s[end] && s[end] != '\'') //找到单引号结束的位置
				end++;
			ft_memmove(&s[end], &s[end] + 1, ft_strlen(&s[end] + 1) + 1); //删除单引号
		}
		else if (s[end] == '\"')
		{
			ft_memmove(&s[end], &s[end] + 1, ft_strlen(&s[end] + 1) + 1);
			while (s[end] && s[end] != '\"')
				end++;
			ft_memmove(&s[end], &s[end] + 1, ft_strlen(&s[end] + 1) + 1);
		}
		else if (s[end] && !is_label_delimiter(s[end]))
			end++;
	}
	return (end);
}

int	is_label_delimiter(char c)
{
	if (c == ' ' || c == '>' || c == '<' || c == '|' || c == '\t')
		return (1);
	return (0);
}
