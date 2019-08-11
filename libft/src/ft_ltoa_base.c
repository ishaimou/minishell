/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ltoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:29:52 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:29:54 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int				affect_sign(long long int val)
{
	if (val < 0)
		return (1);
	return (0);
}

static long long int	affect_nb(long long int val)
{
	if (val < 0)
		return (-(long long int)val);
	return ((long long int)val);
}

char					*ft_itoa_base(long long int val, int base)
{
	long long int		nb;
	int					size;
	int					sign;
	char				*res;

	sign = affect_sign(val);
	size = affect_sign(val) + 1;
	nb = affect_nb(val);
	while (nb /= base)
		size++;
	if (!(res = (char*)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	res[size] = '\0';
	nb = affect_nb(val);
	if (sign)
		res[0] = '-';
	while (size-- > sign)
	{
		if (nb % base < 10)
			res[size] = (nb % base) + '0';
		else
			res[size] = (nb % base) + 'A' - 10;
		nb /= base;
	}
	return (res);
}
