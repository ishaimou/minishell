#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <sys/wait.h>
# include <signal.h>
# include <setjmp.h>
# include "libft.h"

# include <stdio.h> //!!!!!!

# define IS_TRUE 1
# define CWD_BUF_SIZE 1024
# define BUILTIN_NUM 2

static sigjmp_buf 				sig_env;
static volatile sig_atomic_t	jmp_flag = 0;

typedef int (*builtin_func)	(char **);

typedef	struct 		s_minishell
{
	char			*line;
	char			**args;
	char			*cmd_path;
	char			**env;
	char			**builtin_name;
	builtin_func	*funct_tab;
}					t_minishell;

int		builtin_cd(char	**args);
int		builtin_exit(char **args);

#endif
