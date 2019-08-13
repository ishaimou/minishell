#include "minishell.h"

void	print_envlst(t_envlst	*env_lst)
{
	while (env_lst)
	{
		ft_printf("%s=%s\n", env_lst->name, env_lst->value);
		env_lst = env_lst->next;
	}
}

void    free_dbl(char ***tab)
{
	int		i;

	i = 0;
	while ((*tab)[i])
	{
		free((*tab)[i]);
		(*tab)[i] = NULL;
		i++;
	}
	free(*tab);
	*tab = NULL;
}

void	free_msh(t_minishell *msh)
{
	free(msh->line);
	msh->line = NULL;
	free_dbl(&msh->args);
}

void	print_2d(char **s)
{
	while (*s)
		printf("%s\n", *s++);
}

char	*ft_strjoin_sep(const char *s1, const char *s2, char *sep)
{
	char	*strsep;
	char	*str;

	strsep = NULL;
	str = NULL;
	strsep = ft_strjoin(s1, sep);
	str = ft_strjoin(strsep, s2);
	return (str);
}

void	read_cmd(t_minishell *msh)
{
	char	*line;

	if (get_next_line(0, &line) < 0)
	{
		ft_dprintf(2, "Error gnl\n");
		free(line);
		line = NULL;
		exit(EXIT_FAILURE);
	}
	msh->line = line;
}

void	get_cmd_path(t_minishell *msh, char **paths)
{
	int		i;
	DIR		*dirp;
	struct dirent	*res;

	i = 0;
	while (paths[i])
	{
		if (!(dirp = opendir(paths[i])))
			ft_dprintf(2, "opendir error\n");
		while ((res = readdir(dirp)))
		{
			if (!ft_strcmp(res->d_name, msh->args[0]))
			{
				msh->cmd_path = ft_strjoin_sep(paths[i], res->d_name, "/");
				break ;
			}
		}
		if (msh->cmd_path)
			break;
		(void)closedir(dirp);
		i++;
	}
}

void	find_path_cmd(t_minishell *msh)
{
	char			**path_env;
	char			**paths;
	int				i;

	path_env = NULL;
	paths = NULL;
	i = 0;
	while (msh->env[i] && !ft_strstr(msh->env[i], "PATH"))
		i++;
	if (msh->env[i])
	{
		path_env = ft_strsplit(msh->env[i], '=');
		if (*path_env)
			paths = ft_strsplit(*(path_env + 1), ':');
	}
	if (*paths)
		get_cmd_path(msh, paths);
}

void	launch_cmd(t_minishell *msh)
{
	pid_t	cpid;
	pid_t	wpid;
	int		stat_loc;

	if ((cpid = fork()) < 0)
	{
		ft_dprintf(2, "Error fork\n");
		free_msh(msh);
		exit(EXIT_FAILURE);
	}
	else if (cpid == 0)
	{
		if (execve(msh->cmd_path, msh->args, msh->env) < 0)
		{
			ft_dprintf(2, "Error cmd\n");
			free_msh(msh);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if ((wpid = waitpid(cpid, &stat_loc, WUNTRACED)) < 0)
		{
			ft_dprintf(2, "Error waitpid\n");
			exit(EXIT_FAILURE);
		}
	}
	msh->cmd_path = NULL;
}

void	sigint_handler(int signo)
{
	if (!jmp_flag)
		return ;
	siglongjmp(sig_env, 1337);
}

void	set_builtin(t_minishell *msh)
{
	msh->builtin_name = (char**)malloc(sizeof(char*) * (BUILTIN_NUM + 1));
	msh->builtin_name[BUILTIN_NUM] = NULL;
	msh->builtin_name[0] = ft_strdup("cd");
	msh->builtin_name[1] = ft_strdup("exit");
	msh->funct_tab = (builtin_func*)malloc(sizeof(builtin_func) * (BUILTIN_NUM + 1));
	msh->funct_tab[BUILTIN_NUM] = NULL;
	msh->funct_tab[0] = &builtin_cd;
	msh->funct_tab[1] = &builtin_exit;
}

int		get_argc(char **args)
{
	int		argc;

	argc = 0;
	while (*args)
	{
		argc++;
		args++;
	}
	return (argc);
}

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

void	set_oldpwd(t_minishell *msh)
{
	t_envlst	*node;

	node = get_envlst_val(msh, "OLDPWD");
	msh->pwd = get_envlst_val(msh, "PWD")->value;
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(msh->pwd);
	}
}

