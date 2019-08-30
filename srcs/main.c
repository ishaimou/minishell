#include "minishell.h"

void	sigint_handler(int signo)
{
	(void)signo;
	if (!jmp_flag)
		return ;
	siglongjmp(sig_env, 1337);
}

int		main(int ac, char *av[], char *env[])
{
	t_minishell		msh;

	init_msh(&msh, ac, av, env);
	signal(SIGINT, sigint_handler);
	while (1337)
	{
		if (sigsetjmp(sig_env, 1) == 1337)
		{
			init_qparam(&msh);
			ft_putchar('\n');
		}
		jmp_flag = 1;
		msh.sflag = 0;
		prompt_dir(&msh);
		read_cmd(&msh);
		parse_exec_cmd(&msh);
		ft_strdel(&msh.line);
		free_dbl(&msh.cmds);
	}
	return (0);
}
