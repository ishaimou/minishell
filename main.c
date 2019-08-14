#include "minishell.h"

//------------- TEMPORARY FUNCTIONS ---------------//

void	print_2d(char **s)
{
	while (*s)
		printf("%s\n", *s++);
}

void	print_envlst(t_envlst	*env_lst)
{
	while (env_lst)
	{
		ft_printf("%s=%s\n", env_lst->name, env_lst->value);
		env_lst = env_lst->next;
	}
}

//------------------------------------------------//

void	sigint_handler(int signo)
{
	(void)signo;
	if (!jmp_flag)
		return ;
	siglongjmp(sig_env, 1337);
}

int		quotes(char *line)
{
	int		i;
	static int	quote;
	static int	dquote;
	static int	qflag;
	static int	dflag;

	i = 0;
	while (line[i])
	{	
		if (line[i] == '\'' && !qflag && !dquote)
		{
			quote++;
			qflag = 1;
		}
		else if (line[i] == '\'' && qflag)
		{
			quote--;
			qflag = 0;
		}
		else if (line[i] == '\"' && !dflag && !quote)
		{
				dquote++;
				dflag = 1;
		}
		else if (line[i] == '\"' && dflag)
		{
			dquote--;
			dflag = 0;
		}
		i++;
	}
	if (quote)
		return (1);
	else if (dquote)
		return (2);
	else
		return (0);
}

void	read_line(t_minishell *msh)
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

void	read_cmd(t_minishell *msh)
{
	int		q;

	read_line(msh);
	q = quotes(msh->line);
	if (!q)
		return ;
	if (q == 1)
		ft_putstr("\nquote> ");
	else if (q == 2)
		ft_putstr("\ndquote> ");
	read_cmd(msh);
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
			break;
		while ((res = readdir(dirp)))
		{
			if (!ft_strcmp(res->d_name, msh->args[0]))
			{
				msh->cmd_path = ft_strjoin_sep(paths[i], res->d_name, "/");
				break ;
			}
		}
		(void)closedir(dirp);
		if (msh->cmd_path)
			break ;
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
	if (ft_strchr(msh->args[0], '/'))
		msh->cmd_path = msh->args[0];
	else
	{
		msh->env = set_env(msh);
		while (msh->env[i] && !ft_strstr(msh->env[i], "PATH"))
			i++;
		if (msh->env[i])
		{
			path_env = ft_strsplit(msh->env[i], '=');
			if (*path_env)
				paths = ft_strsplit(*(path_env + 1), ':');
		}
		if (paths && *paths)
			get_cmd_path(msh, paths);
	}
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
		msh->env = set_env(msh);
		if (execve(msh->cmd_path, msh->args, msh->env) < 0)
		{
			ft_dprintf(2, "%s: command not found\n", msh->args[0]);
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
}

void	exec_cmd(t_minishell *msh)
{
	int		pos;
	
	if ((pos = is_builtin(msh, msh->args[0])) != -1)
		(msh->funct_tab[pos])(msh);
	else
	{
		find_path_cmd(msh);
		launch_cmd(msh);
	}
}

int		parse_exec_cmd(t_minishell *msh)
{
	int		i;

	i = 0;
	msh->cmds = ft_strsplit(msh->line, ';');
	if (!msh->cmds[0])
		return (0);
	while (msh->cmds[i])
	{
		msh->argc = 0;
		msh->cmd_path = NULL;
		msh->args = ft_split_whitespaces(msh->cmds[i]);
		if (*msh->args)
			exec_cmd(msh);
		i++;
	}
	return (1);
}

int		main(int ac, char *av[], char *env[])
{
	t_minishell		msh;
	
	init_msh(&msh, ac, av, env);	
	signal(SIGINT, sigint_handler);
	while (IS_TRUE)
	{
		if (sigsetjmp(sig_env, 1) == 1337)
			ft_putchar('\n');
		jmp_flag = 1;
		prompt_dir();
		read_cmd(&msh);
		if (!parse_exec_cmd(&msh))
			continue ;
		free_msh(&msh);
	}
	return (0);
}
