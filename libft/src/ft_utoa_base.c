/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:41:13 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:41:14 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_utoa_base(size_t val, int base)
{
	size_t	nb;
	int		size;
	char	*res;

	size = 1;
	nb = val;
	while (nb /= base)
		size++;
	if (!(res = (char*)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	res[size] = '\0';
	nb = val;
	while (size-- > 0)
	{
		if (nb % base < 10)
			res[size] = (nb % base) + '0';
		else
			res[size] = (nb % base) + 'A' - 10;
		nb /= base;
	}
	return (res);
}
