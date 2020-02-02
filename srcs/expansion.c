/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:46:57 by ishaimou          #+#    #+#             */
/*   Updated: 2020/02/02 07:18:16 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		is_sep(char c)
{
	return (c == ':' || c == '/' || c == '#' || c == '-'
			|| c == '{' || c == '}' || c == '+');
}

static int		check_arg(char *arg, int *i)
{
	if (arg[0] == '=')
		return (0);
	while (arg[*i] && arg[*i] != '=' && ft_isprint(arg[*i]))
		(*i)++;
	if (arg[*i] != '=')
		return (0);
	return (1);
}

int				set_varlst(t_minishell *msh, char *arg)
{
	t_diclst	*node;
	char		**tab_var;
	int			i;

	i = 0;
	if (!check_arg(arg, &i))
		return (0);
	if (!(tab_var = (char**)malloc(sizeof(char*) * 3)))
		malloc_error(msh);
	tab_var[0] = ft_strndup(arg, i);
	tab_var[1] = ft_strdup(arg + i + 1);
	tab_var[2] = NULL;
	node = get_diclst_val(msh, tab_var[0], VAR_LST);
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(tab_var[1]);
	}
	else
		add_diclst(msh, &msh->var_lst, tab_var[0], tab_var[1]);
	free_dbl(&tab_var);
	return (1);
}

static void		get_value(t_minishell *msh, char **tab_dollar, char **str)
{
	t_diclst	*node;
	char		*tmp;
	int			len;
	int			i;
	int			j;
	char		*var_key;

	len = get_argc(tab_dollar);
	i = -1;
	while (++i < len)
	{
		j = 0;
		while (tab_dollar[i][j] && !is_sep(tab_dollar[i][j]))
			j++;
		if (tab_dollar[i][j])
			var_key = ft_strndup(tab_dollar[i], j);
		else
			var_key = ft_strdup(tab_dollar[i]);
		if ((node = get_diclst_val(msh, var_key, VAR_LST)) ||
				(node = get_diclst_val(msh, var_key, ENV_LST)))
		{
			tmp = *str;
			*str = ft_strjoin_sep(*str, tab_dollar[i] + j, node->value);
			free(tmp);
		}
		else
		{
			tmp = *str;
			*str = ft_strjoin(*str, tab_dollar[i] + j);
			free(tmp);
		}
		ft_strdel(&var_key);
	}
}

static void		get_recursive_val(t_minishell *msh, char **arg, char *ptr)
{
	char		**tab_dollar;
	char		*str;
	int			i;

	if (*ptr != '$')
		return ;
	else if (*(ptr + 1) == '$')
		get_recursive_val(msh, arg, ptr + 1);
	else
	{
		i = ptr - *arg;
		str = ft_strndup(*arg, i);
		tab_dollar = ft_strsplit(ptr, '$');
		if (*tab_dollar)
			get_value(msh, tab_dollar, &str);
		free_dbl(&tab_dollar);
		free(*arg);
		*arg = str;
	}
}

void			handle_exp(t_minishell *msh)
{
	t_diclst	*node;
	char		*ptr;
	char		*tmp;
	int			i;

	i = 0;
	node = get_diclst_val(msh, "HOME", ENV_LST);
	if (node)
		msh->home = node->value;
	while (msh->args[i])
	{
		while ((ptr = ft_strchr(msh->args[i], '$')))
			get_recursive_val(msh, &msh->args[i], ptr);
		if (msh->args[i][0] == '~')
		{
			tmp = msh->args[i];
			msh->args[i] = ft_strjoin(msh->home, msh->args[i] + 1);
			free(tmp);
		}
		i++;
	}
}
