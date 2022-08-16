/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanouze <mmanouze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/26 16:32:31 by ressalhi          #+#    #+#             */
/*   Updated: 2022/08/16 19:04:02 by mmanouze         ###   ########.fr       */
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
			if (str[parse->i] == '\0' || !ft_isalnum(str[parse->i]))
			{
				s[parse->j++] = str[parse->i - 1];
				if (!ft_isalnum(str[parse->i]) && str[parse->i] != '\0')
				{
					while (str[parse->i] != '$' && str[parse->i])
					{
						s[parse->j++] = str[parse->i];
						parse->i++;
					}
					parse->i--;
					return ;
				}
				return ;
			}
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
	if (str[parse->i] == '\0' || !ft_isalnum(str[parse->i]))
	{
		s[parse->j++] = str[parse->i - 1];
		if (!ft_isalnum(str[parse->i]) && str[parse->i] != '\0')
		{
			while (str[parse->i] != '$' && str[parse->i])
			{
				s[parse->j++] = str[parse->i];
				parse->i++;
			}
			parse->i--;
			return (1);
		}
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

int	count_status(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
			count++;
		i++;
	}
	return (count);
}

void	ft_join(char *s, char *str, int *i)
{
	int	j;

	j = 0;
	while (str[j])
	{
		s[(*i)] = str[j];
		j++;
		(*i)++;
	}
}

char	*expand_status(char *str)
{
    int i;
    int j;
    char    *s;
    char    *string;

	i = count_status(str);
	string = ft_itoa(g_status.g_status);
	s = malloc((ft_strlen(str) - (i * 2)) + (i * ft_strlen(string)) + 1);
    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == 39)
        {
            s[j++] = str[i++];
            while (str[i] != 39 && str[i])
            {
                s[j++] = str[i];
                i++;
            }
            s[j++] = str[i];
        }
        else if (str[i] == '"')
        {
            s[j++] = str[i++];
            while (str[i] != '"' && str[i])
            {
                if (str[i] == '$' && str[i + 1] == '?')
                {
                    ft_join(s, string, &j);
                    i++;
                }
				else
                	s[j++] = str[i];
                i++;
            }
            s[j++] = str[i];
        }
        else if (str[i] == '$' && str[i + 1] == '?')
        {
            ft_join(s, string, &j);
            i++;
        }
        else
            s[j++] = str[i];
        i++;
    }
    s[j] = '\0';
	free(str);
	free(string);
    return (s);
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
	s = expand_status(s);
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
		i++;
	}
	return (1);
}

