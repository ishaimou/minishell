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
	char	*tmp;
	int		len;

	msh->home = get_diclst_val(msh, "HOME", 0)->value;
	if (ft_strstr(str, msh->home))
	{
		len = ft_strlen(msh->home);
		str = ft_strdup(str + len);
		tmp = str;
		str = ft_strjoin("~", str);
		free(tmp);
	}
	return (str);
}

void	prompt_dir(t_minishell *msh)
{
	char *pwd;
	msh->user = get_diclst_val(msh, "USER", 0)->value;
	msh->pwd = get_diclst_val(msh, "PWD", 0)->value;
	msh->pwd = home_to_tild(msh, msh->pwd);
	if (msh->user)
	{
		ft_putstr("\033[1;32m");
		ft_putstr(msh->user);
		ft_putstr("\033[m:");
	}
	if (msh->pwd)
	{
		ft_putstr("\033[1;36m");
		ft_putstr(msh->pwd);
		ft_putstr("\033[m ");
	}
	ft_putstr("$> ");
}
