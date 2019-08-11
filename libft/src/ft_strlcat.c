/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:37:33 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:37:35 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	len_dest;
	size_t	len_src;
	char	*psrc;

	len_dest = 0;
	len_src = 0;
	psrc = (char*)src;
	while (src[len_src] != '\0')
		len_src++;
	while ((*dest != '\0') && (size > 0))
	{
		dest++;
		len_dest++;
		size--;
	}
	while ((*psrc != '\0') && (size > 1))
	{
		*dest = *psrc;
		dest++;
		psrc++;
		size--;
	}
	if (*psrc == 0 || size == 1)
		*dest = '\0';
	return (len_dest + len_src);
}
