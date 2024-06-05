/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu-chen <yu-chen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:48:15 by yu-chen           #+#    #+#             */
/*   Updated: 2024/06/03 19:35:05 by yu-chen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	*minishell(t_env *minienv)
{
	char	*input;
	int		exit_status;
	char	**cmds;

	while (1)
	{
		define_main_signals();
		input = prompt_input(minienv);
		if (has_input_error(input))
			continue ;
		handle_enpansions();
		if (!has_pipe(input))
			exit_status = exec_1cmd();
		else
		{
			cmds = split_cmds(input);
			free(input);
			exit_status = exec_multi_cmds();
			free_array(cmds);
		}
	}
	return (exit_status);
}
