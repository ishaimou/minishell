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
# define CWD_BUF_SIZE 2048 
# define BUILTIN_NUM 6 

static sigjmp_buf 				sig_env;
static volatile sig_atomic_t	jmp_flag = 0;

typedef struct s_minishell	t_minishell;

typedef void (*builtin_func) (t_minishell *);

typedef struct		s_envlst
{
	char			*name;
	char			*value;
	struct s_envlst	*next;
}					t_envlst;

struct		 		s_minishell
{
	int				argc;
	char			*line;
	char			**args;
	char			**cmds;
	char			**env;
	char			*cmd_path;
	char			*home;
	char			*oldpwd;
	char			*pwd;
	char			**builtin_name;
	t_envlst		*env_lst;
	builtin_func	*funct_tab;
};

void		builtin_cd(t_minishell *msh);
void		builtin_exit(t_minishell *msh);
void		builtin_echo(t_minishell *msh);
void		builtin_env(t_minishell *msh);
void		builtin_setenv(t_minishell *msh);
void		builtin_unsetenv(t_minishell *msh);
void		init_msh(t_minishell *msh, int ac, char **av, char **env);
char		**set_env(t_minishell *msh);
void		prompt_dir(void);
int			get_argc(char **args);
int			is_builtin(t_minishell *msh, char *cmd_name);
t_envlst	*get_envlst_val(t_minishell *msh, char *name);
void		add_envlst(t_envlst **env_lst, char **tab_env);
void		set_envlst(t_minishell *msh, char **env);
char		*ft_strjoin_sep(const char *s1, const char *s2, char *sep);
void		free_msh(t_minishell *msh);
void		free_dbl(char ***tab);


#endif
