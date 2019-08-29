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

static int 	read_alias(char **args)
{
	char	*line;
	int		fd;
	
	if (!args[1])
	{
		if ((fd = open(PATH, O_RDONLY)) == -1)
			return (1);
		line = NULL;
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

static int		check_duplicat(char **args)
{
	char	**tab_str;
	char	*line;
	int		fd;
	int		offset;

	offset = 0;
	if ((fd = open(PATH, O_RDONLY)) == -1)
		return (0);	
	line = NULL;
	tab_str = ft_strsplit(args[1], '=');
	while (get_next_line(fd, &line) > 0 && ++offset)
	{	
		if (!ft_strncmp(line, tab_str[0], ft_strlen(tab_str[0])))
		{
			ft_strdel(&line);
			free_dbl(&tab_str);
			close(fd);
			return (offset);
		}
		ft_strdel(&line);
	}
	ft_strdel(&line);
	free_dbl(&tab_str);
	close(fd);
	return (0);
}

static void	rewrite_alias(char *str, int offset)
{
	int		num_line;
	FILE	*f;
	FILE	*fc;
	char	buff[BUFF_SIZE];

	f = fopen(PATH, "r");
	fc = fopen("./alias.copy", "w");
	if (!f || !fc)
		return ;
	num_line = 0;
	while (fgets(buff, BUFF_SIZE, f))
	{
		num_line++;
		if (num_line == offset)
		{
			fputs(str, fc);
			fputs("\n", fc);
		}
		else
			fputs(buff, fc);
	}
	fclose(f);
	fclose(fc);
	remove(PATH);
	rename("./alias.copy", PATH);
}

void	builtin_alias(t_minishell *msh, int ind)
{
	char	**args;
	int		offset;
	int		fd;

	args = msh->args + ind;
	if (read_alias(args))
		return ;
	if (check_assignement(&msh->args[ind], args[1]))
		return ;
	if (!(offset = check_duplicat(args)))
	{
		fd = open(PATH, O_RDWR | O_APPEND | O_CREAT, 0644);
		if (fd == -1)
			return ;
		ft_putendl_fd(args[1], fd);
		close(fd);
	}
	else
		rewrite_alias(args[1], offset);
}
