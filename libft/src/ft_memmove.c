/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:31:14 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:31:16 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	void	*pt;

	pt = ft_memalloc(len);
	if (pt == NULL)
		return (dst);
	ft_memcpy(pt, src, len);
	ft_memcpy(dst, pt, len);
	free(pt);
	return (dst);
}
