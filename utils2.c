/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ressalhi <ressalhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 16:54:54 by ressalhi          #+#    #+#             */
/*   Updated: 2022/08/20 16:43:37 by ressalhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	errors_manag(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		if (!check_errors(tab[i]))
		{
			ft_free_tab(tab);
			return (0);
		}
		i++;
	}
	return (1);
}

char	**error_management(char *str, t_parse *parse)
{
	char	*s;
	char	**tab;

	if (!check_pipes(str))
	{
		free(str);
		return (0);
	}
	s = str_to_split2(str, '|');
	tab = ft_split(s, SPLIT_TOKEN);
	free(s);
	if (!tab)
		return (0);
	if (!check_qoutes(tab, parse))
	{
		ft_free_tab(tab);
		return (0);
	}
	if (!errors_manag(tab))
		return (0);
	return (tab);
}

int	ft_strcmp(char *s1, char *s2)
{
	if (!s1)
		return (1);
	while (*s1 != '\0' && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

char	*ft_strdup(char *s1)
{
	int		i;
	char	*str;

	str = malloc(ft_strlen(s1) + 1);
	if (!str)
		return (0);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

size_t	ft_strlen(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (0);
	str = malloc(strlen(s1) + strlen(s2) + 1);
	if (!str)
		return (0);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin2(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (0);
	str = malloc(strlen(s1) + strlen(s2) + 1);
	if (!str)
		return (0);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	free(s2);
	return (str);
}

char	*ft_strjoin3(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (0);
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (0);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	free(s1);
	return (str);
}

int	ft_isalnum(int c)
{
	return ((c <= 57 && c >= 48) || (c <= 90 && c >= 65)
		|| (c <= 122 && c >= 97) || c == '_');
}
