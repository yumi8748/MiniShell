/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 13:24:29 by leochen           #+#    #+#             */
/*   Updated: 2024/07/01 18:59:49 by leochen          ###   ########.fr       */
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
			continue ;
		handle_expansions(&input, minienv, exit_status);
		if (!has_pipe(input))
			exit_status = execute_one_cmd(input, &minienv);
		else
		{
			splited_cmds = split_all_cmds(input);
			free(input);
			exit_status = execute_multi_cmds(splited_cmds, &minienv);
			free_str_array(splited_cmds);
		}
	}
	return (exit_status);
}
