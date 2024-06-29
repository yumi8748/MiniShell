/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 13:24:29 by leochen           #+#    #+#             */
/*   Updated: 2024/06/29 15:04:57 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	minishell(t_env *minienv)
{
	int		exit_status;
	char	*input;
	char	**splited_cmds;

	exit_status = EXIT_SUCCESS;
	while (1)
	{
		define_main_signals();
		input = do_prompt(minienv);
		if (is_input_error(input, &exit_status, minienv))
			// if error skip the rest of the loop and start over the loop to provide another prompt
			continue ;
		handle_expansions(&input, minienv, exit_status);
		if (!has_pipe(input))
			exit_status = execute_one_cmd(input, &minienv);
		else
		{
			splited_cmds = split_all_cmds(input); //跳过引号内的管道符号 按照管道符号分割命令
			free(input);
			exit_status = execute_multi_cmds(splited_cmds, &minienv);
			free_str_array(splited_cmds);
		}
	}
	return (exit_status);
}
