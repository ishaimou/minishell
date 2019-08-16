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

int		handle_quotes(t_minishell *msh, char *line)
{
	int		i;

	i = 0;
	while (line[i])
	{	
		if (line[i] == '\'' && !msh->qflag && !msh->dquote)
		{
			msh->quote++;
			msh->qflag = 1;
			msh->sflag = 1;
		}
		else if (line[i] == '\'' && msh->qflag)
		{
			msh->quote--;
			msh->qflag = 0;
		}
		else if (line[i] == '\"' && !msh->dflag && !msh->quote)
		{
				msh->dquote++;
				msh->dflag = 1;
				msh->sflag = 1;
		}
		else if (line[i] == '\"' && msh->dflag)
		{
			msh->dquote--;
			msh->dflag = 0;
		}
		i++;
	}
	if (msh->quote)
		return (1);
	else if (msh->dquote)
		return (2);
	else
		return (0);
}

char	*read_line(t_minishell *msh)
{
	char	*line;
	char	*tmp;
	
	if (get_next_line(0, &line) < 0)
	{
		ft_dprintf(2, "Error gnl\n");
		free(line);
		line = NULL;
		exit(EXIT_FAILURE);
	}
	if (msh->quoted)
	{
		tmp = msh->line;
		msh->line = ft_strjoin_sep(msh->line, line, "\n");
		free(tmp);
	}
	else
		msh->line = line;
	return (line);
}

void	read_cmd(t_minishell *msh)
{
	int		q;
	char	*line;

	line = read_line(msh);
	q = handle_quotes(msh, line);
	msh->quoted = q;
	if (!q)
		return ;
	if (q == 1)
		ft_putstr("quote> ");
	else if (q == 2)
		ft_putstr("dquote> ");
	//free(line);
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
				rm_trailing_slash(&paths[i]);
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

void	print_chars(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			ft_putstr("\\n");
		else if (str[i] == '\t')
			ft_putstr("\\t");
		else
			ft_putchar(str[i]);
		i++;
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
			print_chars(msh->args[0]);
			ft_putstr(": command not found\n");
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

void	handle_exp(t_minishell *msh)
{
	char	*ptr;
	char	*tmp;
	int		i;

	i = 0;
	msh->home = get_envlst_val(msh, "HOME")->value;
	while (msh->args[i])
	{
		//if (ft_strchr(msh->args[i], '='))
		//	set_value(msh, &msh->args[i]);
		if (msh->args[i][0] == '~')
		{
			tmp = msh->args[i];
			msh->args[i] = ft_strjoin(msh->home, msh->args[i] + 1);
			free(tmp);
		}
		//while ((ptr = ft_strchr(msh->args[i], '$')))
		//	get_value(msh, &msh->args[i], ptr);
		i++;
	}
}

void	exec_cmd(t_minishell *msh)
{
	int		pos;
	
	handle_exp(msh);
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
	if (msh->sflag)
		simplify_cmd(msh);
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

void	simplify_cmd(t_minishell *msh)
{
	char	*line;
	char	*str;
	
	str = NULL;
	line = msh->line;
	while (*line)
	{
		while (*line && *line != '\'' && *line != '\"')
				str = ft_str_pushback(str, *line++);
		if (*line == '\'')
		{
			while (*line++ && *line != '\'')
				str = ft_str_pushback(str, *line);
		}
		if (*line == '\"')
		{
			while (*line++ && *line != '\"')
				str = ft_str_pushback(str, *line);
		}
		if (*line)
			line++;
	}
	if (str)
	{
		free(msh->line);
		msh->line = str;
	}
}

int		main(int ac, char *av[], char *env[])
{
	t_minishell		msh;
	
	init_msh(&msh, ac, av, env);	
	signal(SIGINT, sigint_handler);
	while (IS_TRUE)
	{
		if (sigsetjmp(sig_env, 1) == 1337)
		{
			init_qparam(&msh);
			ft_putchar('\n');
		}
		jmp_flag = 1;
		msh.sflag = 0;
		prompt_dir(&msh);
		read_cmd(&msh);
		if (!parse_exec_cmd(&msh))
			continue ;
		free_msh(&msh);
	}
	return (0);
}
