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
		if ((fd = open("alias.config", O_RDONLY)) == -1)
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

int		check_duplicat(char **args)
{
	char	**tab_str;
	char	*line;
	int		fd;
	int		offset;

	offset = 0;
	if ((fd = open("./alias.config", O_RDONLY)) == -1)
		return (0);	
	line = NULL;
	tab_str = ft_strsplit(args[1], '=');
	if (*tab_str)
	{
		while (get_next_line(fd, &line) > 0)
		{	
			offset++;
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
	}
	free_dbl(&tab_str);
	close(fd);
	return (0);
}

void	rewrite_alias(char *str, int offset)
{
	int		fd;
	int		fdc;
	int		num_line;
	char	*line;
	char	buff[BUFF_SIZE];
	FILE	*f;
	FILE	*fc;
	char	*path = "./alias.config";

	//f = fopen(path, "r");
	//fc = fopen("./alias.copy", "w");
	//if (!f || !fc)
	//	return ;
	fd = open("./alias.config", O_RDONLY);
	fdc = open("./alias.copy", O_RDWR | O_APPEND | O_CREAT, 0644);
	if (fd == -1 || fdc == -1)
		return ;
	line = NULL;
	num_line = 0;
	//ft_printf("$$$ string = %s $$$\n", str);
	while (//fgets(buff, 1024, f)
		get_next_line(fd, &line) > 0)
	{
		num_line++;
		//ft_printf("### line = %s ###\n", line);
		//ft_printf("===num_line=%d | offset=%d===\n", num_line, offset);
		if (num_line == offset)
		{
			ft_putendl_fd(str, fdc);
			//fputs(str, fc);
			//fputs("\n", fc);
		}
		else
		{
			ft_putendl_fd(line, fdc);
			//fputs(buff, fc);
		}
		//ft_strdel(&line);
	}
	//ft_strdel(&line);
	close(fd);
	close(fdc);
	//fclose(f);
	//fclose(fc);
	remove(path);
	rename("./alias.copy", path);
}

void	builtin_alias(t_minishell *msh, int ind)
{
	char	**args;
	int		offset;
	int		fd;

	args = msh->args + ind;
	if (read_alias(args))
		return ;
	//offset = 3;
	if (check_assignement(&msh->args[ind], args[1]))
		return ;
	if (!(offset = check_duplicat(args)))
	{
		fd = open("./alias.config", O_RDWR | O_APPEND | O_CREAT, 0644);
		if (fd == -1)
			return ;
		ft_putendl_fd(args[1], fd);
		close(fd);
	}
	else
		rewrite_alias(args[1], offset);
}
