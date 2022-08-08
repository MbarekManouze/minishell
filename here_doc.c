/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanouze <mmanouze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 11:04:17 by mmanouze          #+#    #+#             */
/*   Updated: 2022/08/07 19:58:42 by mmanouze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int here_doc(pipex *h_doc, char *limiter)
{
    char *read;
    int i;

    i = 0;
    pipe(h_doc->hr_dc);
    // close(h_doc->hr_dc[0]);
    while (1)
    {
        read = readline(">");
        if (!ft_strncmp(limiter, read, ft_strlen(read) - 2))
        {
            free(read);
            exit(1);
        }
        else if (!read)
        {
            free(read);
            exit(1);
        }
        write(h_doc->hr_dc[1], read, ft_strlen(read));
        write(h_doc->hr_dc[1], "\n", 1);
        free(read);
    }
    free(limiter);
    close(h_doc->hr_dc[1]);
    return (h_doc->hr_dc[0]);
}

void execute_hd(int s,int i, t_parse *parse)
{
    int id;
    char **argv;

    if (parse->data[i].cmd != NULL)
    {
        id = fork();
        if (id == 0)
        {
            dup2(s, 0);
            argv = join_args(parse, i);
            do_command(parse, i, argv);
        }
    }
}

int count_h_d(t_parse *parse)
{
    int i;
    int j;
    int c;

    i = 0;
    j = 0;
    c = 0;
    while (i < parse->num_data)
    {
        if (parse->data[i].num_red >= 1)
        {
            j = 0;
            while (j < parse->data[i].num_red)
            {
                if (parse->data[i].red->type == HERDOC)
                    c++;
                j++;
            }
        }
        i++;
    }
    return (c);
}
