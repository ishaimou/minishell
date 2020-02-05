/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:46:52 by ishaimou          #+#    #+#             */
/*   Updated: 2020/02/03 03:37:51 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			malloc_error(t_minishell *msh)
{
	ft_dprintf(2, "Error: malloc() failed allocate memory\n");
	free_msh(msh);
	exit(EXIT_FAILURE);
}

void			fork_error(t_minishell *msh)
{
	ft_dprintf(2, "Error: fork() cannot fork a process\n");
	free_msh(msh);
	exit(EXIT_FAILURE);
}

void			gnl_error(void)
{
	ft_dprintf(2, "Error: gnl cannot read from standard input\n");
	exit(EXIT_FAILURE);
}

static int		ft_error(t_minishell *msh, int type)
{
	if (type == NO_FILE_DIR)
	{
		print_chars(msh->cmd_path);
		ft_putstr(": no such file or directory\n");
	}
	else if (type == NO_PERM)
	{
		print_chars(msh->cmd_path);
		ft_putstr(": permission denied\n");
	}
	else if (type == NO_FOUND)
	{
		print_chars(msh->cmd_path);
		ft_putstr(": command not found\n");
	}
	return (1);
}

int				check_err(t_minishell *msh, int mode)
{
	struct stat	file_stat;

	if (mode == NO_FOUND)
		if (stat(msh->cmd_path, &file_stat) || !ft_strchr(msh->cmd_path, '/'))
			return (ft_error(msh, NO_FOUND));
	if (access(msh->cmd_path, F_OK))
		return (ft_error(msh, NO_FILE_DIR));
	else if (access(msh->cmd_path, X_OK))
		return (ft_error(msh, NO_PERM));
	else if (!stat(msh->cmd_path, &file_stat))
		if (!S_ISREG(file_stat.st_mode))
			return (ft_error(msh, NO_PERM));
	return (0);
}
