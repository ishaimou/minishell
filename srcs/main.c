/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:47:24 by ishaimou          #+#    #+#             */
/*   Updated: 2020/01/29 04:57:00 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int signo)
{
	(void)signo;
	if (!g_jmp_flag)
		return ;
	siglongjmp(g_sig_env, 1337);
}

int		main(int ac, char *av[], char *env[])
{
	t_minishell		msh;

	init_msh(&msh, ac, av, env);
	signal(SIGINT, sigint_handler);
	while (1337)
	{
		if (sigsetjmp(g_sig_env, 1) == 1337)
		{
			init_qparam(&msh);
			ft_putchar('\n');
		}
		g_jmp_flag = 1;
		msh.sflag = 0;
		prompt_dir(&msh);
		read_cmd(&msh);
		parse_exec_cmd(&msh);
		ft_strdel(&msh.line);
		free_dbl(&msh.cmds);
	}
	return (0);
}
