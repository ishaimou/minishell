/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:37:54 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:37:55 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	int		i;
	int		size;
	char	*str;

	if (s && f)
	{
		size = 0;
		while (s[size])
			size++;
		str = (char*)malloc(sizeof(*str) * (size + 1));
		if (str)
		{
			i = 0;
			while (i < size)
			{
				str[i] = (*f)(s[i]);
				i++;
			}
			str[i] = '\0';
		}
		return (str);
	}
	return (NULL);
}
