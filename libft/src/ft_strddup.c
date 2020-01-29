/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strddup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 04:21:25 by ishaimou          #+#    #+#             */
/*   Updated: 2020/01/29 04:21:46 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_strddup(char **str)
{
	char	**ddup;
	int		argc;
	int		i;

	ddup = NULL;
	if (str)
	{
		argc = get_argc(str);
		ddup = (char**)malloc(sizeof(char*) * (argc + 1));
		ddup[argc] = NULL;
		i = 0;
		while (str[i])
		{
			ddup[i] = ft_strdup(str[i]);
			i++;
		}
	}
	return (ddup);
}
