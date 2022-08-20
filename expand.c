/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ressalhi <ressalhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 10:55:49 by ressalhi          #+#    #+#             */
/*   Updated: 2022/08/20 15:37:18 by ressalhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_dqoute(char *s, char *str, int *i, int *j)
{
	s[(*j)++] = str[(*i)];
	(*i)++;
	while (str[(*i)] != '"' && str[(*i)])
	{
		s[(*j)++] = str[(*i)];
		(*i)++;
	}
}

void	expand_sqoute(char *s, char *str, int *i, int *j)
{
	s[(*j)++] = str[(*i)];
	(*i)++;
	while (str[(*i)] != 39 && str[(*i)])
	{
		s[(*j)++] = str[(*i)];
		(*i)++;
	}
}

void	expand_noqoute(char *s, char *str, int *i, int *j)
{
	s[(*j)++] = ' ';
	s[(*j)++] = str[(*i)];
	s[(*j)++] = str[(*i) + 1];
	s[(*j)++] = ' ';
	(*i)++;
}

void	expaaaand(char *s, char *str, int *i, int *j)
{
	if (str[(*i)] == '"')
		expand_dqoute(s, str, i, j);
	else if (str[(*i)] == 39)
		expand_sqoute(s, str, i, j);
	if ((str[(*i)] == '>' && str[(*i) + 1] == '>')
		|| (str[(*i)] == '<' && str[(*i) + 1] == '<'))
		expand_noqoute(s, str, i, j);
	else if (str[(*i)] == '|' || str[(*i)] == '>' || str[(*i)] == '<')
	{
		s[(*j)++] = ' ';
		s[(*j)++] = str[(*i)];
		s[(*j)++] = ' ';
	}
	else
		s[(*j)++] = str[(*i)];
}

char	*expand(char *str)
{
	char	*s;
	int		i;
	int		j;

	s = malloc((ft_strlen(str) * 2) + 1);
	if (!s)
		return (0);
	i = 0;
	j = 0;
	while (str[i])
	{
		expaaaand(s, str, &i, &j);
		i++;
	}
	s[j] = '\0';
	free(str);
	return (s);
}

char	**expanding(char **tab)
{
	int		i;
	char	**t;

	i = 0;
	while (tab[i])
		i++;
	t = malloc(sizeof(char *) * (i + 1));
	if (!t)
		return (0);
	i = 0;
	while (tab[i])
	{
		t[i] = expand(tab[i]);
		if (!t[i])
			return (0);
		i++;
	}
	t[i] = 0;
	free(tab);
	return (t);
}
