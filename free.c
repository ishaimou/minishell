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

void	free_msh(t_minishell *msh)
{
	if (msh->line)
		free(msh->line);
	if (msh->args)
		free_dbl(&msh->args);
	msh->line = NULL;
}

