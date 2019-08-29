#include "minishell.h"

static void	simplify(char **str, char **cmd)
{
	char	*line;
	
	line = *cmd;
	while (*line)
	{
		while (*line && *line != '\'' && *line != '\"')
				*str = ft_str_pushback(*str, *line++);
		if (*line == '\'')
		{
			while (*line++ && *line != '\'')
				*str = ft_str_pushback(*str, *line);
		}
		if (*line == '\"')
		{
			while (*line++ && *line != '\"')
				*str = ft_str_pushback(*str, *line);
		}
		if (*line)
			line++;
	}
}

void	simplify_cmd(char **cmd)
{
	char	*str;
	
	str = NULL;
	simplify(&str, cmd);
	if (str)
	{
		ft_strdel(cmd);
		*cmd = str;
	}
}

void		realloc_args(t_minishell *msh)
{
	t_diclst	*node;

	free_dbl(&msh->args);
	if (!(msh->args = (char**)malloc(sizeof(char*) * 3)))
		malloc_error(msh);
	msh->args[0] = ft_strdup("cd");
	node = get_diclst_val(msh, "HOME", 0);
	if (node)
		msh->home = node->value;
	else
		msh->home = "";
	msh->args[1] = ft_strdup(msh->home);
	msh->args[2] = NULL;
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
