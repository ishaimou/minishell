#include "minishell.h"

static void	set_builtin(t_minishell *msh)
{
	msh->builtin_name = (char**)malloc(sizeof(char*) * (BUILTIN_NUM + 1));
	msh->builtin_name[BUILTIN_NUM] = NULL;
	msh->builtin_name[0] = ft_strdup("cd");
	msh->builtin_name[1] = ft_strdup("exit");
	msh->builtin_name[2] = ft_strdup("env");
	msh->builtin_name[3] = ft_strdup("setenv");
	msh->builtin_name[4] = ft_strdup("unsetenv");
	msh->builtin_name[5] = ft_strdup("echo");
	msh->funct_tab = (builtin_func*)malloc(sizeof(builtin_func) * (BUILTIN_NUM + 1));
	msh->funct_tab[BUILTIN_NUM] = NULL;
	msh->funct_tab[0] = &builtin_cd;
	msh->funct_tab[1] = &builtin_exit;
	msh->funct_tab[2] = &builtin_env;
	msh->funct_tab[3] = &builtin_setenv;
	msh->funct_tab[4] = &builtin_unsetenv;
	msh->funct_tab[5] = &builtin_echo;
}

static void	set_oldpwd(t_minishell *msh)
{
	t_diclst	*node;

	node = get_diclst_val(msh, "OLDPWD", 0);
	msh->pwd = get_diclst_val(msh, "PWD", 0)->value;
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(msh->pwd);
		msh->oldpwd = get_diclst_val(msh, "OLDPWD", 0)->value;
	}
}

void	init_qparam(t_minishell *msh)
{
	msh->quoted = 0;
	msh->quote = 0;
	msh->dquote = 0;
	msh->qflag = 0;
	msh->dflag = 0;
}

void	init_msh(t_minishell *msh, int ac, char **av, char **env)
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
