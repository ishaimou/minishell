/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:45:44 by ishaimou          #+#    #+#             */
/*   Updated: 2020/01/29 01:51:17 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**alloc_alias(t_minishell *msh, char **line, int i)
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

char		**split_alias(t_minishell *msh)
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

static int	get_alias(t_minishell *msh, char **line, int fd)
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

int			fetch_alias(t_minishell *msh)
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
