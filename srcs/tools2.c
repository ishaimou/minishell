/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 11:14:19 by ishaimou          #+#    #+#             */
/*   Updated: 2020/02/02 11:16:40 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_strjoin_n_free(char **str1, char *str2, char *sep)
{
	char		*tmp;

	tmp = *str1;
	if (sep)
		*str1 = ft_strjoin_sep(*str1, str2, sep);
	else
		*str1 = ft_strjoin(*str1, str2);
	free(tmp);
}

void	split_n_join(t_minishell *msh)
{
	char	*str;
	char	*tmp;
	int		i;

	i = 0;
	str = ft_strnew(0);
	while (msh->args[i])
	{
		tmp = str;
		str = ft_strjoin_sep(str, msh->args[i], " ");
		free(tmp);
		i++;
	}
	free_dbl(&msh->args);
	msh->args = ft_split_whitespaces(str);
	ft_strdel(&str);
}
