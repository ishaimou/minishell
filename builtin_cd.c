#include "minishell.h"

static int	check_path(t_minishell *msh, int ind)
{
	struct stat	file_info;

	if (!stat(msh->args[ind + 1], &file_info))
	{
		if (!S_ISDIR(file_info.st_mode))
		{
			ft_dprintf(2, "cd: not a directory: %s\n", msh->args[ind + 1]);
			return (1);
		}
		if (access(msh->args[ind + 1], X_OK))
		{
			ft_dprintf(2, "cd: permission denied: %s\n", msh->args[ind + 1]);
			return (1);
		}
	}
	return (0);
}

static int		builtin_cd_path(t_minishell *msh, int ind)
{
	char		*buf;
	char		*tmp;

	buf = NULL;
	if (check_path(msh, ind))
		return (1);
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
	return (0);
}

static void		builtin_cd_minus(t_minishell *msh)
{
	char	*buf;
	char	*tmp;

	buf = NULL;
	chdir(msh->oldpwd);
	tmp = home_to_tild(msh, msh->oldpwd);
	ft_printf("%s\n", tmp);
	free(tmp);
	free(get_diclst_val(msh, "OLDPWD", 0)->value);
	get_diclst_val(msh, "OLDPWD", 0)->value = ft_strdup(msh->pwd);
	free(get_diclst_val(msh, "PWD", 0)->value);
	get_diclst_val(msh, "PWD", 0)->value = getcwd(buf, CWD_BUF_SIZE);
}

static void		builtin_cd_set(t_minishell *msh)
{
	t_diclst	*node;
	char		*cwd;
	char		*buf;

	buf = NULL;
	cwd = getcwd(buf, CWD_BUF_SIZE);
	node = get_diclst_val(msh, "OLDPWD", 0);
	if (node)
		msh->oldpwd = node->value;
	else
	{
		add_diclst(msh, &msh->env_lst, "OLDPWD", cwd);
		msh->oldpwd = get_diclst_val(msh, "OLDPWD", 0)->value;
	}
	node = get_diclst_val(msh, "PWD", 0);
	if (node)
		msh->pwd = node->value;
	else
	{
		add_diclst(msh, &msh->env_lst, "PWD", cwd);
		msh->pwd = get_diclst_val(msh, "PWD", 0)->value;
	}
	free(cwd);
}

void		builtin_cd(t_minishell *msh, int ind)
{
	char	*buf;

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
	builtin_cd_set(msh);
	if (!ft_strlen(msh->args[ind + 1]))
		return ;
	if (!ft_strcmp(msh->args[ind + 1], "-"))
		builtin_cd_minus(msh);
	else
	{
		if (builtin_cd_path(msh, ind))
			return ;
	}
}
