/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ressalhi <ressalhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:52:39 by ressalhi          #+#    #+#             */
/*   Updated: 2022/08/18 20:40:53 by ressalhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	excute_builtins(char **comd, t_parse *parse)
{
	if (!strcmp(comd[0], "echo"))
		ft_echo(comd+1);
	else if (!strcmp(comd[0], "export"))
		export(comd+1, parse);
	else if (!strcmp(comd[0], "env"))
		print_env(parse);
	else if (!strcmp(comd[0], "unset"))
		ft_unset(comd+1, parse);
	else if (!strcmp(comd[0], "pwd"))
		ft_pwd(parse);
	else if (!strcmp(comd[0], "cd"))
		ft_cd(comd+1, parse);
	else if (!strcmp(comd[0], "exit"))
		ft_exit(comd+1);
}

void	sig_int(int sign)
{
	if (sign == SIGINT && !g_status.g_herd)
	{
		g_status.g_status = 1;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	ft_free(t_parse *parse)
{
	int	i;
	int	j;

	i = 0;
	while (i < parse->num_data)
	{
		free(parse->data[i].cmd);
		j = 0;
		while (parse->data[i].args[j])
		{
			free(parse->data[i].args[j]);
			j++;
		}
		free(parse->data[i].args);
		j = 0;
		while (j < parse->data[i].num_red)
		{
			free(parse->data[i].red[j].file);
			j++;
		}
		if (parse->data[i].num_red >= 0)
			free(parse->data[i].red);
		i++;
	}
	free(parse->data);
}

int check_space(char *str)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (str[i])
		i++;
	while (str[j] == ' ')
		j++;
	if (j == i)
		return (1);
	else
		return (0);
}

int check(char *str)
{
	int i;
	
	i = 0;
	if (str[i] == '\0')
	{
		free(str);
		return (1);
	}
	while (str[i])
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}
	free(str);
	return (1);
}

int	main(int ac, char **av, char **env)
{
	char	*str;
	t_parse	*parse;
	pipex	*t_pipe;

	(void)ac;
	(void)av;
	parse = malloc(sizeof(t_parse));
	t_pipe = malloc(sizeof(pipex));
	t_pipe->save[0] = dup(0);
	t_pipe->save[1] = dup(1);
	t_pipe->out = 0;
	t_pipe->cmd_number = 0;
	t_pipe->in_err = 0;
	parse->env = ft_env(env);
	g_status.g_status = 0;
	while (1)
	{
		t_pipe->id = 0;
		signal(SIGINT, sig_int);
		signal(SIGQUIT, SIG_IGN);
		str = readline("bash-0.2$ ");
		if (!str)
		{
			printf("exit\n");
			exit (g_status.g_status);
		}
		if (check(str) || check_space(str))
			continue;
		add_history(str);
		if (!parser(str, parse))
			continue;
		//system("leaks minishell");
		wait_cmd(t_pipe, parse);
		if (check_for_builtins(parse, t_pipe))
		{
			dup2(t_pipe->save[0], 0);
			dup2(t_pipe->save[1], 1);
			ft_free(parse);
			continue;
		}
		ft_begin(parse, t_pipe);
		ft_free(parse);
		dup2(t_pipe->save[0], 0);
		dup2(t_pipe->save[1], 1);
	}
}

int check_for_builtins(t_parse *parse, pipex *t_pipe)
{
	char **comd;
	int j;
	int k;

	j = 0;
	if (parse->num_data == 1)
	{
		if (!ft_strcmp(parse->data[0].cmd, "pwd") || !ft_strcmp(parse->data[0].cmd, "export") || !ft_strcmp(parse->data[0].cmd, "env")
        	|| !ft_strcmp(parse->data[0].cmd, "unset") || !ft_strcmp(parse->data[0].cmd, "cd") || !ft_strcmp(parse->data[0].cmd, "exit")
			|| !ft_strcmp(parse->data[0].cmd, "echo"))
    	{
			h_d(parse);
			k = check_red(parse, t_pipe,0);
			if (g_status.g_status == 1 && k == 1)
				return (1);
			comd = join_args(parse, 0);
			excute_builtins(comd, parse);
			while (comd[j])
			{
				free(comd[j]);
				j++;
			}
			free(comd);
			return (1);
    	}
	}
	return (0);
}

void ft_begin(t_parse *parse, pipex *t_pipe)
{
	g_status.g_status = 0;
	h_d(parse);
	commands(parse, t_pipe);
}

void commands(t_parse *parse, pipex *t_pipe)
{
	char **cmd;
	int i;
	int j;
	int k;
	int status;

	i = 0;
	j = 0;
	while (i < parse->num_data - 1)
	{	
		if (parse->data[i].cmd == NULL)
		{
			j = check_red(parse, t_pipe, i);
			i++;
			continue;
		}
		cmd = join_args(parse, i);
		start(parse, i, t_pipe, cmd);
		j = 0;
		while (cmd[j])
		{
			free(cmd[j]);
			j++;
		}
		free(cmd);
		i++;
	}
	k = check_red(parse, t_pipe,i);
	if (find_here_d(parse, i) || parse->data[i].cmd)
	{
		t_pipe->wait_id[t_pipe->id] = fork();
		if (t_pipe->wait_id[t_pipe->id] == 0)
		{
			if (parse->data[i].cmd)
				cmd = join_args(parse, i);
			if (g_status.g_status == 13)
			{
				write(2, "permission denied\n", 19);
				exit(13);
			}
			if (g_status.g_status == 1)
				exit(1);
			do_command(parse, i, cmd);
		}
	}
	t_pipe->id = 0;
	close(0);
	while (t_pipe->id < t_pipe->cmd_number)
	{   
		if (waitpid(t_pipe->wait_id[t_pipe->id], &status, 0) == -1)
		{
			write(2, "Error waitpid\n", 15);
			exit(1);
		}
		if(WIFEXITED(status))
			g_status.g_status = WEXITSTATUS(status);
		if (status == 2)
			g_status.g_status = 130;
		if (status == 3)
			g_status.g_status = 131;
		t_pipe->id++;
	}
	if (t_pipe->cmd_number)
		free(t_pipe->wait_id);
	t_pipe->wait_id = 0;
}

void h_d(t_parse *parse)
{
	int i;
	int c;
	int id;
	int sts;

	i = 0;
	while (i < parse->num_data)
	{
		c = 0;
		if (parse->data[i].num_red >= 1)
		{
			while (c < parse->data[i].num_red)
			{
				if (parse->data[i].red[c].type == HERDOC)
				{
					pipe(parse->data[i].fd);
					id = fork();
					if (id == 0)
						here_doc(parse, ft_strdup(parse->data[i].red[c].file), i);
					else
					{
						waitpid(id, &sts, 0);
						if (sts == 2)
							g_status.g_status = 1;
						else
							g_status.g_status = 0;
						close(parse->data[i].fd[1]);
					}
				}
				c++;
			}
		}
		i++;
	}
}

int count_args(t_parse *parse, int i)
{
	int j;
	int c;

	j = 0;
	c = 0;
	if (parse->data[i].cmd)
	{
		c++;
		while (parse->data[i].args[j])
		{
			j++;
			c++;
		}
	}
	return (c);
}

char **join_args(t_parse *parse, int i)
{
	char **split;
	int j;
	int c;
	int len;

	j = 0;
	c = 0;
	len = count_args(parse, i);
	if (!len)
		return (NULL);
	split = malloc(sizeof(char *) * (len + 1));
	if (parse->data[i].cmd)
	{
		split[j] = ft_strdup(parse->data[i].cmd);
		j++;
		if (parse->data[i].args[c])
		{
			while (parse->data[i].args[c])
			{
				split[j] = ft_strdup(parse->data[i].args[c]);
				c++;
				j++;
			}
		}
		split[j] = 0;
	}
	return (split);
}
