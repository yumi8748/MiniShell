/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chen_one_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:02:31 by leochen           #+#    #+#             */
/*   Updated: 2024/06/07 12:56:30 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// int	str_equal(const char *str1, const char *str2)  //公共
// {
// 	size_t	size;

// 	if (!str1 || !str2)
// 		return (0);
// 	size = ft_strlen(str1);
// 	if (size != ft_strlen(str2))
// 		return (0);
// 	return (ft_strncmp(str1, str2, size) == 0);
// }

// int	is_builtin(char *command)    //公共
// {
// 	if (!command)
// 		return (0);
// 	if (str_equal(command, "echo"))
// 		return (1);
// 	if (str_equal(command, "cd"))
// 		return (1);
// 	if (str_equal(command, "pwd"))
// 		return (1);
// 	if (str_equal(command, "export"))
// 		return (1);
// 	if (str_equal(command, "unset"))
// 		return (1);
// 	if (str_equal(command, "env"))
// 		return (1);
// 	if (str_equal(command, "exit"))
// 		return (1);
// 	return (0);
// }

// char	**split_args(char *command)  //公共 
// {
// 	char	**exec_args;

// 	if (!contains_quote_mark(command))
// 		return (ft_split(command, ' '));
// 	replace_spaces(command, '"');
// 	replace_spaces(command, '\'');
// 	remove_quotes(command);
// 	exec_args = ft_split(command, ' ');
// 	restore_spaces(exec_args);
// 	return (exec_args);
// }


// static int	handle_redirects(char *command, int original_fds[2])
// {
// 	char	redirect;

// 	original_fds[IN] = NO_REDIRECT;
// 	original_fds[OUT] = NO_REDIRECT;
// 	redirect = get_next_redirect(command);
// 	while (redirect)
// 	{
// 		if (redirect == '<')
// 		{
// 			if (!handle_input_redirect(command, original_fds))
// 				return (FAILED);
// 		}
// 		if (redirect == '>')
// 		{
// 			if (!handle_output_redirect(command, original_fds))
// 				return (FAILED);
// 		}
// 		if (redirect < 0)
// 		{
// 			save_original_fd_in(original_fds);
// 			redirect_heredoc(command, redirect);
// 		}
// 		redirect = get_next_redirect(command);
// 	}
// 	return (SUCCESS);
// }

// int	execute_forked_builtin(char **args, t_env **minienv)
// {
// 	int	exit_status;

// 	exit_status = execute_builtin(args, minienv);
// 	free_array(args);
// 	free_minienv(minienv);
// 	rl_clear_history();
// 	exit(exit_status);
// }

// int	execute_builtin(char **args, t_env **minienv)
// {
// 	char	*command;

// 	command = args[0];
// 	if (str_equal(command, "echo"))
// 		return (echo(args));
// 	if (str_equal(command, "pwd"))
// 		return (pwd());
// 	if (str_equal(command, "env"))
// 		return (env(*minienv));
// 	if (str_equal(command, "export"))
// 		return (builtin_export(args, minienv));
// 	if (str_equal(command, "unset"))
// 		return (unset(args, minienv));
// 	if (str_equal(command, "cd"))
// 		return (cd(args, *minienv));
// 	if (str_equal(command, "exit"))
// 		return (builtin_exit(args, minienv));
// 	else
// 		return (EXIT_FAILURE);
// }

// static void	restore_original_fds(int original_fds[2])
// {
// 	if (original_fds[IN] != NO_REDIRECT)
// 		redirect_fd(original_fds[IN], STDIN_FILENO);
// 	if (original_fds[OUT] != NO_REDIRECT)
// 		redirect_fd(original_fds[OUT], STDOUT_FILENO);
// }

// int	execute_forked_external(char **args, t_env *minienv)
// {
// 	int		child_pid;
// 	char	*command;

// 	command = args[0];
// 	child_pid = fork();
// 	define_execute_signals(child_pid);
// 	if (child_pid == -1)
// 		print_perror_msg("fork", command);
// 	else if (child_pid == 0)
// 		execute_external(args, minienv);
// 	else
// 		return (wait_for_child(child_pid, TRUE));
// 	exit(EXIT_FAILURE);
// }

// int	execute_one_command(char *command, t_env **minienv)
// {
// 	char	**args;
// 	int		exit_status;
// 	int		original_fds[2];

// 	if (!handle_redirects(command, &original_fds[0]))
// 	{
// 		restore_original_fds(original_fds);
// 		free(command);
// 		return (EXIT_FAILURE);
// 	}
// 	args = split_args(command);
// 	free(command);
// 	if (is_builtin(args[0]))
// 		exit_status = execute_builtin(args, minienv);
// 	else
// 		exit_status = execute_forked_external(args, *minienv);
// 	free_array(args);
// 	restore_original_fds(original_fds);
// 	return (exit_status);
// }


// int	has_pipe(char *str)
// {
// 	while (*str)
// 	{
// 		if (*str == '\'')
// 		{
// 			str++;
// 			while (*str != '\'')
// 				str++;
// 		}
// 		if (*str == '"')
// 		{
// 			str++;
// 			while (*str != '"')
// 				str++;
// 		}
// 		if (*str == '|')
// 			return (1);
// 		str++;
// 	}
// 	return (0);
// }