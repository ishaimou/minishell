/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:47:24 by ishaimou          #+#    #+#             */
/*   Updated: 2020/02/02 07:19:24 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int signo)
{
	(void)signo;
	if (!g_jmp_flag)
		return ;
	siglongjmp(g_sig_env, SIG_JMP);
}

int		main(int ac, char *av[], char *env[])
{
	t_minishell		msh;

	init_msh(&msh, ac, av, env);
	signal(SIGINT, sigint_handler);
	while (TRUE)
	{
		if (sigsetjmp(g_sig_env, 1) == SIG_JMP)
		{
			init_qparam(&msh);
			ft_putchar('\n');
		}
		g_jmp_flag = TRUE;
		msh.sflag = FALSE;
		prompt_dir(&msh);
		read_cmd(&msh);
		parse_exec_cmd(&msh);
		ft_strdel(&msh.line);
		free_dbl(&msh.cmds);
	}
	return (0);
}
