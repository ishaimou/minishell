/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:39:25 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:39:27 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strrchr(const char *str, int c)
{
	char	*pt;

	pt = (char*)str;
	if (c == '\0')
	{
		while (*pt != '\0')
			pt++;
		return (pt);
	}
	while (*str != '\0')
	{
		if (*str == (const char)c)
			pt = (char*)str;
		str++;
	}
	if (*pt == (char)c)
		return (pt);
	return (0);
}
