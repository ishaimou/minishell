#include "minishell.h"

t_diclst	*get_diclst_val(t_minishell *msh, char *name, int type)
{
	t_diclst	*tmp;

	if (!type)
		tmp = msh->env_lst;
	else
		tmp = msh->var_lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	add_diclst(t_diclst **dic_lst, char **tab)
{
	t_diclst	*node;
	t_diclst	*tmp;

	node = (t_diclst*)malloc(sizeof(t_diclst));
	node->next = NULL;
	node->name = ft_strdup(tab[0]);
	if (tab[1])
		node->value = ft_strdup(tab[1]);
	else
		node->value = ft_strnew(0);
	if (!*dic_lst)
		*dic_lst = node;
	else
	{
		tmp = *dic_lst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

int		diclst_size(t_diclst *begin_list)
{
	int		i;

	i = 0;
	while (begin_list)
	{
		begin_list = begin_list->next;
		i++;
	}
	return (i);
}

void	set_envlst(t_minishell *msh, char **env)
{
	char	**tab_env;

	tab_env = NULL;
	while (*env)
	{
		tab_env = ft_strsplit(*env, '=');
		if (*tab_env)
			add_diclst(&msh->env_lst, tab_env);
		free_dbl(&tab_env);
		env++;
	}
}

char	**set_env(t_minishell *msh)
{
	t_diclst	*env_lst;
	char		**environ;
	char		*str;
	int			i;
	
	i = 0;
	env_lst = msh->env_lst;
	environ = (char**)malloc(sizeof(char*) * (diclst_size(msh->env_lst) + 1));
	while (env_lst)
	{
		str = ft_strjoin_sep(env_lst->name, env_lst->value, "=");
		environ[i] = ft_strdup(str);
		free(str);
		env_lst = env_lst->next;
		i++;
	}
	environ[i] = NULL;
	return (environ);
}
