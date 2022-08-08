/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ressalhi <ressalhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/26 16:32:31 by ressalhi          #+#    #+#             */
/*   Updated: 2022/07/21 12:58:47 by ressalhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dollar_dqoutes(char *s, char *str, t_parse *parse)
{
	char	*s1;

	s[parse->j++] = str[parse->i++];
	while (str[parse->i] != '"' && str[parse->i])
	{
		if (str[parse->i] == '$' && str[parse->i + 1] != '"'
			&& str[parse->i + 1] != 39)
		{
			parse->i++;
			s1 = dollar_fill(str, parse, s1);
			fill_str(s1, s, parse);
		}
		else
			s[parse->j++] = str[parse->i];
		parse->i++;
	}
	if (str[parse->i] == '"')
		s[parse->j++] = str[parse->i];
}

int	dollar_dqoutes2(char *s, char *str, t_parse *parse)
{
	char	*s1;

	s1 = NULL;
	parse->i++;
	if (str[parse->i] == '\0')
	{
		s[parse->j++] = str[parse->i - 1];
		return (0);
	}
	if (str[parse->i] == '"')
		fill_str2(s, str, parse, '"');
	else if (str[parse->i] == 39)
		fill_str2(s, str, parse, 39);
	else
	{
		s1 = dollar_fill(str, parse, s1);
		fill_str(s1, s, parse);
	}
	return (1);
}

void	dollar_hundle2(char *s, char *str, t_parse *parse)
{
	while (str[parse->i])
	{
		if (str[parse->i] == '"')
			dollar_dqoutes(s, str, parse);
		else if (str[parse->i] == 39)
			fill_str2(s, str, parse, 39);
		else if (str[parse->i] == '$')
		{
			if (!dollar_dqoutes2(s, str, parse))
				break ;
		}
		else
			s[parse->j++] = str[parse->i];
		parse->i++;
	}
}

char	*dollar_hundler(char *str, t_parse *parse)
{
	char	*s;

	if (!str)
		return (NULL);
	parse->j = variable_count(str, parse);
	if (!parse->j)
		return (NULL);
	s = malloc(parse->j + 1);
	parse->i = 0;
	parse->j = 0;
	dollar_hundle2(s, str, parse);
	s[parse->j] = '\0';
	free(str);
	return (s);
}

int	dollar(t_parse *p, int len)
{
	int	i;
	int	j;

	i = 0;
	while (i < len)
	{
		p->data[i].cmd = dollar_hundler(p->data[i].cmd, p);
		j = 0;
		while (p->data[i].args[j])
		{
			p->data[i].args[j] = dollar_hundler(p->data[i].args[j], p);
			j++;
		}
		j = 0;
		while (j < p->data[i].num_red)
		{
			p->data[i].red[j].file = dollar_hundler(p->data[i].red[j].file,
					p);
			j++;
		}
		i++;
	}
	return (1);
}
