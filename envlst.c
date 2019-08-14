#include "minishell.h"

t_envlst	*get_envlst_val(t_minishell *msh, char *name)
{
	t_envlst	*tmp;
	
	tmp = msh->env_lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	add_envlst(t_envlst **env_lst, char **tab_env)
{
	t_envlst	*node;
	t_envlst	*tmp;

	node = (t_envlst*)malloc(sizeof(t_envlst));
	node->next = NULL;
	node->name = ft_strdup(tab_env[0]);
	node->value = ft_strdup(tab_env[1]);
	if (!*env_lst)
		*env_lst = node;
	else
	{
		tmp = *env_lst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

int		envlst_size(t_envlst *begin_list)
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
			add_envlst(&msh->env_lst, tab_env);
		free_dbl(&tab_env);
		env++;
	}
}

char	**set_env(t_minishell *msh)
{
	t_envlst	*env_lst;
	char		**environ;
	char		*str;
	int			i;
	
	i = 0;
	env_lst = msh->env_lst;
	environ = (char**)malloc(sizeof(char*) * (envlst_size(msh->env_lst) + 1));
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
