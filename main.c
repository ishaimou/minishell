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


int		parse_cmd(t_minishell *msh)
{
	char	**env;

	env = msh->env;
	msh->args = ft_split_whitespaces(msh->line);
	if (!msh->args[0])
		return (0);
	if (ft_strchr(msh->args[0], '/'))
		msh->cmd_path = msh->args[0];
	else
		find_path_cmd(msh, env);
	return (1);
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
			ft_printf("Error cmd\n");
			free_msh(msh);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wpid = waitpid(cpid, &stat_loc, WUNTRACED);
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


int		builtin_cd(char **args)
{
	return (0);
}

int		builtin_exit(char **args)
{
	if (!args[1])
		exit(EXIT_SUCCESS);
	exit(ft_atoi(args[1]));
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

void	exec_cmd(t_minishell *msh)
{
	int		pos;	

	if ((pos = is_builtin(msh, msh->args[0])) != -1)
	{
		(msh->funct_tab[pos])(msh->args);
	}
	else
		launch_cmd(msh);
}

int		main(int ac, char *av[], char *env[])
{
	t_minishell		msh;
	msh.env = env;
	msh.cmd_path = NULL;
	msh.args = NULL;

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
		if (!parse_cmd(&msh))
			continue ;
		exec_cmd(&msh);
		//print_2d(msh.args);
		//printf("cmd_path = %s\n", msh.cmd_path); 
		//msh.cmd_path = NULL;
		free_msh(&msh);
	}
	return (0);
}
