/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:12:23 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/29 14:59:32 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	replace_inquote_pipe(char *s)
{
	char	quote;

	quote = 0;
	while (*s)
	{
		if ((*s == '\"' || *s == '\'') && !quote)
			quote = *s;
		else if (*s == quote)
			quote = 0;
		else if (*s == '|' && quote)
			*s = '\x01';
		s++;
	}
}

void	reset_inquote_pipe(char **splited_cmds)
{
	char	*cmd;

	while (*splited_cmds)
	{
		cmd = *splited_cmds;
		while (*cmd)
		{
			if (*cmd == '\x01')
				*cmd = '|';
			cmd++;
		}
		splited_cmds++;
	}
}

/*char	**split_all_cmds(char *cmd_line) //跳过引号内的管道符号 按照管道符号分割命令
{
	char	**splited_cmds;

	replace_inquote_pipe(cmd_line); //将引号内的管道符号替换为-1
	splited_cmds= ft_split(cmd_line,  '|');  //按照管道符号分割命令
	reset_inquote_pipe(splited_cmds);  //将分割后的命令中的-1替换为管道符号
	// printf("reset cmd line:%s\n", cmd_line);
	// printf("split cmd by '|':splited_cmd[0]:%s\n", splited_cmds[0]);
	// printf("split cmd by '|':splited_cmd[1]:%s\n", splited_cmds[1]);
	return (splited_cmds);  //返回分割后的命令
}*/

char	**split_all_cmds(char *cmd_line)
{
	char	*cmd_copy;
	char	**splited_cmds;

	cmd_copy = ft_strdup(cmd_line);
	replace_inquote_pipe(cmd_copy);
	splited_cmds = ft_split(cmd_copy, '|');
	reset_inquote_pipe(splited_cmds);
	free(cmd_copy);
	return (splited_cmds);
}

char	**split_one_arg(char *cmd) //按照空格分解splited_cmd[i]
{
	char **args;

	if (!has_quote(cmd))
		return (ft_split(cmd, ' ')); //如果没有引号 就直接用空格分割
	replace_inquote_spaces(cmd);     //处理成对出现的引号 把里面的空格替换成-1
	remove_quotes(cmd);              //去掉引号
	args = ft_split(cmd, ' ');
	reset_spaces(args); //把1替换成空格
	return (args);
}
