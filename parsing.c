/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ressalhi <ressalhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 16:26:20 by ressalhi          #+#    #+#             */
/*   Updated: 2022/07/23 12:43:31 by ressalhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**redirections(char **tab, t_data *data)
{
	int		i;
	char	**t;

	i = 0;
	while (tab[i])
		i++;
	t = malloc(sizeof(char *) * (i + 1));
	if (!t)
		return (0);
	data->num_red = count_red(tab);
	data->red = malloc(sizeof(t_red) * data->num_red);
	if (!data->red)
		return (0);
	return (redirections2(tab, t, data));
}

int	parsing2(char **t, t_parse *parse, int i)
{
	int	j;
	int	k;

	k = 0;
	while (t[k])
		k++;
	parse->data[i].cmd = ft_strdup(t[0]);
	free(t[0]);
	parse->data[i].args = malloc(sizeof(char *) * k);
	k = 0;
	j = 1;
	while (t[j])
	{
		parse->data[i].args[k++] = ft_strdup(t[j]);
		free(t[j]);
		j++;
	}
	parse->data[i].args[k] = 0;
	return (1);
}

void	clean_red(t_parse *p, int i)
{
	int	h;

	h = 0;
	while (h < p->data[i].num_red)
	{
		if (p->data[i].red[h].type != HERDOC)
			p->data[i].red[h].file = dollar_hundler(p->data[i].red[h].file,
					p);
		p->data[i].red[h].file = remove_qoutes(p->data[i].red[h].file);
		h++;
	}
	//system("leaks minishell");
}

int	parsing(char **tab, t_parse *p)
{
	int		i;
	char	*str;
	char	**t;

	i = 0;
	p->i = 0;
	p->j = 0;
	while (tab[i])
	{
		str = str_to_split(tab[i], ' ');
		if (!str)
			return (0);
		t = ft_split(str, SPLIT_TOKEN);
		free(str);
		t = redirections(t, &p->data[i]);
		if (!t)
			return (0);
		clean_red(p, i);
		if (!t[0])
		{
			p->data[i].cmd = NULL;
			p->data[i].args = malloc(sizeof(char *));
			p->data[i].args[0] = NULL;
			i++;
			continue;
		}
		parsing2(t, p, i);
		i++;
	}
	free(t);
	return (1);
}

int	parser(char *str, t_parse *parse)
{
	char	**tab;
	int		i;

	tab = error_management(str, parse);
	if (!tab)
		return (0);
	tab = expanding(tab);
	i = 0;
	while (tab[i])
		i++;
	parse->data = malloc(sizeof(t_data) * i);
	if (!parse->data)
		return (0);
	parse->num_data = i;
	if (!parsing(tab, parse))
		return (0);
	if (!dollar(parse, i))
		return (0);
	if (!remove_trash(parse, i))
		return (0);
	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
	return (1);
}
