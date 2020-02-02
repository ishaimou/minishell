/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:47:51 by ishaimou          #+#    #+#             */
/*   Updated: 2020/02/01 04:44:24 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	quotes(t_minishell *msh, char *line, int i)
{
	if (line[i] == '\'' && !msh->qflag && !msh->dquote)
	{
		msh->quote++;
		msh->qflag = 1;
		msh->sflag = 1;
	}
	else if (line[i] == '\'' && msh->qflag)
	{
		msh->quote--;
		msh->qflag = 0;
	}
	else if (line[i] == '\"' && !msh->dflag && !msh->quote)
	{
		msh->dquote++;
		msh->dflag = 1;
		msh->sflag = 1;
	}
	else if (line[i] == '\"' && msh->dflag)
	{
		msh->dquote--;
		msh->dflag = 0;
	}
}

static int	handle_quotes(t_minishell *msh, char *line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		quotes(msh, line, i);
		i++;
	}
	if (msh->quote)
		return (1);
	else if (msh->dquote)
		return (2);
	else
		return (0);
}

static void	read_line(t_minishell *msh, int *q)
{
	char	*line;
	char	*tmp;

	line = NULL;
	if (get_next_line(STDIN, &line) < 0)
		gnl_error();
	*q = handle_quotes(msh, line);
	if (msh->quoted)
	{
		tmp = msh->line;
		msh->line = ft_strjoin_sep(msh->line, line, "\n");
		ft_strdel(&tmp);
		ft_strdel(&line);
	}
	else
		msh->line = line;
}

void		read_cmd(t_minishell *msh)
{
	int		q;

	read_line(msh, &q);
	msh->quoted = q;
	if (!q)
		return ;
	if (q == 1)
		ft_putstr("quote> ");
	else if (q == 2)
		ft_putstr("dquote> ");
	read_cmd(msh);
}
