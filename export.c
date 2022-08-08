/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ressalhi <ressalhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 19:14:47 by ressalhi          #+#    #+#             */
/*   Updated: 2022/07/19 16:45:17 by ressalhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_env(char **env)
{
	int		i;
	int		j;
	int		h;
	char	*s;

	h = 0;
	while (env[h])
		h++;
	i = 0;
	while (i < h)
	{
		j = i + 1;
		while (j < h)
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				s = env[i];
				env[i] = env[j];
				env[j] = s;
			}
			j++;
		}
		i++;
	}
}

void	add_print_declare(char **env)
{
	int		i;
	int		j;

	i = 0;
	while (env[i])
	{
		if (!strchr(env[i], '='))
			printf("declare -x %s\n", env[i]);
		else
		{
			printf("declare -x ");
			j = 0;
			while (env[i][j])
			{
				if (env[i][j] == '=')
				{
					printf("%c", env[i][j]);
					break ;
				}
				printf("%c", env[i][j]);
				j++;
			}
			printf("%c%s%c\n", '"', env[i] + j + 1, '"');
		}
		i++;
	}
}

int	check_valid(char *str)
{
	int	i;

	i = 0;
	if (!((str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= 'A' && str[i] <= 'Z')
			|| str[i] == '_'))
		return (0);
	while (str[i] != '=' && str[i])
	{
		if (!((str[i] >= 'a' && str[i] <= 'z')
				|| (str[i] >= 'A' && str[i] <= 'Z')
				|| (str[i] >= '0' && str[i] <= '9')
				|| str[i] == '_' || str[i] == '='))
			return (0);
		i++;
	}
	return (1);
}

int	get_index(char *str, char **env)
{
	int	i;
	int	j;
	int	index;

	j = 0;
	while (str[j])
	{
		if (str[j] == '=')
			break ;
		j++;
	}
	index = 0;
	i = 0;
	while (env[i])
	{
		if (!strncmp(str, env[i], j))
			return (i);
		i++;
	}
	return (-1);
}

char	**add_variables(char *str, char **env)
{
	char	**t;
	int		i;

	i = 0;
	while (env[i])
		i++;
	t = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (env[i])
	{
		t[i] = strdup(env[i]);
		free(env[i]);
		i++;
	}
	free(env);
	t[i] = strdup(str);
	i++;
	t[i] = 0;
	return (t);
}

void	modify_variable(char *str, char **env, int index)
{
	int	i;

	if (!strchr(str, '='))
		return ;
	i = 0;
	while (env[i])
	{
		if (i == index)
		{
			free(env[i]);
			env[i] = strdup(str);
		}
		i++;
	}
}

void	export(char **tab, t_parse *parse)
{
	int	i;
	int	j;

	sort_env(parse->env);
	if (!tab[0])
	{
		add_print_declare(parse->env);
		return ;
	}
	i = 0;
	while (tab[i])
	{
		if (check_valid(tab[i]))
		{
			j = get_index(tab[i], parse->env);
			if (j == -1)
				parse->env = add_variables(tab[i], parse->env);
			else
				modify_variable(tab[i], parse->env, j);
		}
		else
			printf("bash: export: `%s': not a valid identifier\n", tab[i]);
		i++;
	}
}
