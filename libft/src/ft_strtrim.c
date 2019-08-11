/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:40:20 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:40:21 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int		ft_is_blank(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

static	char	*ft_return(int start, int end, const char *s)
{
	char	*trim;
	int		i;

	if (end <= start)
	{
		trim = (char*)malloc(sizeof(*trim));
		trim[0] = '\0';
		return (trim);
	}
	trim = (char*)malloc(sizeof(*trim) * (end - start + 2));
	if (trim == NULL)
		return (NULL);
	i = start;
	while (i <= end)
	{
		trim[i - start] = s[i];
		i++;
	}
	trim[i - start] = '\0';
	return (trim);
}

char			*ft_strtrim(char const *s)
{
	int		end;
	int		start;

	if (s != NULL)
	{
		start = 0;
		while (ft_is_blank(s[start]) && s[start] != '\0')
			start++;
		end = start;
		while (s[end] != '\0')
			end++;
		end--;
		while (ft_is_blank(s[end]))
			end--;
		return (ft_return(start, end, s));
	}
	return (NULL);
}
