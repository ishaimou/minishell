/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:47:56 by ishaimou          #+#    #+#             */
/*   Updated: 2020/02/02 11:14:37 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_sep(char c)
{
	return (c == ':' || c == '/' || c == '#' || c == '-'
			|| c == '{' || c == '}' || c == '+');
}

int		check_arg(char *arg, int *i)
{
	if (arg[0] == '=')
		return (0);
	while (arg[*i] && arg[*i] != '=' && ft_isprint(arg[*i]))
		(*i)++;
	if (arg[*i] != '=')
		return (0);
	return (1);
}

void	print_chars(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			ft_putstr("\\n");
		else if (str[i] == '\t')
			ft_putstr("\\t");
		else
			ft_putchar(str[i]);
		i++;
	}
}

void	rm_trailing_slash(char **path)
{
	int		i;
	char	*str;

	i = 0;
	if (*path && **path)
	{
		str = ft_strdup(*path);
		while (str[i])
			i++;
		i--;
		while (str[i] && str[i] == '/')
			i--;
		str[i + 1] = '\0';
		free(*path);
		*path = str;
	}
}
