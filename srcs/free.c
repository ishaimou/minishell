/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:47:15 by ishaimou          #+#    #+#             */
/*   Updated: 2020/02/02 11:16:42 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			free_dbl(char ***tab)
{
	int			i;

	i = 0;
	if (tab && *tab)
	{
		while ((*tab)[i])
		{
			free((*tab)[i]);
			(*tab)[i] = NULL;
			i++;
		}
		free(*tab);
		*tab = NULL;
	}
}

void			free_diclst(t_diclst **begin)
{
	t_diclst	*curr;
	t_diclst	*next;

	if (begin && *begin)
	{
		curr = *begin;
		while (curr)
		{
			next = curr->next;
			ft_strdel(&curr->name);
			ft_strdel(&curr->value);
			free(curr);
			curr = next;
		}
		*begin = NULL;
	}
}

void			free_msh(t_minishell *msh)
{
	ft_strdel(&msh->line);
	ft_strdel(&msh->cmd_path);
	ft_strdel(&msh->alias_path);
	ft_strdel(&msh->alias_path_cp);
	free_dbl(&msh->args);
	free_dbl(&msh->cmds);
	free_dbl(&msh->env);
	free(msh->funct_tab);
	free_dbl(&msh->builtin_name);
	free_diclst(&msh->env_lst);
	free_diclst(&msh->var_lst);
}
