#include "minishell.h"

//------------- TEMPORARY FUNCTIONS ---------------//

void	print_2d(char **s)
{
	while (*s)
		printf("%s\n", *s++);
}

void	print_diclst(t_diclst	*dic_lst)
{
	while (dic_lst)
	{
		ft_printf("%s=%s\n", dic_lst->name, dic_lst->value);
		dic_lst = dic_lst->next;
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

void	error_fork(t_minishell *msh)
{
	ft_dprintf(2, "Error fork\n");
	free_msh(msh);
	exit(EXIT_FAILURE);
}

int		ft_error(t_minishell *msh, int type)
{
	if (type == NO_FILE_DIR)
	{
		print_chars(msh->cmd_path);
		ft_putstr(": no such file or directory\n");
	}
	else if (type == NO_PERM)
	{
		print_chars(msh->cmd_path);
		ft_putstr(": permission denied\n");
	}
	else if (type == NO_FOUND)
	{
		print_chars(msh->cmd_path);
		ft_putstr(": command not found\n");
	}
	return (1);
}

int		check_err(t_minishell *msh, int mode)
{
	struct stat	file_stat;

	if (mode == NO_FOUND)
		if (stat(msh->cmd_path, &file_stat))
			return (ft_error(msh, NO_FOUND));
	if (access(msh->cmd_path, F_OK))
		return (ft_error(msh, NO_FILE_DIR));
	else if (access(msh->cmd_path, X_OK))
		return (ft_error(msh, NO_PERM));
	else if (!stat(msh->cmd_path, &file_stat))
		if (!S_ISREG(file_stat.st_mode))
			return (ft_error(msh, NO_PERM));
	return (0);
}

void	ft_execve(t_minishell *msh, int ind)
{

	msh->env = set_env(msh);
	if (check_err(msh, NO_FOUND))
		exit(EXIT_FAILURE);
	if  (execve(msh->cmd_path, (msh->args + ind), msh->env) < 0)
	{
		print_chars(msh->cmd_path);
		ft_putstr(": Error execve\n");
		free_msh(msh);
		exit(EXIT_FAILURE);
	}
}

void	launch_cmd(t_minishell *msh, int ind)
{
	pid_t	cpid;
	pid_t	wpid;
	int		stat_loc;

	if ((cpid = fork()) < 0)
		error_fork(msh);
	else if (cpid == 0)
		ft_execve(msh, ind);
	else
	{
		if ((wpid = waitpid(cpid, &stat_loc, WUNTRACED)) < 0)
		{
			ft_dprintf(2, "Error waitpid\n");
			exit(EXIT_FAILURE);
		}
	}
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
		{
			i++;
			continue ;
		}
		while ((res = readdir(dirp)))
		{
			if (!ft_strcmp(res->d_name, msh->cmd_path))
			{
				rm_trailing_slash(&paths[i]);
				msh->cmd_path = ft_strjoin_sep(paths[i], res->d_name, "/");
				(void)closedir(dirp);
				return ;
			}
		}
		(void)closedir(dirp);
		i++;
	}
}

int		find_path_cmd(t_minishell *msh, int ind)
{
	char			**path_env;
	char			**paths;
	int				i;

	path_env = NULL;
	paths = NULL;
	i = 0;
	msh->cmd_path = msh->args[ind];
	if (ft_strchr(msh->cmd_path, '/'))
		return (check_err(msh, 0));
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
	return (0);
}

int		exec_cmd(t_minishell *msh)
{
	int		pos;
	int		ind;

	ind = 0;
	while (msh->args[ind] && ft_strchr(msh->args[ind], '=') 
		&& set_varlst(msh, msh->args[ind]))
		ind++;
	handle_exp(msh);
	if (!msh->args[ind] || !msh->args[ind][0])
		return (1) ;
	if ((pos = is_builtin(msh, msh->args[ind])) != -1)
		(msh->funct_tab[pos])(msh);
	else
	{
		if (find_path_cmd(msh, ind))
			return (1);
		launch_cmd(msh, ind);
	}
	return (0);
}

int		parse_exec_cmd(t_minishell *msh)
{
	int		i;

	i = 0;
	if (msh->sflag)
		simplify_cmd(msh);
	msh->cmds = ft_strsplit(msh->line, ';');
	if (!msh->cmds[0])
		return (1);
	while (msh->cmds[i])
	{
		msh->argc = 0;
		msh->cmd_path = NULL;
		msh->args = ft_split_whitespaces(msh->cmds[i]);
		if (*msh->args)
			return (exec_cmd(msh));
		i++;
	}
	return (0);
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
		if (parse_exec_cmd(&msh))
			continue ;
		free_msh(&msh);
	}
	return (0);
}
