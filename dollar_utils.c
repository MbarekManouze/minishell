/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ressalhi <ressalhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 13:54:57 by ressalhi          #+#    #+#             */
/*   Updated: 2022/07/21 11:12:00 by ressalhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_str2(char *s, char *str, t_parse *parse, char c)
{
	s[parse->j++] = str[parse->i++];
	while (str[parse->i] != c && str[parse->i])
		s[parse->j++] = str[parse->i++];
	s[parse->j++] = str[parse->i];
}

void	variable_c(char *str, int *count, int *i, t_parse *p)
{
	int		k;
	char	*s;

	k = 0;
	while ((str[(*i)] >= 'a' && str[(*i)] <= 'z')
		|| (str[(*i)] >= 'A' && str[(*i)] <= 'Z')
		|| (str[(*i)] >= '0' && str[(*i)] <= '9') || str[(*i)] == '_')
	{
		k++;
		(*i)++;
	}
	s = malloc(k + 1);
	(*i) = (*i) - k;
	k = 0;
	while ((str[(*i)] >= 'a' && str[(*i)] <= 'z')
		|| (str[(*i)] >= 'A' && str[(*i)] <= 'Z')
		|| (str[(*i)] >= '0' && str[(*i)] <= '9') || str[(*i)] == '_')
		s[k++] = str[(*i)++];
	s[k] = '\0';
	s = env_cher(s, p->env);
	if (s)
	{
		(*count) += ft_strlen(s);
		free(s);
	}
}

int	variable_count(char *str, t_parse *p)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			variable_c(str, &count, &i, p);
			i--;
		}
		else
			count++;
		i++;
	}
	return (count);
}
