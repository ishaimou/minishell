/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:37:26 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:37:28 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	len_s1;
	unsigned int	len_s2;
	char			*join;

	if (s1 && s2)
	{
		len_s1 = ft_strlen(s1);
		len_s2 = ft_strlen(s2);
		join = (char*)malloc(sizeof(*join) * (len_s1 + len_s2 + 1));
		if (join == NULL)
			return (NULL);
		i = 0;
		j = 0;
		while (i < len_s1)
			join[j++] = s1[i++];
		i = 0;
		while (i < len_s2)
			join[j++] = s2[i++];
		join[j] = '\0';
		return (join);
	}
	return (NULL);
}
