/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:35:50 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:35:51 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*pt;

	pt = (char*)s;
	while (*pt)
	{
		if (*pt == (char)c)
			return (pt);
		pt++;
	}
	if (c == 0 && *pt == '\0')
		return (pt);
	return (NULL);
}
