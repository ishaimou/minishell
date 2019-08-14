#include "minishell.h"

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

void	prompt_dir(void)
{
	char	*cwd;
	char	*buf;

	buf = NULL;
	cwd = getcwd(buf, CWD_BUF_SIZE);
	if (cwd)
	{
		ft_putstr("\033[1;32m");
		ft_putstr(cwd);
		ft_putstr("\033[m");
	}
	ft_putstr("$> ");
}

