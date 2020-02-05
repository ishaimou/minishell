/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:46:33 by ishaimou          #+#    #+#             */
/*   Updated: 2020/02/03 06:58:01 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		env_error(char **args)
{
	if (args[0] && args[1] && args[1][0] == '\0')
	{
		ft_dprintf(2, "%s: error: name must not be empty string\n", args[0]);
		return (1);
	}
	return (0);
}

void			builtin_env(t_minishell *msh, int ind)
{
	t_diclst	*env_lst;
	int			argc;

	argc = get_argc(msh->args + ind);
	if (argc > 1)
	{
		ft_printf("env: %s: No such file or directory\n", msh->args[1]);
		return ;
	}
	env_lst = msh->env_lst;
	while (env_lst)
	{
		ft_printf("%s=%s\n", env_lst->name, env_lst->value);
		env_lst = env_lst->next;
	}
}

void			builtin_setenv(t_minishell *msh, int ind)
{
	t_diclst	*env_lst;
	char		**args;
	int			argc;

	args = msh->args + ind;
	if ((argc = get_argc(args)) != 2 && argc != 3)
	{
		ft_printf("setenv: usage: setenv [NAME] [OPTIONAL VALUE]\n");
		return ;
	}
	if (env_error(args))
		return ;
	env_lst = msh->env_lst;
	while (env_lst)
	{
		if (!ft_strcmp(args[1], env_lst->name))
		{
			free(env_lst->value);
			env_lst->value = args[2] ? ft_strdup(args[2]) : ft_strnew(0);
			return ;
		}
		env_lst = env_lst->next;
	}
	add_diclst(msh, &msh->env_lst, args[1], args[2]);
}

static void		ft_unsetenv(t_minishell **msh, t_diclst **node, t_diclst **prev)
{
	if ((*msh)->env_lst == *node)
	{
		(*msh)->env_lst = (*node)->next;
		ft_strdel(&(*node)->name);
		ft_strdel(&(*node)->value);
		free(*node);
		*node = NULL;
	}
	else
	{
		(*prev)->next = (*node)->next;
		ft_strdel(&(*node)->name);
		ft_strdel(&(*node)->value);
		free(*node);
		*node = NULL;
	}
}

void			builtin_unsetenv(t_minishell *msh, int ind)
{
	t_diclst	*env_lst;
	t_diclst	*prev;
	int			argc;
	char		**args;

	args = msh->args + ind;
	if ((argc = get_argc(args)) != 2)
	{
		ft_printf("unsetenv: usage: unsetenv [NAME]\n");
		return ;
	}
	if (env_error(args))
		return ;
	env_lst = msh->env_lst;
	prev = env_lst;
	while (env_lst)
	{
		if (!ft_strcmp(args[1], env_lst->name))
		{
			ft_unsetenv(&msh, &env_lst, &prev);
			break ;
		}
		prev = env_lst;
		env_lst = env_lst->next;
	}
}
