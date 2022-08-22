/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanouze <mmanouze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 13:38:57 by ressalhi          #+#    #+#             */
/*   Updated: 2022/08/22 11:31:58 by mmanouze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_num(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] == '-' || str[i] == '+')
			count++;
		if ((str[i] == '-' || str[i] == '+') && str[i + 1] == '\0')
			return (0);
		if ((str[i] < '0' || str[i] > '9') && str[i] != '-' && str[i] != '+')
			return (0);
		i++;
	}
	if (count >= 2)
		return (0);
	return (1);
}

void	exit_code(char *str)
{
	int	code;

	code = atoi(str);
	if (!g_status.g_id)
	{
		printf("exit\n");
		exit (code);
	}
	else
		exit (code);
}

void	ft_exit2(char **tab)
{
	if (!tab[0])
	{
		if (!g_status.g_id)
		{
			printf("exit\n");
			exit (g_status.g_status);
		}
		else
			exit (g_status.g_status);
	}
	if (!check_num(tab[0]))
	{
		if (!g_status.g_id)
		{
			printf("exit\nbash: exit: %s: numeric argument required\n", tab[0]);
			exit (255);
		}
		else
		{
			printf("bash: exit: %s: numeric argument required\n", tab[0]);
			exit (255);
		}
	}
}

void	ft_exit(char **tab)
{
	int	i;

	ft_exit2(tab);
	i = 0;
	while (tab[i])
		i++;
	if (i > 1)
	{
		if (!g_status.g_id)
			printf("exit\nbash: exit: too many arguments\n");
		else
			printf("bash: exit: too many arguments\n");
	}
	else
		exit_code(tab[0]);
}
