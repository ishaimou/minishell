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

void	get_cmd_path(t_minishell *msh, char **paths, int ind)
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
			if (!ft_strcmp(res->d_name, msh->args[ind]))
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

void	find_path_cmd(t_minishell *msh, int ind)
{
	char			**path_env;
	char			**paths;
	int				i;

	path_env = NULL;
	paths = NULL;
	i = 0;
	if (ft_strchr(msh->args[ind], '/'))
		msh->cmd_path = msh->args[ind];
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
			get_cmd_path(msh, paths, ind);
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

void	launch_cmd(t_minishell *msh, int ind)
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
		if (execve(msh->cmd_path, (msh->args + ind), msh->env) < 0)
		{
			print_chars(msh->args[ind]);
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

int		set_varlst(t_minishell *msh, char *arg)
{
	t_diclst	*node;
	char		**tab_var;
	int			i;

	if (arg[0] == '=')
		return (0);
	i = 0;
	while (arg[i] && arg[i] != '=' && ft_isalnum(arg[i]))
		i++;
	if (arg[i] != '=')
		return (0);
	tab_var = (char**)malloc(sizeof(char*) * 3);
	tab_var[0] = ft_strndup(arg, i);
	tab_var[1] = ft_strdup(arg + i + 1);
	tab_var[2] = NULL;
	node = get_diclst_val(msh, tab_var[0], 1);
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(tab_var[1]);
	}
	else
		add_diclst(&msh->var_lst, tab_var);
	free_dbl(&tab_var);
	return (1);
}

void	get_value(t_minishell *msh, char **arg, char *ptr)
{
	t_diclst	*node;
	char		**tab_dollar;
	char		*str;
	char		*tmp;
	int			i;
	int			len;

	i = ptr - *arg;
	str = ft_strndup(*arg, i);
	tab_dollar = ft_strsplit(ptr, '$');
	if (*tab_dollar)
	{
		i = -1;
		len = get_argc(tab_dollar);
		while (++i < len)
		{
			if ((node = get_diclst_val(msh, tab_dollar[i], 1)) ||
				 (node = get_diclst_val(msh, tab_dollar[i], 0)))
			{
				tmp = str;
				str = ft_strjoin(str, node->value);
				free(tmp);
			}
		}
	}
	free(tab_dollar);
	free(*arg);
	*arg = str;
}

void	handle_exp(t_minishell *msh)
{
	char	*ptr;
	char	*tmp;
	int		i;

	i = 0;
	msh->home = get_diclst_val(msh, "HOME", 0)->value;
	while (msh->args[i])
	{
		while ((ptr = ft_strchr(msh->args[i], '$')))
			get_value(msh, &msh->args[i], ptr);
		if (msh->args[i][0] == '~')
		{
			tmp = msh->args[i];
			msh->args[i] = ft_strjoin(msh->home, msh->args[i] + 1);
			free(tmp);
		}
		i++;
	}
}

void	exec_cmd(t_minishell *msh)
{
	int		pos;
	int		ind;

	ind = 0;
	while (msh->args[ind] && ft_strchr(msh->args[ind], '=') && set_varlst(msh, msh->args[ind]))
		ind++;
	handle_exp(msh);
	if (!msh->args[ind] || !msh->args[ind][0])
		return ;
	if ((pos = is_builtin(msh, msh->args[ind])) != -1)
		(msh->funct_tab[pos])(msh);
	else
	{
		find_path_cmd(msh, ind);
		launch_cmd(msh, ind);
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
