#include "minishell.h"

static void	ft_execve(t_minishell *msh, int ind)
{

	if (check_err(msh, NO_FOUND))
		exit(EXIT_FAILURE);
	if  (execve(msh->cmd_path, (msh->args + ind), msh->env) < 0)
	{
		print_chars(msh->cmd_path);
		ft_putstr(": failed execution\n");
		exit(EXIT_FAILURE);
	}
}

void	launch_cmd(t_minishell *msh, int ind)
{
	pid_t	cpid;
	pid_t	wpid;
	int		stat_loc;

	if ((cpid = fork()) < 0)
		fork_error(msh);
	else if (cpid == 0)
		ft_execve(msh, ind);
	else
	{
		wpid = waitpid(cpid, &stat_loc, WUNTRACED);
		free_dbl(&msh->env);
	}
}
