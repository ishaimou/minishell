#include "minishell.h"

static int	check_assignement(char **args, char *str)
{
	char	*tmp;

	tmp = str;
	if (!ft_strchr(str, '='))
		return (1);
	while (*str && !ft_isblank(*str) && *str != '=')
		str++;
	if (*str != '=' || ft_isblank(*(str + 1)))
		return (ft_printf("alias: bad assignement\n"));
	if (*str++ && *str == '\"' && *str++)
		while (*str && *str != '\"')
			str++;
	else if (*str == '\'' && *str++)
		while (*str && *str != '\'')
			str++;
	else
		while (*str && !ft_isblank(*str))
			str++;
	if (*str == '\'' || *str == '\"')
		str++;
	*str = '\0';
	free(*args);
	*args = ft_strdup(tmp);
	return (0);
}

static int 	alias_print(char **args, int fd)
{
	char	*line;
	
	line = NULL;
	if (!args[1])
	{
		while (get_next_line(fd, &line) > 0)
		{
			ft_printf("%s\n", line);
			ft_strdel(&line);
		}
		ft_strdel(&line);
		close(fd);
		return (1);
	}
	return (0);
}

void	builtin_alias(t_minishell *msh, int ind)
{
	int		fd;
	char	**args;
	
	args = msh->args + ind;
	fd = open("./alias.config", O_RDWR | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
		return ;
	if (alias_print(args, fd))
		return ;
	if (check_assignement(&msh->args[ind], args[1]))
	{
		close(fd);
		return ;
	}
	ft_putendl_fd(args[1], fd);
	close(fd);
}
