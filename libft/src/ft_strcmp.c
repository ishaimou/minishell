/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:36:27 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:36:28 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_strcmp(const char *s1, const char *s2)
{
	int				i;
	int				res;
	unsigned char	*ps1;
	unsigned char	*ps2;

	i = 0;
	res = 0;
	ps1 = (unsigned char*)s1;
	ps2 = (unsigned char*)s2;
	while (ps1[i] != '\0' || ps2[i] != '\0')
	{
		if (ps1[i] != ps2[i])
		{
			res = (ps1[i] - ps2[i]);
			break ;
		}
		i++;
	}
	return ((int)res);
}
