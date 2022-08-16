/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanouze <mmanouze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 16:30:31 by ressalhi          #+#    #+#             */
/*   Updated: 2022/08/16 19:08:48 by mmanouze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_to_split(char *str, char c)
{
	int		i;
	char	*s;

	s = ft_strdup(str);
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			s[i] = SPLIT_TOKEN;
		else if (s[i] == '"')
		{
			i++;
			while (s[i] != '"' && s[i])
				i++;
		}
		else if (s[i] == 39)
		{
			i++;
			while (s[i] != 39 && s[i])
				i++;
		}
		i++;
	}
	return (s);
}

char	*str_to_split2(char *str, char c)
{
	int		i;
	char	*s;

	s = ft_strdup(str);
	free(str);
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			s[i] = SPLIT_TOKEN;
		else if (s[i] == '"')
		{
			i++;
			if (s[i] == '\0')
				break ;
			while (s[i] != '"' && s[i])
				i++;
			if (s[i] == '\0')
				break ;
		}
		else if (s[i] == 39)
		{
			i++;
			if (s[i] == '\0')
				break ;
			while (s[i] != 39 && s[i])
				i++;
			if (s[i] == '\0')
				break ;
		}
		i++;
	}
	return (s);
}


int	check_pipes(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '|')
	{
		printf("Syntax error\n");
		return (0);
	}
	while (str[i])
	{
		if (str[i] == '|')
		{
			i++;
			while (str[i] == ' ')
				i++;
			if (str[i] == '|' || str[i] == '\0')
			{
				printf("Syntax error\n");
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int	check_error2(char *str, int i)
{
	while (str[i] == ' ')
		i++;
	if (str[i] == '|' || str[i] == '<'
		|| str[i] == '>' || str[i] == '\0')
	{
		printf("Syntax error\n");
		return (-1);
	}
	return (i);
}

int	check_errors3(char *str, int i)
{
	if (str[i] == '<')
	{
		i++;
		i = check_error2(str, i);
		if (i == -1)
			return (0);
	}
	else if (str[i] == '>')
	{
		i++;
		i = check_error2(str, i);
		if (i == -1)
			return (0);
	}
	return (1);
}

int	check_errors(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
		{
			i = i + 2;
			i = check_error2(str, i);
			if (i == -1)
				return (0);
		}
		else if (str[i] == '>' && str[i + 1] == '>')
		{
			i = i + 2;
			i = check_error2(str, i);
			if (i == -1)
				return (0);
		}
		else if (!check_errors3(str, i))
			return (0);
		i++;
	}
	return (1);
}
