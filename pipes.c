/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanouze <mmanouze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 15:01:44 by mmanouze          #+#    #+#             */
/*   Updated: 2022/08/20 18:59:31 by mmanouze         ###   ########.fr       */
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
    if (str == NULL)
        return (NULL);
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

void protection(char *comd)
{
    write(2, "bash: ", 6);
    write(2, comd, ft_strlen(comd));
    write(2, " : No such file or directory\n", 30);
    exit(127);
}

void do_command(t_parse *parse,int i, char **comd)
{
    char    **paths;
    char    *take;

    g_status.g_id = 1;
    if (!strcmp(comd[0], "pwd") || !strcmp(comd[0], "export")
        || !strcmp(comd[0], "env") || !strcmp(comd[0], "unset")
        || !strcmp(comd[0], "cd") || !strcmp(comd[0], "exit")
        || !strcmp(comd[0], "echo"))
    {
        excute_builtins(comd, parse);
        exit(0) ;
    }
    if (access(parse->data[i].cmd, F_OK & X_OK) == 0)
        execve(parse->data[i].cmd, comd, parse->env);
    paths = ft_paths(parse->env);
    if (!paths)
        protection(comd[0]);
    take = command_path(comd[0], paths);
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

    g_status.g_status = 0;
    pipe(t_pipe->fd);
    k = check_red(parse, t_pipe, i);
    t_pipe->wait_id[t_pipe->id] = fork();
    if (t_pipe->wait_id[t_pipe->id++] == 0)
    {
        if (k == 1)
        {
            if (g_status.g_conti == 1)
                exit(1);
            if (g_status.g_status == 1)
				exit(1);
            if (parse->data[i].sign == 1)
				exit(1);
            t_pipe->out = 0;
            close(t_pipe->fd[0]);
            close(t_pipe->fd[1]);
            do_command(parse,i, comd);
        }
        else
        {
            if (g_status.g_conti == 1)
                exit(1);
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
    dup2(t_pipe->save[1], 1);
    t_pipe->in_err = 0;
    t_pipe->out_err = 0;
    t_pipe->out = 0;
    if (parse->data[i].num_red >= 1)
    {
        while (c < parse->data[i].num_red)
        {
            if (parse->data[i].red[c].type == 1 && g_status.g_status != 1 && parse->data[i].cmd && g_status.g_conti != 1)
            {
                g_status.g_status = 0;
                if (t_pipe->in_err != 9 && parse->data[i].cmd)
                {
                    t_pipe->file_inpt = open(parse->data[i].red[c].file, O_RDONLY, 0644);
                    dup2(t_pipe->file_inpt, 0);
                }
                if (access(parse->data[i].red[c].file, F_OK) == -1)
                {
                    dup2(t_pipe->save[1], 1);
                    printf("bash: %s: No such file or directory\n", parse->data[i].red[c].file);
                    g_status.g_status = 1;
                    t_pipe->in_err = 9;
                }
                else if (access(parse->data[i].red[c].file, R_OK) == -1)
                {
                    dup2(t_pipe->save[1], 1);
                    printf("bash: %s: Permission denied\n", parse->data[i].red[c].file);
                    g_status.g_status = 1;
                    t_pipe->in_err = 9;
                }
            }
            else if (parse->data[i].red[c].type == 2 && g_status.g_conti != 1)
            {
                if (t_pipe->in_err != 9 && t_pipe->out_err == 0 && g_status.g_status != 1)
                {
                    g_status.g_status = 0;
                    t_pipe->file_outpt = open(parse->data[i].red[c].file, O_CREAT | O_RDWR | O_TRUNC, 0644);
                    if (access(parse->data[i].red[c].file, W_OK) == -1)
                    {
                        printf("bash: %s: Permission denied\n", parse->data[i].red[c].file); 
                        t_pipe->out_err = 1;
                        g_status.g_status = 1;
                    }
                    dup2(t_pipe->file_outpt, 1);
                    t_pipe->out = 1;
                }
            }
            else if (parse->data[i].red[c].type == 4 && g_status.g_conti != 1)
            {
                if (t_pipe->in_err != 9 && t_pipe->out_err == 0 && g_status.g_status != 1)
                {
                    g_status.g_status = 0;
                    t_pipe->file_appnd = open(parse->data[i].red[c].file, O_CREAT | O_APPEND | O_RDWR, 0644);
                    if (access(parse->data[i].red[c].file, W_OK) == -1)
                    {
                        printf("bash: %s: Permission denied\n", parse->data[i].red[c].file); 
                        t_pipe->out_err = 1;
                        g_status.g_status = 1;
                    }
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
            {
                if (parse->num_data == 1 && not_builtins(parse, 0)){}
                else
                    j++;
            }
            i++;
        }
    }
    t_pipe->cmd_number = j;
    if (t_pipe->cmd_number)
        t_pipe->wait_id = malloc(sizeof(int) * (j));
}

int not_builtins(t_parse *parse, int i)
{
    if (!ft_strcmp(parse->data[i].cmd, "pwd") || !ft_strcmp(parse->data[i].cmd, "export") || !ft_strcmp(parse->data[i].cmd, "env")
    || !ft_strcmp(parse->data[i].cmd, "unset") || !ft_strcmp(parse->data[i].cmd, "cd") || !ft_strcmp(parse->data[i].cmd, "exit")
    || !ft_strcmp(parse->data[i].cmd, "echo"))
        return (1);
    return (0);
}