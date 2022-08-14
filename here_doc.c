/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanouze <mmanouze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 11:04:17 by mmanouze          #+#    #+#             */
/*   Updated: 2022/08/14 10:31:01 by mmanouze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int here_doc(t_parse *parse, char *limiter, int i)
{
    char *read;

    parse->data[i].fd = malloc(sizeof(int) * 2);
    parse->data[i].fd[0] = -1;
    parse->data[i].fd[1] = -1;
    pipe(parse->data[i].fd);
    while (1)
    {
        read = readline(">");
        if (!ft_strncmp(limiter, read, ft_strlen(read) - 1))
        {
            free(read);
            free(limiter);
            close(parse->data[i].fd[1]);
            parse->data[i].fd[1] = -1;
            return (0);
        }
        else if (!read)
        {
            free(read);
            return (0);
        }
        write(parse->data[i].fd[1], read, ft_strlen(read));
        write(parse->data[i].fd[1], "\n", 1);
        free(read);
    }
}
