/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanouze <mmanouze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 11:04:17 by mmanouze          #+#    #+#             */
/*   Updated: 2022/08/17 15:57:58 by mmanouze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int here_doc(t_parse *parse, char *limiter, int i)
{
    char *read;

    g_status.g_herd = 1;
    signal(SIGINT, SIG_DFL);
    while (1)
    {
        read = readline(">");
        if (!read)
        {
            free(read);
            exit(0);
        }
        else if (!ft_strcmp(limiter, read))
        {
            free(read);
            free(limiter);
            exit(1);
        }
        read = dollar_hundler(read, parse);
        write(parse->data[i].fd[1], read, ft_strlen(read));
        write(parse->data[i].fd[1], "\n", 1);
        free(read);
    }
}

int find_here_d(t_parse *parse, int i)
{
    int j;

    j = 0;
    if (parse->data[i].num_red >= 1 )
    {
        while (j < parse->data[i].num_red)
        {
            if (parse->data[i].red[j].type == HERDOC)
            {
                if (parse->data[i].cmd)
                    return (1);
            }
            j++;
        }
    }
    return (0);
}