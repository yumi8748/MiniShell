/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 18:20:55 by leochen           #+#    #+#             */
/*   Updated: 2024/06/27 18:21:02 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	get_redir_symbol(char *s)  //返回的是具体的定向符号 或者返回-1(<<)
{
     int	i;

    i = 0;
    while (s[i])
     {
         if (s[i] == '\'' || s[i] == '\"')
            skip_quotes(s, i, s[i]);
        if (s[i] == '<' && s[i+1] == '<') // check for '<<'
            return (-1); // return -1 for '<<'
        else if (s[i] == '<' || s[i] == '>' || s[i] < 0)
             return (s[i]);
        i++;
     }
     return (0);
 }