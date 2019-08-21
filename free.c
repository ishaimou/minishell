#include "minishell.h"

void    free_dbl(char ***tab)
{
	int		i;

	i = 0;
	while ((*tab)[i])
	{
		free((*tab)[i]);
		(*tab)[i] = NULL;
		i++;
	}
	free(*tab);
	*tab = NULL;
}

void	free_diclst(t_diclst **begin)
{
	t_diclst 	*curr;
	t_diclst	*tmp;

	curr = *begin;
	while (curr)
	{
		tmp = curr;
		free(curr->name);
		free(curr->value);
		curr = curr->next;
		free(tmp);
	}
	*begin = NULL;
}

void	free_msh(t_minishell *msh)
{
	free(msh->line);
	if (msh->args)
		free_dbl(&msh->args);
	if (msh->cmds)
		free_dbl(&msh->cmds);
	free(msh->cmd_path);
	if (msh->env)
		free_dbl(&msh->env);
}
