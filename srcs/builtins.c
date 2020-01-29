/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:46:24 by ishaimou          #+#    #+#             */
/*   Updated: 2020/01/29 02:29:04 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			builtin_help(t_minishell *msh, int ind)
{
	(void)ind;
	(void)msh;
	ft_printf("%{BLUE}==============================================");
	ft_printf("==== *** %{CYAN}MINISHELL%{eoc}");
	ft_printf("%{BLUE}*** ==========================================");
	ft_printf("========%{eoc}\n\n\n");
	ft_printf("SYNOPSIS\n\t./minishell\n\n");
	ft_printf("DESCRIPTION\n\tMinishell is a minimalist interactive");
	ft_printf(" command line ");
	ft_printf("interpreter that executes commands read from standard");
	ft_printf(" input.\n\n");
	ft_printf("MANDATORY BUILTINS\n\tcd - echo - exit - env - setenv - ");
	ft_printf("unsetenv\n\n");
	ft_printf("BONUS BUILTINS\n\tsource - alias - help\n\n");
	ft_printf("LISTS\n\tA sequence of one or more commands separated by");
	ft_printf(" one semicolon.\n");
	ft_printf("\t\tEXAMPLE: command1 ; command2 ; ...\n\n");
	ft_printf("NOTES\n\tCommands separated by a ; are executed sequentially: ");
	ft_printf("the minishell waits for each command to terminate in turn.\n\n");
	ft_printf("\tUse man pages to learn about how to use any other ");
	ft_printf("UNIX commands.\n\n");
}

void			builtin_exit(t_minishell *msh, int ind)
{
	int			argc;
	int			ret;
	char		**args;

	args = msh->args + ind;
	argc = get_argc(args);
	if (argc > 2)
	{
		ft_printf("exit: too many arguments\n");
		return ;
	}
	ret = args[1] ? ft_atoi(args[1]) : EXIT_SUCCESS;
	free_msh(msh);
	exit(ret);
}

void			builtin_echo(t_minishell *msh, int ind)
{
	char		**args;
	int			argc;
	int			i;

	i = 1;
	args = msh->args + ind;
	argc = get_argc(args);
	args++;
	while (*args)
	{
		i++;
		ft_printf("%s", *args);
		if (i != argc)
			ft_putchar(' ');
		args++;
	}
	ft_putchar('\n');
}

static void		copy_n_affect(t_minishell *msh, char **line, int action)
{
	static char	*cpy_line;
	static char	**cpy_args;
	static char	**cpy_cmds;

	if (!action)
	{
		cpy_line = ft_strdup(msh->line);
		cpy_args = ft_strddup(msh->args);
		cpy_cmds = ft_strddup(msh->cmds);
		ft_strdel(&msh->line);
		free_dbl(&msh->args);
		free_dbl(&msh->cmds);
	}
	else
	{
		ft_strdel(line);
		msh->line = cpy_line;
		msh->args = cpy_args;
		msh->cmds = cpy_cmds;
	}
}

void			builtin_source(t_minishell *msh, int ind)
{
	int			argc;
	char		**args;
	char		*line;
	int			fd;

	args = msh->args + ind;
	if ((argc = get_argc(args)) != 2)
	{
		ft_printf("source: usage: source [FILE]\n");
		return ;
	}
	if ((fd = open(args[1], O_RDONLY)) == -1)
		return ;
	copy_n_affect(msh, &line, 0);
	msh->sflag = 1;
	while (get_next_line(fd, &line) > 0)
	{
		msh->line = line;
		parse_exec_cmd(msh);
		ft_strdel(&msh->line);
		free_dbl(&msh->args);
		free_dbl(&msh->cmds);
	}
	copy_n_affect(msh, &line, 1);
	close(fd);
}
