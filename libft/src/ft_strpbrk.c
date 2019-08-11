/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strpbrk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:39:12 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:39:13 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strpbrk(const char *s1, const char *s2)
{
	char	*pt1;
	char	*pt2;

	if (!s1 || !s2)
		return (NULL);
	pt1 = (char*)s1;
	while (*pt1)
	{
		pt2 = (char*)s2;
		while (*pt2)
		{
			if (*pt2 == *pt1)
				return (pt1);
			pt2++;
		}
		pt1++;
	}
	return (NULL);
}
