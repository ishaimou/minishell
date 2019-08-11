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

static sigjmp_buf 				sig_env;
static volatile sig_atomic_t	jmp_flag = 0;

typedef	struct s_minishell
{
	char		*line;
	char		**args;
	char		*cmd_path;
	char		**env;
}				t_minishell;



#endif
