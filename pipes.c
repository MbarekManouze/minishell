/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanouze <mmanouze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 15:01:44 by mmanouze          #+#    #+#             */
/*   Updated: 2022/08/08 11:29:29 by mmanouze         ###   ########.fr       */
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
        fprintf(stderr, "****hnaaa\n");
        excute_builtins(comd, parse);
        exit(1) ;
    }
    if (execve(take, comd, parse->env) == -1)
    {
        write(2, "command not found\n", 19);
        exit(1);
    }
}

void start(t_parse *parse,int i, pipex *t_pipe, char **comd)
{
    int id;

    // if (!strcmp(comd[0], "pwd") || !strcmp(comd[0], "export") || !strcmp(comd[0], "env")
    //     || !strcmp(comd[0], "unset") || !strcmp(comd[0], "cd") || !strcmp(comd[0], "exit"))
    // {
    //     fprintf(stderr, "ana hnaaaa ahh db\n");
    //     excute_builtins(parse);
    //     return ;
    // }
    pipe(t_pipe->fd);
    id = fork();
    if (id == 0)
    {
        if (check_red(parse, t_pipe, i) == 1)
        {
            if (i == 0)
                dup2(t_pipe->fd[1], 1);
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
        dup2(t_pipe->fd[0], 0);
        close(t_pipe->fd[0]);
        close(t_pipe->fd[1]);
    }
}

int check_red(t_parse *parse, pipex *t_pipe, int i)
{
    int c;

    c = 0;
    if (parse->data[i].num_red >= 1)
    {
        while (c < parse->data[i].num_red)
        {
            if (parse->data[i].red[c].type == 1)
            {
                if (access(parse->data[i].red[c].file, F_OK) == -1)
                {
                    dup2(t_pipe->save[1], 1);
                    printf("file %s does not exist\n", parse->data[i].red[c].file);
                    exit(1);
                }
                t_pipe->file_inpt = open(parse->data[i].red[c].file, O_RDONLY, 0644);
                dup2(t_pipe->file_inpt, 0);
            }
            else if (parse->data[i].red[c].type == 2)
            {
                t_pipe->file_outpt = open(parse->data[i].red[c].file, O_CREAT | O_RDWR | O_TRUNC, 0644);
                dup2(t_pipe->file_outpt, 1);
            }
            else if (parse->data[i].red[c].type == 4)
            {
                t_pipe->file_appnd = open(parse->data[i].red[c].file, O_CREAT | O_APPEND | O_RDWR, 0644);
                dup2(t_pipe->file_appnd, 1);
            }
            c++;
        }
        return (1);
    }
    return (0);
}
