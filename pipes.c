/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanouze <mmanouze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 15:01:44 by mmanouze          #+#    #+#             */
/*   Updated: 2022/08/16 16:49:38 by mmanouze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **ft_paths(char **env)
{
    int     i;
    char    *str;
    char    **ok;

    i = 0;
    str = NULL;
    while (env[i])
    {
        if (!ft_strncmp(env[i], "PATH", 4))
        {
            str = ft_strchr(env[i], '/');
            break ;
        }
        i++;
    }
    ok = ft_split(str, ':');
    return (ok);
}

char *command_path(char *cmd, char **paths)
{
    int i;
    int j;
    char *join;

    i = 0;
    j = 1;
    while (paths[i])
    {
        join = ft_strjoin_p(paths[i], cmd);
        if (!access(join, X_OK))
            return (join);
        free(join);
        i++;
    }
    return (NULL);
}

void do_command(t_parse *parse,int i, char **comd)
{
    char    **paths;
    char    *take;

    if (access(parse->data[i].cmd, F_OK & X_OK) == 0)
        execve(parse->data[i].cmd, comd, parse->env);
    paths = ft_paths(parse->env);
    take = command_path(comd[0], paths);
    if (!strcmp(comd[0], "pwd") || !strcmp(comd[0], "export")
        || !strcmp(comd[0], "env") || !strcmp(comd[0], "unset")
        || !strcmp(comd[0], "cd") || !strcmp(comd[0], "exit")
        || !strcmp(comd[0], "echo"))
    {
        excute_builtins(comd, parse);
        exit(1) ;
    }
    if (execve(take, comd, parse->env) == -1)
    {
        write(2, "bash: ", 6);
        write(2, comd[0], ft_strlen(comd[0]));
        write(2, " :command not found\n", 21);
        exit(127);
    }
}

void start(t_parse *parse,int i, pipex *t_pipe, char **comd)
{
    int k;

    pipe(t_pipe->fd);
    k = check_red(parse, t_pipe, i);
    t_pipe->wait_id[t_pipe->id] = fork();
    if (t_pipe->wait_id[t_pipe->id++] == 0)
    {
        if (k == 1)
        {
            if (g_status.g_status == 1)
				exit(1);
            t_pipe->out = 0;
            close(t_pipe->fd[0]);
            close(t_pipe->fd[1]);
            do_command(parse,i, comd);
        }
        else
        {
            dup2(t_pipe->fd[1], 1);
            close(t_pipe->fd[0]);
            close(t_pipe->fd[1]);
            do_command(parse,i, comd);
        }
    }
    else
    {
        // printf("+++%d\n", g_status);
        dup2(t_pipe->fd[0], 0);
        close(t_pipe->fd[0]);
        close(t_pipe->fd[1]);
    }
}

int check_red(t_parse *parse, pipex *t_pipe, int i)
{
    int c;

    c = 0;
    dup2(t_pipe->save[1], 1);
    t_pipe->in_err = 0;
    t_pipe->out = 0;
    if (parse->data[i].num_red >= 1)
    {
        while (c < parse->data[i].num_red)
        {
            if (parse->data[i].red[c].type == 1)
            {
                if (t_pipe->in_err != 9)
                {
                    t_pipe->file_inpt = open(parse->data[i].red[c].file, O_RDONLY, 0644);
                    dup2(t_pipe->file_inpt, 0);
                }
                if (access(parse->data[i].red[c].file, F_OK) == -1)
                {
                    dup2(t_pipe->save[1], 1);
                    printf("file %s does not exist\n", parse->data[i].red[c].file);
                    g_status.g_status = 1;
                    t_pipe->in_err = 9;
                }
            }
            else if (parse->data[i].red[c].type == 2)
            {
                if (t_pipe->in_err != 9)
                {
                    t_pipe->file_outpt = open(parse->data[i].red[c].file, O_CREAT | O_RDWR | O_TRUNC, 0644);
                    if (access(parse->data[i].red[c].file, R_OK) == -1)
                    {
                        g_status.g_status = 13;
                    }
                    dup2(t_pipe->file_outpt, 1);
                    t_pipe->out = 1;
                }
            }
            else if (parse->data[i].red[c].type == 4)
            {
                if (t_pipe->in_err != 9)
                {
                    t_pipe->file_appnd = open(parse->data[i].red[c].file, O_CREAT | O_APPEND | O_RDWR, 0644);
                    dup2(t_pipe->file_appnd, 1);
                }
            }
            else if (parse->data[i].red[c].type == 3)
                dup2(parse->data[i].fd[0], 0);
            c++;
        }
        if (t_pipe->out != 1 && parse->num_data != 1)
            dup2(t_pipe->fd[1], 1);
        return (1);
    }
    return (0);
}

// void set_red(t_parse *parse, pipex *t_pipe, int i)
// {
//     int c;

//     c = 0;
//     dup2(t_pipe->save[1], 1);
//     if (parse->data[i].num_red >= 1)
//     {
//         while (c < parse->data[i].num_red)
//         {
//             if (parse->data[i].red[c].type == 1)
//             {
//                 // t_pipe->file_inpt = open(parse->data[i].red[c].file, O_RDONLY, 0644);
//                 dup2(t_pipe->file_inpt, 0);
//             }
//             else if (parse->data[i].red[c].type == 2 && c <= t_pipe->out_cor)
//             {
//                 t_pipe->out = 1;
//                 // fprintf(stderr, "***** %d\n", t_pipe->file_outpt);
//                 // t_pipe->file_outpt = open(parse->data[i].red[c].file, O_CREAT | O_RDWR | O_TRUNC, 0644);
//                 dup2(t_pipe->file_outpt, 1);
//             }
//             else if (parse->data[i].red[c].type == 4)
//             {
//                     // t_pipe->file_appnd = open(parse->data[i].red[c].file, O_CREAT | O_APPEND | O_RDWR, 0644);
//                     dup2(t_pipe->file_appnd, 1);
//             }
//             else if (parse->data[i].red[c].type == 3)
//                 dup2(parse->data[i].fd[0], 0);
//             c++;
//         }
//         if (t_pipe->out != 1 && parse->num_data != 1)
//             dup2(t_pipe->fd[1], 1);
//     }
// }

void wait_cmd(pipex *t_pipe, t_parse *parse)
{
    int i;
    int j;
    int c;

    i = 0;
    j = 0;
    if (parse->num_data >= 1)
    {
        while (i < parse->num_data)
        {
            c = 0;
            if (parse->data[i].cmd)
                j++;
            // if (parse->data[i].num_red >= 1)
            // {
            //     while (c < parse->data[i].num_red)
            //     {
            //         if (parse->data[i].red[c].type == 3)
            //             j++;
            //         c++;
            //     }
            // }
            i++;
        }
    }
    // printf("%d\n", j);
    t_pipe->cmd_number = j;
    t_pipe->wait_id = malloc(sizeof(int) * (j));
}