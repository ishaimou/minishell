#include "minishell.h"

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


void	rm_trailing_slash(char **path)
{
	int		i;
	char	*str;

	i = 0;
	if (*path && **path)
	{
		str = ft_strdup(*path);
		while (str[i])
			i++;
		i--;
		while (str[i] && str[i] == '/')
			i--;
		str[i + 1] = '\0';
		free(*path);
		*path = str;
	}
}

char	*ft_strjoin_sep(const char *s1, const char *s2, char *sep)
{
	char	*strsep;
	char	*str;

	strsep = NULL;
	str = NULL;
	strsep = ft_strjoin(s1, sep);
	str = ft_strjoin(strsep, s2);
	free(strsep);
	return (str);
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

char	*home_to_tild(t_minishell *msh, char *str)
{
	char		*tmp;
	int			len;
	char		*new_str;
	t_diclst	*node;

	new_str = ft_strdup(str);
	node = get_diclst_val(msh, "HOME", 0);
	if (node)
		msh->home = node->value;
	if (msh->home[0] && ft_strstr(str, msh->home))
	{
		len = ft_strlen(msh->home);
		free(new_str);
		new_str = ft_strdup(str + len);
		tmp = new_str;
		new_str = ft_strjoin("~", new_str);
		free(tmp);
	}
	return (new_str);
}

void	prompt_dir(t_minishell *msh)
{
	char *buf;
	char *cwd;
	char *cwd_tild;
	t_diclst	*node;

	buf = NULL;
	node = get_diclst_val(msh, "USER", 0);
	if (node)
		msh->user = node->value;
	else
		msh->user = "";
	cwd = getcwd(buf, CWD_BUF_SIZE);
	cwd_tild = home_to_tild(msh, cwd);
	if (msh->user)
	{
		ft_putstr("\033[1;32m");
		ft_putstr(msh->user);
		ft_putstr("\033[m:");
	}
	ft_putstr("\033[1;36m");
	ft_putstr(cwd_tild);
	ft_putstr("\033[m ");
	ft_putstr("$> ");
	free(cwd);
	free(cwd_tild);
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
