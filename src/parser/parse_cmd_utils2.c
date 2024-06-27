/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:12:23 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/27 18:39:24 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


void reset_inquote_pipe(char **splited_cmds)
{
	char *cmd;

	while (*splited_cmds != NULL)
	{
		cmd = *splited_cmds;
		while (*cmd != '\0')
		{
			if (*cmd == '\x01')
				*cmd = '|';
			cmd++;
		}
		splited_cmds++;
	}
}

void	replace_inquote_pipe(char *s)
{
	char quote;

	while (*s)
	{
		if (*s == '\"' || *s == '\'')
		{
			quote = *s;
			s++;
			while (*s && *s != quote)
			{
				if (*s == '|')
					*s = '\x01';
				s++;
			}
	}
		else
			s++;
	}
}

char	**split_all_cmds(char *cmd_line) //跳过引号内的管道符号 按照管道符号分割命令
{
	char	**splited_cmds;

	replace_inquote_pipe(cmd_line); //将引号内的管道符号替换为-1
	splited_cmds= ft_split(cmd_line,  '|');  //按照管道符号分割命令
	reset_inquote_pipe(splited_cmds);  //将分割后的命令中的-1替换为管道符号
	// printf("reset cmd line:%s\n", cmd_line);
	// printf("split cmd by '|':splited_cmd[0]:%s\n", splited_cmds[0]);
	// printf("split cmd by '|':splited_cmd[1]:%s\n", splited_cmds[1]);
	return (splited_cmds);  //返回分割后的命令
}


char	**split_one_arg(char *cmd)   //按照空格分解splited_cmd[i]
{
	char	**args;

	if (!has_quote(cmd)) 
		return (ft_split(cmd, ' ')); //如果没有引号 就直接用空格分割
	replace_inquote_spaces(cmd); //处理成对出现的引号 把里面的空格替换成-1
	remove_quotes(cmd);  //去掉引号
	args = ft_split(cmd, ' ');
	reset_spaces(args); //把1替换成空格 
	return (args);
}