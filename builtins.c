#include "minishell.h"

void		builtin_cd(t_minishell *msh)
{
	char	*buf;
	char	*tmp;

	buf = NULL;
	msh->argc = get_argc(msh->args);
	if (msh->argc > 2)
		ft_printf("cd: usage: cd [optional argument]\n");
	else if (!msh->args[1])
	{
		msh->home = get_diclst_val(msh, "HOME", 0)->value;
		if (chdir(msh->home))
			ft_dprintf(2, "Error chdir\n");
		else
		{
			tmp = get_diclst_val(msh, "PWD", 0)->value;
			free(get_diclst_val(msh, "OLDPWD", 0)->value);
			get_diclst_val(msh, "OLDPWD", 0)->value = ft_strdup(tmp);
			free(tmp);
			tmp = NULL;
			get_diclst_val(msh, "PWD", 0)->value = ft_strdup(msh->home);
		}
	}
	else if (!ft_strcmp(msh->args[1], "-"))
	{
		msh->oldpwd = get_diclst_val(msh, "OLDPWD", 0)->value;
		msh->pwd = get_diclst_val(msh, "PWD", 0)->value;
		if (chdir(msh->oldpwd))
			ft_dprintf(2, "Error chdir\n");
		else
		{
			msh->oldpwd = home_to_tild(msh, msh->oldpwd);
			ft_printf("%s\n", msh->oldpwd);
			free(get_diclst_val(msh, "OLDPWD", 0)->value);
			get_diclst_val(msh, "OLDPWD", 0)->value = ft_strdup(msh->pwd);
			free(get_diclst_val(msh, "PWD", 0)->value);
			get_diclst_val(msh, "PWD", 0)->value = getcwd(buf, CWD_BUF_SIZE);
		}
	}
	else
	{
		if (chdir(msh->args[1]))
			ft_dprintf(2, "Error chdir\n");
		else
		{
			tmp = get_diclst_val(msh, "PWD", 0)->value;
			free(get_diclst_val(msh, "OLDPWD", 0)->value);
			get_diclst_val(msh, "OLDPWD", 0)->value = ft_strdup(tmp);
			free(tmp);
			tmp = NULL;
			get_diclst_val(msh, "PWD", 0)->value = getcwd(buf, CWD_BUF_SIZE);
		}
	}
}

void		builtin_exit(t_minishell *msh)
{
	int		argc;

	argc = get_argc(msh->args);
	if (argc > 2)
	{
		ft_printf("exit: too many arguments\n");
		return ;
	}
	if (!msh->args[1])
		exit(EXIT_SUCCESS);
	exit(ft_atoi(msh->args[1]));
}

void	builtin_env(t_minishell *msh)
{
	t_diclst	*env_lst;
	int			argc;

	argc = get_argc(msh->args);
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

void	builtin_setenv(t_minishell *msh)
{
	t_diclst	*env_lst;
	char		**args;
	int			argc;


	if ((argc = get_argc(msh->args)) != 2 && argc != 3)
	{
		ft_printf("setenv: usage: setenv [NAME] [OPTIONAL VALUE]\n");
		return ;
	}
	args = msh->args;
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
	add_diclst(&msh->env_lst, ++args); 
}

void	ft_unsetenv(t_minishell **msh, t_diclst **node, t_diclst **prev)
{
	t_diclst	*tmp;

	if ((*msh)->env_lst == *node)
	{
		tmp = *node;
		*node = (*node)->next;
		(*msh)->env_lst = *node;
		free(tmp);
	}
	else
	{
		(*prev)->next = (*node)->next;
		free(*node);
	}
}

void	builtin_unsetenv(t_minishell *msh)
{
	t_diclst	*env_lst;
	t_diclst	*prev;
	int			argc;

	if ((argc = get_argc(msh->args)) != 2)
	{
		ft_printf("unsetenv: usage: unsetenv [NAME]\n");
		return ;
	}
	env_lst = msh->env_lst;
	prev = env_lst;
	while (env_lst)
	{
		if (!strcmp(msh->args[1], env_lst->name))
			ft_unsetenv(&msh, &env_lst, &prev);
		prev = env_lst;
		env_lst = env_lst->next;
	}
}

void	builtin_echo(t_minishell *msh)
{
	char	**args;
	int		argc;
	int		i;

	i = 1;
	argc = get_argc(msh->args);
	args = msh->args;
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