void		builtin_cd(t_minishell *msh)
{
	char	*buf;

	buf = NULL;
	msh->argc = get_argc(msh->args);
	if (msh->argc > 2)
		ft_printf("usage: cd [optional argument]\n");
	else if (!msh->args[1])
	{
		msh->home = get_envlst_val(msh, "HOME")->value;
		if (chdir(msh->home))
			ft_dprintf(2, "Error chdir\n");
		get_envlst_val(msh, "OLDPWD")->value = msh->pwd;
		get_envlst_val(msh, "PWD")->value = msh->home;
		msh->pwd = get_envlst_val(msh, "PWD")->value;
		print_envlst(msh->env_lst);
	}
	else if (!ft_strcmp(msh->args[1], "-"))
	{
		msh->oldpwd = get_envlst_val(msh, "OLDPWD")->value;
		msh->pwd = get_envlst_val(msh, "PWD")->value;
		if (chdir(msh->oldpwd))
			ft_dprintf(2, "Error chdir\n");
		get_envlst_val(msh, "OLDPWD")->value = msh->pwd;
		get_envlst_val(msh, "PWD")->value = getcwd(buf, CWD_BUF_SIZE);
		print_envlst(msh->env_lst);
	}
	else
	{
		get_envlst_val(msh, "OLDPWD")->value = msh->pwd;
		if (chdir(msh->args[1]))
			ft_dprintf(2, "Error chdir\n");
		get_envlst_val(msh, "PWD")->value = getcwd(buf, CWD_BUF_SIZE);
		msh->pwd = get_envlst_val(msh, "PWD")->value;
		print_envlst(msh->env_lst);
	}
}

void		builtin_exit(t_minishell *msh)
{
	if (!msh->args[1])
		exit(EXIT_SUCCESS);
	exit(ft_atoi(msh->args[1]));
}

int		is_builtin(t_minishell *msh, char *cmd_name)
{
	int		i;

	i = -1;
	while (++i < BUILTIN_NUM)
		if (!ft_strcmp(msh->builtin_name[i], cmd_name))
			return (i);
	return (-1);
}

int		exec_cmd(t_minishell *msh)
{
	int		i;
	int		pos;

	i = 0;
	pos = 0;
	msh->cmds = ft_strsplit(msh->line, ';');
	if (!msh->cmds[0])
		return (0);
	while (msh->cmds[i])
	{
		msh->argc = 0;
		msh->cmd_path = NULL;
		msh->args = ft_split_whitespaces(msh->cmds[i]);
		if (*msh->args)
		{
			if (ft_strchr(msh->args[0], '/'))
				msh->cmd_path = msh->args[0];
			else
				find_path_cmd(msh);
			if ((pos = is_builtin(msh, msh->args[0])) != -1)
				(msh->funct_tab[pos])(msh);
			else
				launch_cmd(msh);
		}
		i++;
	}
	return (1);
}


void	add_envlist(t_envlst **env_lst, char **tab_env)
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

void	set_envlist(t_minishell *msh, char **env)
{
	char	**tab_env;

	tab_env = NULL;
	while (*env)
	{
		tab_env = ft_strsplit(*env, '=');
		if (*tab_env)
			add_envlist(&msh->env_lst, tab_env);
		free_dbl(&tab_env);
		env++;
	}
}

void	init_msh(t_minishell *msh, char **env)
{
	msh->cmds = NULL;
	msh->cmd_path = NULL;
	msh->args = NULL;
	msh->argc = 0;
	msh->home = NULL;
	msh->oldpwd = NULL;
	msh->env_lst = NULL;
	msh->env = env;	
	set_builtin(msh);
	set_envlist(msh, env);
	set_oldpwd(msh);
	print_envlst(msh->env_lst);
}

void	prompt_dir(void)
{
	char	*cwd;
	char	*buf;

	buf = NULL;
	cwd = getcwd(buf, CWD_BUF_SIZE);
	if (cwd)
		ft_putstr(cwd);
	ft_putstr(">>> ");
}

int		main(int ac, char *av[], char *env[])
{
	t_minishell		msh;
	
	init_msh(&msh, env);	
	signal(SIGINT, sigint_handler);
	while (IS_TRUE)
	{
		if (sigsetjmp(sig_env, 1) == 1337)
			ft_putchar('\n');
		jmp_flag = 1;
		prompt_dir();
		read_cmd(&msh);
		if (!exec_cmd(&msh))
			continue ;
		free_msh(&msh);
	}
	return (0);
}
