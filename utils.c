/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanouze <mmanouze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 16:36:35 by ressalhi          #+#    #+#             */
/*   Updated: 2022/08/16 19:17:14 by mmanouze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_qoutes3(char *str, int i, t_parse *parse, char c)
{
	parse->dqoute++;
	i++;
	while (str[i])
	{
		if (str[i] == c)
		{
			parse->dqoute++;
			break ;
		}
		i++;
	}
	return (i);
}

int	check_qoutes4(char *str, int i, t_parse *parse, char c)
{
	parse->sqoute++;
	i++;
	while (str[i])
	{
		if (str[i] == c)
		{
			parse->sqoute++;
			break ;
		}
		i++;
	}
	return (i);
}

int	check_qoutes2(char *str, t_parse *parse)
{
	int	i;

	parse->dqoute = 0;
	parse->sqoute = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '"')
		{
			i = check_qoutes3(str, i, parse, '"');
			if (!str[i])
				break ;
		}
		else if (str[i] == 39)
		{
			i = check_qoutes4(str, i, parse, 39);
			if (!str[i])
				break ;
		}
		i++;
	}
	if (parse->dqoute % 2 != 0 || parse->sqoute % 2 != 0)
	{
		printf("Unclosed Qoute\n");
		return (0);
	}
	return (1);
}

int	check_qoutes(char **tab, t_parse *parse)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		if (!check_qoutes2(tab[i], parse))
			return (0);
		i++;
	}
	return (1);
}

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

int	count_red(char **str)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if (str[i][j] == '"')
			{
				j++;
				while (str[i][j] != '"' && str[i][j])
					j++;
			}
			else if (str[i][j] == 39)
			{
				j++;
				while (str[i][j] != 39 && str[i][j])
					j++;
			}
			else if ((str[i][j] == '>' && str[i][j + 1] == '>')
				|| (str[i][j] == '<' && str[i][j + 1] == '<'))
			{
				j++;
				count++;
			}
			else if (str[i][j] == '>' || str[i][j] == '<')
				count++;
			j++;
		}
		i++;
	}
	return (count);
}
