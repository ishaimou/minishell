/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:42:30 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:42:31 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			is_format(char c)
{
	char	*ref;
	int		i;

	i = 0;
	ref = "cspdiouUxXfegrkb}%";
	while (ref[i])
		if (c == ref[i++])
			return (1);
	return (0);
}

int			is_postflag(char c)
{
	char	*ref;
	int		i;

	i = 0;
	ref = "lLh+-rjz";
	while (ref[i])
		if (c == ref[i++])
			return (1);
	return (0);
}

int			is_preflag(char c)
{
	char	*ref;
	int		i;

	i = 0;
	ref = " 0+-#'*$r";
	while (ref[i])
		if (c == ref[i++])
			return (1);
	return (0);
}
