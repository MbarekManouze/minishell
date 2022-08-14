/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanouze <mmanouze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/26 16:34:06 by ressalhi          #+#    #+#             */
/*   Updated: 2022/08/14 10:27:25 by mmanouze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*env_cher(char *s1, char **env)
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
			if (env[i][ft_strlen(s1)] == '=')
				return (s);
			else
				return (0);
			free(s1);
		}
		i++;
	}
	return (0);
}

void	fill_str(char *s1, char *s, t_parse *parse)
{
	int	k;

	if (s1)
	{
		k = 0;
		while (s1[k])
		{
			s[parse->j++] = s1[k];
			k++;
		}
		free(s1);
	}
	else
		free(s1);
}

char	*dollar_fill(char *str, t_parse *parse, char *s1)
{
	int		k;

	k = 0;
	while ((str[parse->i] >= 'a' && str[parse->i] <= 'z')
		|| (str[parse->i] >= 'A' && str[parse->i] <= 'Z')
		|| (str[parse->i] >= '0' && str[parse->i] <= '9')
		|| str[parse->i] == '_')
	{
		parse->i++;
		k++;
	}
	s1 = malloc(k + 1);
	parse->i = parse->i - k;
	k = 0;
	while ((str[parse->i] >= 'a' && str[parse->i] <= 'z')
		|| (str[parse->i] >= 'A' && str[parse->i] <= 'Z')
		|| (str[parse->i] >= '0' && str[parse->i] <= '9')
		|| str[parse->i] == '_')
		s1[k++] = str[parse->i++];
	s1[k] = '\0';
	s1 = env_cher(s1, parse->env);
	parse->i--;
	return (s1);
}
