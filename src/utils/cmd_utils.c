/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:12:23 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/22 14:09:37 by leochen          ###   ########.fr       */
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
			if (*cmd == -1)
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
					*s = -1;
				s++;
			}
	}
	if (*s)
		s++;
	}
}

char	**split_all_cmds(char *cmd_line) //跳过引号内的管道符号 按照管道符号分割命令
{
	char	**splited_cmds;

	replace_inquote_pipe(cmd_line); //将引号内的管道符号替换为-1
	splited_cmds= ft_split(cmd_line, '|');  //按照管道符号分割命令
	reset_inquote_pipe(splited_cmds);  //将分割后的命令中的-1替换为管道符号
	printf("splited_cmd[0]:%s\n", splited_cmds[0]);
	printf("splited_cmd[1]:%s\n", splited_cmds[1]);
	return (splited_cmds);  //返回分割后的命令
}




