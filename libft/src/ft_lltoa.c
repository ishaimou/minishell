/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lltoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:27:33 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:27:35 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long long int	nbr_val(long long int nbr)
{
	if (nbr < 0)
		return (-nbr);
	return (nbr);
}

static int				sign_val(long long int nbr)
{
	if (nbr < 0)
		return (1);
	return (0);
}

static int				size_val(long long int nbr)
{
	if (nbr < 0)
	{
		if (nbr == -9223372036854775807)
			return (-1);
		return (2);
	}
	return (1);
}

char					*ft_lltoa(long long int nbr)
{
	char				*res;
	long long int		tmp;
	int					size;
	int					sign;

	size = size_val(nbr);
	sign = sign_val(nbr);
	if (sign == -1)
		return (ft_strdup("-9223372036854775807"));
	tmp = nbr_val(nbr);
	while (tmp /= 10)
		size++;
	if (!(res = (char*)malloc(sizeof(char) * (size + 1))))
		return (res);
	res[size] = '\0';
	if (sign)
		res[0] = '-';
	tmp = nbr_val(nbr);
	while (size-- > sign)
	{
		res[size] = (tmp % 10) + 48;
		tmp /= 10;
	}
	return (res);
}
