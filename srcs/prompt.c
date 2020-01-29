/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:47:34 by ishaimou          #+#    #+#             */
/*   Updated: 2020/01/29 02:45:53 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char			*home_to_tild(t_minishell *msh, char *str)
{
	char		*tmp;
	int			len;
	char		*new_str;
	t_diclst	*node;

	new_str = ft_strdup(str);
	node = get_diclst_val(msh, "HOME", 0);
	if (node)
		msh->home = node->value;
	else
		msh->home = "";
	if (msh->home[0] && ft_strstr(str, msh->home))
	{
		len = ft_strlen(msh->home);
		free(new_str);
		new_str = ft_strdup(str + len);
		tmp = new_str;
		new_str = ft_strjoin("~", new_str);
		ft_strdel(&tmp);
	}
	return (new_str);
}

void			prompt_dir(t_minishell *msh)
{
	char		*buf;
	char		*cwd;
	char		*cwd_tild;
	t_diclst	*node;

	buf = NULL;
	node = get_diclst_val(msh, "USER", 0);
	if (node)
		msh->user = node->value;
	else
		msh->user = "";
	cwd = getcwd(buf, CWD_BUF_SIZE);
	cwd_tild = home_to_tild(msh, cwd);
	if (msh->user)
	{
		ft_putstr("\033[1;32m");
		ft_putstr(msh->user);
		ft_putstr("\033[m:");
	}
	ft_putstr("\033[1;36m");
	ft_putstr(cwd_tild);
	ft_putstr("\033[m ");
	ft_putstr("$> ");
	ft_strdel(&cwd);
	ft_strdel(&cwd_tild);
}
