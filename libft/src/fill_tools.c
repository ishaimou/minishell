/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:24:50 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:25:05 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char		*ft_strcolor(char *str)
{
	char	*res;
	int		len;
	int		i;

	len = 0;
	while (str[len] && str[len] != '}' && str[len] != '%')
		len++;
	res = ft_strnew(len);
	i = -1;
	while (++i < len)
		res[i] = str[i];
	return (res);
}

int			adv_freeflag(char **str)
{
	free(*str);
	return (-1);
}
