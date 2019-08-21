#include "minishell.h"

void		realloc_args(t_minishell *msh)
{
	t_diclst	*node;

	free_dbl(&msh->args);
	msh->args = (char**)malloc(sizeof(char*) * 3);
	msh->args[0] = ft_strdup("cd");
	node = get_diclst_val(msh, "HOME", 0);
	if (node)
		msh->home = node->value;
	else
		msh->home = "";
	msh->args[1] = ft_strdup(msh->home);
	msh->args[2] = NULL;
}

void		builtin_cd(t_minishell *msh, int ind)
{
	char	*buf;
	char	*tmp;
	char	*cwd;
	t_diclst	*node;

	buf = NULL;
	msh->argc = get_argc(msh->args + ind);
	if (msh->argc > 2)
	{
		ft_printf("cd: usage: cd [optional argument]\n");
		return ;
	}
	if (!msh->args[ind + 1])
	{
		realloc_args(msh);
		ind = 0;
	}
	cwd = getcwd(buf, CWD_BUF_SIZE);
	node = get_diclst_val(msh, "OLDPWD", 0);
	if (node)
		msh->oldpwd = node->value;
	else
		add_diclst(&msh->env_lst, "OLDPWD", cwd);
	node = get_diclst_val(msh, "PWD", 0);
	if (node)
		msh->pwd = node->value;
	else
		add_diclst(&msh->env_lst, "PWD", cwd);
	free(cwd);
	if (!ft_strlen(msh->args[ind + 1]))
		return ;
	if (!ft_strcmp(msh->args[ind + 1], "-"))
	{
		chdir(msh->oldpwd);
		tmp = home_to_tild(msh, msh->oldpwd);
		ft_printf("%s\n", tmp);
		free(tmp);
		free(get_diclst_val(msh, "OLDPWD", 0)->value);
		get_diclst_val(msh, "OLDPWD", 0)->value = ft_strdup(msh->pwd);
		free(get_diclst_val(msh, "PWD", 0)->value);
		buf = NULL;
		get_diclst_val(msh, "PWD", 0)->value = getcwd(buf, CWD_BUF_SIZE);
	}
	else
	{
		if (chdir(msh->args[ind + 1]))
			ft_dprintf(2, "cd: no such file or directory: %s\n", msh->args[ind + 1]);
		else
		{
			tmp = get_diclst_val(msh, "PWD", 0)->value;
			free(get_diclst_val(msh, "OLDPWD", 0)->value);
			get_diclst_val(msh, "OLDPWD", 0)->value = ft_strdup(tmp);
			free(tmp);
			buf = NULL;
			get_diclst_val(msh, "PWD", 0)->value = getcwd(buf, CWD_BUF_SIZE);
		}
	}
}

void		builtin_exit(t_minishell *msh, int ind)
{
	int		argc;
	int		ret;
	char	**args;

	args = msh->args + ind;
	argc = get_argc(args);
	if (argc > 2)
	{
		ft_printf("exit: too many arguments\n");
		return ;
	}
	ret = args[1] ? ft_atoi(args[1]) : EXIT_SUCCESS;
	free_msh(msh);
	free_dbl(&msh->builtin_name);
	free_diclst(&msh->env_lst);
	free_diclst(&msh->var_lst);
	free(msh->funct_tab);
	exit(ret);
}

void	builtin_env(t_minishell *msh, int ind)
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

void	builtin_setenv(t_minishell *msh, int ind)
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
	env_lst = msh->env_lst;
	while (env_lst)
	{
		if (!ft_strcmp(args[1], env_lst->name))
		{	
			free(env_lst->value);
			if (args[2])
				env_lst->value = ft_strdup(args[2]);
			else
				env_lst->value = ft_strnew(0);
			return ;
		}
		env_lst = env_lst->next;
	}
	add_diclst(&msh->env_lst, args[1], args[2]); 
}

void	ft_unsetenv(t_minishell **msh, t_diclst **node, t_diclst **prev)
{
	t_diclst	*tmp;

	if ((*msh)->env_lst == *node)
	{
		tmp = *node;
		*node = (*node)->next;
		(*msh)->env_lst = *node;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
	else
	{
		(*prev)->next = (*node)->next;
		free((*node)->name);
		free((*node)->value);
		free(*node);
	}
}

void	builtin_unsetenv(t_minishell *msh, int ind)
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
	env_lst = msh->env_lst;
	prev = env_lst;
	while (env_lst)
	{
		if (!strcmp(args[1], env_lst->name))
			ft_unsetenv(&msh, &env_lst, &prev);
		prev = env_lst;
		env_lst = env_lst->next;
	}
}

void	builtin_echo(t_minishell *msh, int ind)
{
	char	**args;
	int		argc;
	int		i;

	i = 1;
	args = msh->args + ind;
	argc = get_argc(args);
	args++;
	while (*args)
	{
		i++;
		ft_printf("%s", *args);
		if (i != argc)
			ft_putchar(' ');
		args++;
	}
	ft_putchar('\n');
}
