#include "minishell.h"

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


void	prompt_dir(void)
{
	char	*cwd_buf;

	cwd_buf = NULL;
	getcwd(cwd_buf, CWD_BUF_SIZE);
	if (cwd_buf)
		ft_putstr(cwd_buf);
	ft_putstr(" >>> ");
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

void	find_path_cmd(t_minishell *msh, char **env)
{
	char			**path_env;
	char			**paths;

	path_env = NULL;
	paths = NULL;
	while (*env && !ft_strstr(*env, "PATH"))
		env++;
	if (env)
	{
		path_env = ft_strsplit(*env, '=');
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

void		get_home(t_minishell *msh, char **env)
{
	char	**home_env;

	home_env = NULL;
	while (*env && !ft_strstr(*env, "HOME"))
		env++;
	if (env)
	{
		home_env = ft_strsplit(*env, '=');
		if (*home_env)
			msh->home = home_env[1];
	}
}

void		get_oldpwd(t_minishell *msh, char **env)
{
	char	**oldpwd_env;
	
	oldpwd_env = NULL;
	while (*env && !ft_strstr(*env, "OLDPWD"))
		env++;
	if (env)
	{
		oldpwd_env = ft_strsplit(*env, '=');
		if (*oldpwd_env)
			msh->oldpwd = oldpwd_env[1];
	}
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

void		builtin_cd(t_minishell *msh)
{
	msh->argc = get_argc(msh->args);
	if (msh->argc > 2)
		ft_printf("usage: cd [optional argument]\n");
	else if (!msh->args[1])
	{
		get_home(msh, msh->env);
		if (chdir(msh->home))
			ft_dprintf(2, "Error chdir\n");
	}
	else if (!ft_strcmp(msh->args[1], "-"))
	{
		get_oldpwd(msh, msh->env);
		if (chdir(msh->oldpwd))
			ft_dprintf(2, "Error chdir\n");
	}
	else
	{
		if (chdir(msh->args[1]))
			ft_dprintf(2, "Error chdir\n");
	}
	msh->argc = 0;
	msh->cmd_path = NULL;
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

int		exec_cmd(t_minishell *msh, char **env)
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
		msh->args = ft_split_whitespaces(msh->cmds[i]);
		if (*msh->args)
		{
			if (ft_strchr(msh->args[0], '/'))
				msh->cmd_path = msh->args[0];
			else
				find_path_cmd(msh, env);
			if ((pos = is_builtin(msh, msh->args[0])) != -1)
				(msh->funct_tab[pos])(msh);
			else
				launch_cmd(msh);
		}
		i++;
	}
	return (1);
}
/*
void	exec_cmd(t_minishell *msh)
{
	int		pos;	

}
*/
int		main(int ac, char *av[], char *env[])
{
	t_minishell		msh;
	msh.env = env;
	msh.cmds = NULL;
	msh.cmd_path = NULL;
	msh.args = NULL;
	msh.argc = 0;
	msh.home = NULL;
	msh.oldpwd = NULL;

	print_2d(env);
	set_builtin(&msh);
	signal(SIGINT, sigint_handler);
	while (IS_TRUE)
	{
		if (sigsetjmp(sig_env, 1) == 1337)
			ft_putchar('\n');
		jmp_flag = 1;
		ft_putstr(">>> ");
		//prompt_dir();
		read_cmd(&msh);
		if (!exec_cmd(&msh, env))
			continue ;
		printf("odlpwd = %s\n", msh.oldpwd);
		//print_2d(msh.args);
		//printf("cmd_path = %s\n", msh.cmd_path); 
		//msh.cmd_path = NULL;
		free_msh(&msh);
	}
	return (0);
}
