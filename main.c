/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ressalhi <ressalhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:52:39 by ressalhi          #+#    #+#             */
/*   Updated: 2022/08/21 19:03:36 by ressalhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	excute_builtins(char **comd, t_parse *parse)
{
	if (!strcmp(comd[0], "echo"))
		ft_echo(comd + 1);
	else if (!strcmp(comd[0], "export"))
		ft_export(comd + 1, parse);
	else if (!strcmp(comd[0], "env"))
		print_env(parse);
	else if (!strcmp(comd[0], "unset"))
		ft_unset(comd + 1, parse);
	else if (!strcmp(comd[0], "pwd"))
		ft_pwd(parse);
	else if (!strcmp(comd[0], "cd"))
		ft_cd(comd + 1, parse);
	else if (!strcmp(comd[0], "exit"))
		ft_exit(comd + 1);
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

int	check_space(char *str)
{
	int	i;
	int	j;

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

int	check(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
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
	initializing(t_pipe, parse, env);
	while (1)
	{
		signal(SIGINT, sig_int);
		signal(SIGQUIT, SIG_IGN);
		str = readline("bash-0.2$ ");
		if (!str)
		{
			printf("exit\n");
			exit(g_status.g_status);
		}
		if (check(str) || check_space(str))
			continue ;
		add_history(str);
		if (!parser(str, parse))
			continue ;
		wait_cmd(t_pipe, parse);
		if (check_for_builtins(parse, t_pipe))
		{
			ft_default(t_pipe);
			ft_free(parse);
			continue ;
		}
		ft_begin(parse, t_pipe);
		ft_free(parse);
		ft_default(t_pipe);
	}
}
