/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:39:52 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:39:53 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strstr(const char *str, const char *find)
{
	unsigned int	i;
	unsigned int	j;
	char			*pt;

	i = 0;
	if (find[i] == '\0')
		return ((char*)str);
	pt = 0;
	while (str[i] != '\0')
	{
		j = 0;
		if (str[i] == find[j])
		{
			pt = (char*)&str[i];
			while (str[i + j] == find[j])
			{
				if (find[j + 1] == '\0')
					return (pt);
				j++;
			}
			pt = 0;
		}
		i++;
	}
	return (pt);
}
