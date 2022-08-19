/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ressalhi <ressalhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 16:11:28 by mmanouze          #+#    #+#             */
/*   Updated: 2022/08/17 18:10:05 by ressalhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_env(char **env)
{
	char	**tab;
	int		i;

	i = 0;
	while (env[i])
		i++;
	tab = malloc(sizeof(char *) * (i + 1));
	if (!tab)
		return (0);
	i = 0;
	while (env[i])
	{
		tab[i] = ft_strdup(env[i]);
		i++;
	}
	tab[i] = 0;
	return (tab);
}
