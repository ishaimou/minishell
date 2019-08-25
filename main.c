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

void	quotes(t_minishell *msh, char *line, int i)
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
}

int		handle_quotes(t_minishell *msh, char *line)
{
	int		i;

	i = 0;
	while (line[i])
	{	
		quotes(msh, line, i);
		i++;
	}
	if (msh->quote)
		return (1);
	else if (msh->dquote)
		return (2);
	else
		return (0);
}

void	read_line(t_minishell *msh, int *q)
{
	char	*line;
	char	*tmp;

	line = NULL;
	if (get_next_line(0, &line) < 0)
	{
		ft_dprintf(2, "Error: gnl cannot read from fd\n");
		exit(EXIT_FAILURE);
	}
	*q = handle_quotes(msh, line);
	if (msh->quoted)
	{
		tmp = msh->line;
		msh->line = ft_strjoin_sep(msh->line, line, "\n");
		ft_strdel(&tmp);
		ft_strdel(&line);
	}
	else
		msh->line = line;
}

void	read_cmd(t_minishell *msh)
{
	int		q;

	read_line(msh, &q);
	msh->quoted = q;
	if (!q)
		return ;
	if (q == 1)
		ft_putstr("quote> ");
	else if (q == 2)
		ft_putstr("dquote> ");
	read_cmd(msh);
}

void	error_fork(t_minishell *msh)
{
	ft_dprintf(2, "Error: fork() cannot fork a process\n");
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

	if (check_err(msh, NO_FOUND))
		exit(EXIT_FAILURE);
	if  (execve(msh->cmd_path, (msh->args + ind), msh->env) < 0)
	{
		print_chars(msh->cmd_path);
		ft_putstr(": failed execution\n");
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
		wpid = waitpid(cpid, &stat_loc, WUNTRACED);
		free_dbl(&msh->env);
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
		if (!(dirp = opendir(paths[i])) && ++i)
			continue ;
		while ((res = readdir(dirp)))
		{
			if (!ft_strcmp(res->d_name, msh->cmd_path))
			{
				rm_trailing_slash(&paths[i]);
				ft_strdel(&msh->cmd_path);
				msh->cmd_path = ft_strjoin_sep(paths[i], res->d_name, "/");
				(void)closedir(dirp);
				return ;
			}
		}
		(void)closedir(dirp);
		i++;
	}
}

void	find_path(t_minishell *msh, int i)
{
	char			**path_env;
	char			**paths;

	path_env = NULL;
	paths = NULL;
	path_env = ft_strsplit(msh->env[i], '=');
	if (*path_env)
	{
		paths = ft_strsplit(*(path_env + 1), ':');
		free_dbl(&path_env);
	}
	if (paths && *paths)
	{
		get_cmd_path(msh, paths);
		free_dbl(&paths);
	}
}

int		find_path_cmd(t_minishell *msh, int ind)
{
	int				i;

	i = 0;
	msh->env = set_env(msh);
	msh->cmd_path = ft_strdup(msh->args[ind]);
	if (ft_strchr(msh->cmd_path, '/'))
		return (check_err(msh, 0));
	while (msh->env[i] && !ft_strstr(msh->env[i], "PATH"))
		i++;
	if (!msh->env[i])
		return (0) ;
	find_path(msh, i);
	return (0);
}

int		get_alias(t_minishell *msh, char **line, int fd)
{
	char	*cmd;
	char	**args;

	cmd = ft_strdup(*line);
	simplify_cmd(&cmd);
	args = ft_strsplit(cmd, '=');
	if (args[0] && !ft_strcmp(args[0], msh->args[0]))
	{
		ft_strdel(&msh->args[0]);
		if (args[1])
			msh->args[0] = ft_strdup(args[1]);
		else
			msh->args[0] = ft_strnew(0);
		ft_strdel(line);
		free_dbl(&args);
		free(cmd);
		close(fd);
		return (1);
	}
	ft_strdel(line);
	free(cmd);
	free_dbl(&args);
	return (0);
}

int		fetch_alias(t_minishell *msh)
{
	int		fd;
	char	*line;

	if ((fd = open("./alias.config", O_RDONLY)) == -1)
		return (0);
	line = NULL;
	while (get_next_line(fd, &line) > 0)
		if (get_alias(msh, &line, fd))
			return (1);
	ft_strdel(&line);
	close(fd);
	return (0);
}

void	split_n_join(t_minishell *msh)
{
	char	*str;
	char	*tmp;
	int		i;

	i = 0;
	str = ft_strnew(0);
	while (msh->args[i])
	{
		tmp = str;
		str = ft_strjoin_sep(str, msh->args[i], " ");
		free(tmp);
		i++;
	}
	free_dbl(&msh->args);
	msh->args = ft_split_whitespaces(str);
	ft_strdel(&str);
}

void	exec_cmd(t_minishell *msh)
{
	int		pos;
	int		ind;

	ind = 0;
	if (fetch_alias(msh))
		split_n_join(msh);
	while (msh->args[ind] && ft_strchr(msh->args[ind], '=') 
			&& set_varlst(msh, msh->args[ind]))
		ind++;
	handle_exp(msh);
	if (!msh->args[ind] || !msh->args[ind][0])
		return ;
	if ((pos = is_builtin(msh, msh->args[ind])) != -1)
		(msh->funct_tab[pos])(msh, ind);
	else
	{
		if (find_path_cmd(msh, ind))
		{
			free_dbl(&msh->env);
			return ;
		}
		launch_cmd(msh, ind);
	}
}

char	**alloc_alias(t_minishell *msh, char **line, int i)
{
	char	**tab_str;

	if (!(tab_str = (char**)malloc(sizeof(char*) * 3)))
		malloc_error(msh);
	tab_str[0] = ft_strdup("alias");
	tab_str[1] = ft_strtrim(*line + i);
	tab_str[2] = NULL;
	ft_strdel(line);
	return (tab_str);
}

char	**split_alias(t_minishell *msh)
{
	char	*line;
	int		i;

	if (!strstr(msh->line, "alias"))
		return (NULL);
	line = ft_strtrim(msh->line);
	if (ft_strncmp(line, "alias", 5))
	{
		ft_strdel(&line);
		return (NULL);
	}
	i = 0;
	while (line[i] && !ft_isblank(line[i]))
		i++;
	if (!line[i])
	{
		ft_strdel(&line);
		return (NULL);
	}
	return (alloc_alias(msh, &line, i));
}

void	parse_exec_cmd(t_minishell *msh)
{
	int		i;

	i = 0;
	if (msh->sflag && !ft_strstr(msh->line, "alias"))
		simplify_cmd(&msh->line);
	msh->cmds = ft_strsplit(msh->line, ';');
	if (!msh->cmds[0])
		return ;
	while (msh->cmds[i])
	{
		msh->argc = 0;
		msh->cmd_path = NULL;
		if (!(msh->args = split_alias(msh)))
			msh->args = ft_split_whitespaces(msh->cmds[i]);
		if (*msh->args)
			exec_cmd(msh);
		free_dbl(&msh->args);
		ft_strdel(&msh->cmd_path);
		i++;
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
		parse_exec_cmd(&msh);
		ft_strdel(&msh.line);
		free_dbl(&msh.cmds);
	}
	return (0);
}
