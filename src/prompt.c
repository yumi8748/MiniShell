/* ************************************************************************** */
/*                                   ➡️                                        */
/*                                                        :::      ::::::::   */
/*   chen_prompt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:58:28 by leochen           #+#    #+#             */
/*   Updated: 2024/06/03 16:35:24 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*cat_prompt(char *user, char *hostname, char *dir,
		int free_hostname)
{
	static char	prompt[BUF_SIZE];

	ft_bzero(prompt, BUF_SIZE);
	ft_strlcat(prompt, YELLOW BOLD, BUF_SIZE);
	ft_strlcat(prompt, user, BUF_SIZE);
	ft_strlcat(prompt, WHITE, BUF_SIZE);
	ft_strlcat(prompt, "@", BUF_SIZE);
	ft_strlcat(prompt, PURPLE BOLD, BUF_SIZE);
	ft_strlcat(prompt, hostname, BUF_SIZE);
	ft_strlcat(prompt, WHITE, BUF_SIZE);
	ft_strlcat(prompt, ":", BUF_SIZE);
	ft_strlcat(prompt, CYAN BOLD, BUF_SIZE);
	ft_strlcat(prompt, dir, BUF_SIZE);
	ft_strlcat(prompt, RESET RESET_SIZE, BUF_SIZE);
	ft_strlcat(prompt, "🍀 ", BUF_SIZE);
	if (free_hostname == 1)
		free(hostname);
	return (prompt);
}

static char	*extract_hostname(char *session, char end_char)
{
	int		i;
	char	*hostname;

	i = 0;
	hostname = NULL;
	while (session[i] && session[i] != end_char)
		i++;
	if (session[i] && session[i] == end_char)
	{
		hostname = ft_substr(session, 0, i);
	}
	return (hostname);
}

static char	*get_hostname(t_env *minienv, int *free_hostname)
{
	char	*hostname;

	hostname = minienv_value("SESSION_MANAGER", minienv);
	if (!hostname)
	{
		hostname = "localhost";
		*free_hostname = 0;
	}
	else
	{
		hostname = ft_strchr(hostname, '/') + 1;
		hostname = extract_hostname(hostname, '.');
		*free_hostname = 1;
	}
	return (hostname);
}
char	*prompt_msg(t_env *minienv)
{
	char	*user;
	char	*hostname;
	char	*pwd;
	char	*dir;
	int		free_hostname;

	free_hostname = 0;
	user = minienv_value("USER", minienv);
	if (!user)
		user = "unknown";
	hostname = get_hostname(minienv, &free_hostname);
	pwd = minienv_value("PWD", minienv);
	if (!pwd)
		pwd = "unkown";
	if (ft_strrchr(pwd, '/') + 1)
		dir = ft_strrchr(pwd, '/') + 1;
	if (ft_strncmp(pwd, "/", 2) == 0)
		dir = "/";
	return (cat_prompt(user, hostname, dir, free_hostname));
}

char	*do_prompt(t_env *minienv)
{
	char	*input;

	input = readline(prompt_msg(minienv));
	if (!input)
		shell_exit(NULL, &minienv);
	if (input && *input)
		add_history(input);
	return (input);
}
