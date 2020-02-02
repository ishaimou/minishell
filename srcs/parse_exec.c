/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:47:29 by ishaimou          #+#    #+#             */
/*   Updated: 2020/02/02 09:28:46 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void			get_cmd_path(t_minishell *msh, char **paths)
{
	int				i;
	DIR				*dirp;
	struct dirent	*res;

	i = 0;
	while (paths[i])
	{
		if (!(dirp = opendir(paths[i])) && ++i)
			continue ;
		while ((res = readdir(dirp)))
		{
			if (!ft_strcmp(res->d_name, msh->cmd_path))
			{
				rm_trailing_slash(&paths[i]);
				ft_strdel(&msh->cmd_path);
				msh->cmd_path = ft_strjoin_sep(paths[i], res->d_name, "/");
				(void)closedir(dirp);
				return ;
			}
		}
		(void)closedir(dirp);
		i++;
	}
}

static void			find_path(t_minishell *msh, int i)
{
	char			**path_env;
	char			**paths;

	path_env = NULL;
	paths = NULL;
	path_env = ft_strsplit(msh->env[i], '=');
	if (*path_env)
	{
		paths = ft_strsplit(*(path_env + 1), ':');
		free_dbl(&path_env);
	}
	if (paths && *paths)
	{
		get_cmd_path(msh, paths);
		free_dbl(&paths);
	}
}

static int			find_path_cmd(t_minishell *msh, int ind)
{
	int				i;

	i = 0;
	msh->env = set_env(msh);
	msh->cmd_path = ft_strdup(msh->args[ind]);
	if (ft_strchr(msh->cmd_path, '/'))
		return (check_err(msh, 0));
	while (msh->env[i] && !ft_strstr(msh->env[i], "PATH"))
		i++;
	if (!msh->env[i])
		return (0);
	find_path(msh, i);
	return (0);
}

static void			exec_cmd(t_minishell *msh)
{
	int				pos;
	int				ind;

	ind = 0;
	while (fetch_alias(msh))
		split_n_join(msh);
	while (msh->args[ind] && ft_strchr(msh->args[ind], '=')
			&& set_varlst(msh, msh->args[ind]))
		ind++;
	handle_exp(msh);
	if (!msh->args[ind] || !msh->args[ind][0])
		return ;
	if ((pos = is_builtin(msh, msh->args[ind])) != -1)
		(msh->funct_tab[pos])(msh, ind);
	else
	{
		if (find_path_cmd(msh, ind))
		{
			free_dbl(&msh->env);
			return ;
		}
		launch_cmd(msh, ind);
	}
}

void				parse_exec_cmd(t_minishell *msh)
{
	int				i;

	i = 0;
	if (msh->sflag && !ft_strstr(msh->line, "alias"))
		simplify_cmd(&msh->line);
	msh->cmds = ft_strsplit(msh->line, ';');
	if (!msh->cmds[0])
		return ;
	while (msh->cmds[i])
	{
		msh->argc = 0;
		msh->cmd_path = NULL;
		if (!(msh->args = split_alias(msh)))
			msh->args = ft_split_whitespaces(msh->cmds[i]);
		if (*msh->args)
			exec_cmd(msh);
		free_dbl(&msh->args);
		ft_strdel(&msh->cmd_path);
		i++;
	}
}
