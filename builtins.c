/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ressalhi <ressalhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 11:30:14 by ressalhi          #+#    #+#             */
/*   Updated: 2022/07/19 15:24:40 by ressalhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	modify_oldpwd(char **env)
{
	int		i;
	char	*s;
	char	*s1;

	s = getcwd(NULL, 0);
	s1 = ft_strjoin2("OLDPWD=", s);
	i = 0;
	while (env[i])
	{
		if (!strncmp("OLDPWD", env[i], 6))
		{
			free(env[i]);
			env[i] = strdup(s1);
		}
		i++;
	}
	free(s1);
}

void	modify_pwd(char **env)
{
	int		i;
	char	*s;
	char	*s1;

	s = getcwd(NULL, 0);
	s1 = ft_strjoin2("PWD=", s);
	i = 0;
	while (env[i])
	{
		if (!strncmp("PWD", env[i], 3))
		{
			free(env[i]);
			env[i] = strdup(s1);
		}
		i++;
	}
	free(s1);
}

int	cher_home(char *s, char **env)
{
	int		i;
	int		j;

	i = 0;
	while (env[i])
	{
		j = strncmp(s, env[i], ft_strlen(s));
		if (j == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	*env_cher2(char *s1, char **env)
{
	int		i;
	int		j;
	char	*s;

	i = 0;
	while (env[i])
	{
		j = strncmp(s1, env[i], (ft_strlen(s1)));
		if (j == 0)
		{
			s = ft_strdup(env[i] + ft_strlen(s1) + 1);
			return (s);
		}
		i++;
	}
	return (0);
}

void	ft_cd(char **tab, t_parse *parse)
{
	int		i;
	char	*s;

	if (!tab[0])
	{
		i = cher_home("HOME", parse->env);
		if (i == -1)
		{
			printf("bash: cd: HOME not set\n");
			return ;
		}
		else
		{
			modify_oldpwd(parse->env);
			s = env_cher2("HOME", parse->env);
			if (!s)
				return ;
			i = chdir(s);
			free(s);
			if (i != 0)
				printf("Error\n");
			modify_pwd(parse->env);
		}
	}
	else
	{
		modify_oldpwd(parse->env);
		i = chdir(tab[0]);
		if (i != 0)
		{
			printf("bash: cd: %s: No such file or directory\n", tab[0]);
			return ;
		}
		modify_pwd(parse->env);
	}
}

int	check_num(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] == '-' || str[i] == '+')
			count++;
		if ((str[i] == '-' || str[i] == '+') && str[i + 1] == '\0')
			return (0);
		if ((str[i] == '-' || str[i] == '+')
			&& (str[i - 1] >= '0' && str[i - 1] <= '9'))
			return (0);
		if ((str[i] < '0' || str[i] > '9') && str[i] != '-' && str[i] != '+')
			return (0);
		i++;
	}
	if (count >= 2)
		return (0);
	return (1);
}

void	exit_code(char *str)
{
	int	code;
	
	code = atoi(str);
	if (code <= 255 && code >= 0)
	{
		printf("exit\n");
		exit (code);	
	}
	else
	{
		if (code > 0)
		{
			printf("exit\n");
			code = code % 256;
			exit (code);
		}
		else
		{
			printf("exit\n");
			code = (code % 256) + 256;
			exit (code);
		}
	}
}

void	ft_exit(char **tab)
{
	int	i;
	
	i = 0;
	if (!tab[0])
	{
		printf("exit\n");
		exit (0);
	}
	if (!check_num(tab[0]))
	{
		printf("exit\nbash: exit: %s: numeric argument required\n", tab[0]);
		exit (255);
	}
	while (tab[i])
		i++;
	if (i > 1)
	{
		printf("exit\nbash: exit: too many arguments\n");
		return ;
	}
	else
		exit_code(tab[0]);
}
