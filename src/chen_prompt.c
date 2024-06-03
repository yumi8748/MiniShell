/* ************************************************************************** */
/*                                   ➡️                                         */
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

#define BUF_SIZE 2048
# define BUILTIN_MISUSE 2
//# define LLONG_MAX 9223372036854775807
# define BOLD    "\001\e[1m\002"
# define RESET_SIZE   "\001\e[0m\002"
# define GREEN	"\001\e[0;32m\002"
# define YELLOW	"\001\e[0;93m\002"
# define BLUE	"\001\e[0;34m\002"
# define PURPLE	"\001\e[0;35m\002"
# define CYAN	"\001\e[0;96m\002"
# define WHITE	"\001\e[0;37m\002"
# define RESET	"\001\e[0m\002"

static char *cat_prompt(char *user, char *hostname, char *dir)
{
	static char prompt[BUF_SIZE];

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
	free(hostname);
	return (prompt);	
}

static char *extract_hostname(char *session, char end_char)
{
	int	i;
	char *hostname;

	i = 0;
	hostname = NULL;
	while(session[i] && session[i] != end_char)
		i++;
	if (session[i] && session[i] == end_char)
		hostname = ft_substr(session, 0, i);
	return (hostname);

}

char	*prompt_msg(t_env *minienv)
{
	char *user;
	char *hostname;
	char *pwd;
	char *dir;

	user = minienv_value("USER", minienv);
	if (!user)
		user = "unknown";
	hostname = minienv_value("SESSION_MANAGER", minienv);
	if (!hostname)
		hostname = "localhost";
	else 
	{
		hostname = ft_strchr(hostname, '/') + 1;
		hostname = extract_hostname(hostname, '.');
	}
	pwd = minienv_value("PWD", minienv);
	if (!pwd)
		pwd = "unkown";
	if (ft_strrchr(pwd, '/') + 1)
		dir = ft_strrchr(pwd, '/') + 1;
	if (ft_strncmp(pwd, "/", 2) == 0)
		dir = "/";	
	return (cat_prompt(user, hostname, dir));
}

char	*do_prompt(t_env *minienv)
{
	char *input;

	input = readline(prompt_msg(minienv));
	if (!input)
		shell_exit(NULL, &minienv);
	if (input && *input)
		add_history(input);
	return (input);
}

/////////////////////////////////////////////////////////////////////////////////////////
void	shell_exit(char **args, t_env **minienv)
{
	int	exit_status;

	exit_status = 0;
	if (args && args[1])
	{
		if (!is_valid_long_long(args[1]))
			exit_with_error("exit", "numeric argument required", BUILTIN_MISUSE);
		if (args[2])
			exit_with_error("exit", "too many arguments", EXIT_FAILURE);

		exit_status = ft_atoll(args[1]);
	}
	clean_up_exit(args, minienv, exit_status);
}

void	clean_up_exit(char **args, t_env **minienv, int exit_status)
{
	rl_clear_history();
	free_minienv(minienv);
	close_all_fds();
	free_str_array(args);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(exit_status);
}

void	free_minienv(t_env **minienv)
{
	t_env	*aux;
	t_env	*next;

	if (!minienv || !*minienv) 
		return;
	aux = *minienv;
	while (aux)
	{
		free(aux->key_pair);
		next = aux->next;
		free(aux);
		aux = next;
	}
	minienv = NULL;
}

void	check_args_error(char **args)
{
	char	*exit_status;

	if (!args || !args[1])
	{
		if (args)
			free_str_array(args);
		close_all_fds();
		exit(EXIT_SUCCESS);
	}
	exit_status = args[1];
	if (!is_valid_long_long(exit_status))
	{
		free_str_array(args);
		exit_with_error("exit", "numeric argument required", BUILTIN_MISUSE);
	}
	if (args[2] != NULL)
	{
		free_str_array(args);
		exit_with_error("exit", "too many arguments", EXIT_FAILURE);
	}
}

void	print_error_msg(char *command, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	exit_with_error(char *command, char *msg, int error)
{
	print_error_msg(command, msg);
	clean_up_exit(NULL, NULL, error);
}
int	is_valid_long_long(char *str)
{
	long long	out;
	int			c;

	if (ft_strlen(str) > 20)
		return (0);
	if (ft_strncmp(str, "-9223372036854775808", 21) == 0)
		return (1);
	out = 0;
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		c = *str - '0';
		if (out > (LLONG_MAX - c) / 10)
			return (0);
		out = out * 10 + c;
		str++;
	}
	return (1);
}

void	print_perror_msg(char *command, char *perror_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror(perror_msg);
}

void	close_extra_fds(void)
{
	int	last_open_fd;

	last_open_fd = open("/tmp/last_fd", O_RDWR | O_CREAT, 0666);
	if (last_open_fd == -1)
		print_perror_msg("open", "/tmp/last_fd");
	while (last_open_fd > STDERR_FILENO)
	{
		close(last_open_fd);
		last_open_fd--;
	}
}

void	close_all_fds(void)
{
	close_extra_fds();
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

long long	ft_atoll(const char *str)
{
	long long	number;
	int			sign;

	number = 0;
	sign = 1;
	while (ft_isspace(*str) && !(*str == '-' || *str == '+'))
	{
		str++;
	}
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = sign * -1;
		str++;
	}
	while (*str && ft_isdigit(*str))
	{
		number = (number * 10) + (*str - '0');
		str++;
	}
	return (number * sign);
}



void    free_str_array(char **s)
{
    int i;
  
    i = 0;
    if (s != NULL)
    {
        while (s[i] != NULL)
        {
            free(s[i]);
            s[i] = NULL;
            i++;
        }
        free(s);
    }
}
