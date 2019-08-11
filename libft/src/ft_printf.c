/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:31:43 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:31:45 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
**	len[3]:		0: len		1: len_format		2: pflag
*/

static int			exception(char *format, int *len)
{
	unsigned int	i;
	int				l;

	l = 0;
	if (len[2] == -1)
	{
		i = 0;
		while (format[i] && format[i] != '%')
			ft_putchar(format[i++]);
		return (0);
	}
	l = put_spstr(format);
	if (format[len[2] - 1] == '%')
		return (-1);
	return (l);
}

int					ft_printf(const char *format, ...)
{
	t_chr			*chr;
	t_lst			*lst;
	va_list			ap;
	int				len[3];

	len[0] = 0;
	len[1] = 0;
	len[2] = 0;
	while (format[len[1]])
		len[1]++;
	va_start(ap, format);
	lst = parse_format(ap, (char*)format, &len[2]);
	if (!lst)
		return (exception((char*)format, len));
	if (!(chr = load_chr((char*)format, lst)))
	{
		free_lst(&lst);
		return (-1);
	}
	fill_chr(lst, chr, ap);
	len[0] = put_chr(lst, chr);
	free_lst(&lst);
	free_chr(&chr);
	va_end(ap);
	return (len[0]);
}
