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
