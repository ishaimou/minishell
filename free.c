#include "minishell.h"

void	malloc_error(t_minishell *msh)
{
	ft_dprintf(2, "Error: malloc() failed allocate memory\n");
	free_msh(msh);
	exit(EXIT_FAILURE);
}

void    free_dbl(char ***tab)
{
	int		i;

	i = 0;
	if (tab && *tab)
	{	
		while ((*tab)[i])
		{
			free((*tab)[i]);
			(*tab)[i] = NULL;
			i++;
		}
		free(*tab);
		*tab = NULL;
	}
}

void	free_diclst(t_diclst **begin)
{
	t_diclst 	*curr;
	t_diclst	*next;

	if (begin && *begin)
	{
		curr = *begin;
		while (curr)
		{
			next = curr->next;
			ft_strdel(&curr->name);
			ft_strdel(&curr->value);
			free(curr);
			curr = next;
		}
		*begin = NULL;
	}
}

void	free_msh(t_minishell *msh)
{
	ft_strdel(&msh->line);
	ft_strdel(&msh->cmd_path);
	free_dbl(&msh->args);
	free_dbl(&msh->cmds);
	free_dbl(&msh->env);
	free(msh->funct_tab);
	free_dbl(&msh->builtin_name);
	free_diclst(&msh->env_lst);
	free_diclst(&msh->var_lst);
}
