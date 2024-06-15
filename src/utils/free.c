/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:38:06 by leochen           #+#    #+#             */
/*   Updated: 2024/06/14 17:18:01 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../inc/minishell.h"

void    free_str_array(char **s)   //changed name  void	free_array(char **arr)
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