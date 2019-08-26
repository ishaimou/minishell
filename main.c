#include "minishell.h"

//------------- TEMPORARY FUNCTIONS ---------------//

void	print_2d(char **s)
{
	while (*s)
		ft_printf("%s\n", *s++);
}

void	print_diclst(t_diclst	*dic_lst)
{
	while (dic_lst)
	{
		ft_printf("%s=%s\n", dic_lst->name, dic_lst->value);
		dic_lst = dic_lst->next;
	}
}

//------------------------------------------------//

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
