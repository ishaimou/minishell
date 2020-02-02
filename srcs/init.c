/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:47:20 by ishaimou          #+#    #+#             */
/*   Updated: 2020/02/02 04:04:22 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		set_builtin_funct(t_minishell *msh)
{
	if (!(msh->funct_tab = (t_builtin_func*)malloc(sizeof(t_builtin_func)
		* (BUILTIN_NUM + 1))))
		malloc_error(msh);
	msh->funct_tab[BUILTIN_NUM] = NULL;
	msh->funct_tab[0] = &builtin_cd;
	msh->funct_tab[1] = &builtin_exit;
	msh->funct_tab[2] = &builtin_env;
	msh->funct_tab[3] = &builtin_setenv;
	msh->funct_tab[4] = &builtin_unsetenv;
	msh->funct_tab[5] = &builtin_echo;
	msh->funct_tab[6] = &builtin_source;
	msh->funct_tab[7] = &builtin_alias;
	msh->funct_tab[8] = &builtin_help;
}

static void		set_builtin(t_minishell *msh)
{
	if (!(msh->builtin_name = (char**)malloc(sizeof(char*)
		* (BUILTIN_NUM + 1))))
		malloc_error(msh);
	msh->builtin_name[BUILTIN_NUM] = NULL;
	msh->builtin_name[0] = ft_strdup("cd");
	msh->builtin_name[1] = ft_strdup("exit");
	msh->builtin_name[2] = ft_strdup("env");
	msh->builtin_name[3] = ft_strdup("setenv");
	msh->builtin_name[4] = ft_strdup("unsetenv");
	msh->builtin_name[5] = ft_strdup("echo");
	msh->builtin_name[6] = ft_strdup("source");
	msh->builtin_name[7] = ft_strdup("alias");
	msh->builtin_name[8] = ft_strdup("help");
	set_builtin_funct(msh);
}

static void		set_oldpwd(t_minishell *msh)
{
	t_diclst	*node;
	char		*buf;

	buf = NULL;
	msh->pwd = getcwd(buf, CWD_BUF_SIZE);
	node = get_diclst_val(msh, "PWD", ENV_LST);
	if (node)
	{
		free(node->value);
		node->value = msh->pwd ? ft_strdup(msh->pwd) : ft_strnew(0);
	}
	else
		add_diclst(msh, &msh->env_lst, "PWD", msh->pwd);
	node = get_diclst_val(msh, "OLDPWD", ENV_LST);
	if (node)
	{
		free(node->value);
		node->value = msh->pwd ? ft_strdup(msh->pwd) : ft_strnew(0);
	}
	else
		add_diclst(msh, &msh->env_lst, "OLDPWD", msh->pwd);
	free(msh->pwd);
}

void			init_qparam(t_minishell *msh)
{
	msh->quoted = 0;
	msh->quote = 0;
	msh->dquote = 0;
	msh->qflag = 0;
	msh->dflag = 0;
}

void			init_msh(t_minishell *msh, int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	msh->line = NULL;
	msh->cmds = NULL;
	msh->cmd_path = NULL;
	msh->args = NULL;
	msh->argc = 0;
	msh->sflag = 0;
	msh->user = NULL;
	msh->home = NULL;
	msh->pwd = NULL;
	msh->oldpwd = NULL;
	msh->env_lst = NULL;
	msh->var_lst = NULL;
	msh->env = NULL;
	init_qparam(msh);
	set_builtin(msh);
	set_envlst(msh, env);
	set_oldpwd(msh);
}
