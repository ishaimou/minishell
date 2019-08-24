#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <setjmp.h>
# include <fcntl.h>
# include "libft.h"

# include <stdio.h> //!!!!!!

# define IS_TRUE 1
# define NO_FILE_DIR 1
# define NO_PERM 2
# define NO_FOUND 3
# define CWD_BUF_SIZE 2048 
# define BUILTIN_NUM 8 

static sigjmp_buf 				sig_env;
static volatile sig_atomic_t	jmp_flag = 0;

typedef struct s_minishell	t_minishell;

typedef void (*builtin_func) (t_minishell *, int);

typedef struct		s_diclst
{
	char			*name;
	char			*value;
	struct s_diclst	*next;
}					t_diclst;

struct		 		s_minishell
{
	int				sflag;
	int				qflag;
	int				dflag;
	int				quote;
	int				dquote;
	int				quoted;
	int				argc;
	char			*line;
	char			**args;
	char			**cmds;
	char			**env;
	char			*cmd_path;
	char			*user;
	char			*home;
	char			*oldpwd;
	char			*pwd;
	char			**builtin_name;
	t_diclst		*env_lst;
	t_diclst		*var_lst;
	builtin_func	*funct_tab;
};

void		builtin_cd(t_minishell *msh, int ind);
void		builtin_exit(t_minishell *msh, int ind);
void		builtin_echo(t_minishell *msh, int ind);
void		builtin_env(t_minishell *msh, int ind);
void		builtin_setenv(t_minishell *msh, int ind);
void		builtin_unsetenv(t_minishell *msh, int ind);
void		builtin_source(t_minishell *msh, int ind);
void		builtin_alias(t_minishell *msh, int ind);
void		init_msh(t_minishell *msh, int ac, char **av, char **env);
char		**set_env(t_minishell *msh);
void		prompt_dir(t_minishell *msh);
int			get_argc(char **args);
int			is_builtin(t_minishell *msh, char *cmd_name);
t_diclst	*get_diclst_val(t_minishell *msh, char *name, int type);
void		add_diclst(t_minishell *msh, t_diclst **dic_lst, char *name, char *value);
void		set_envlst(t_minishell *msh, char **env);
char		*ft_strjoin_sep(const char *s1, const char *s2, char *sep);
void		free_msh(t_minishell *msh);
void		free_dbl(char ***tab);
void		init_qparam(t_minishell *msh);
void		rm_trailing_slash(char **path);
void		simplify_cmd(t_minishell *msh);
char		*home_to_tild(t_minishell *msh, char *str);
void		simplify_cmd(t_minishell *msh);
void		handle_exp(t_minishell *msh);
void		get_value(t_minishell *msh, char **arg, char *ptr);
int			set_varlst(t_minishell *msh, char *arg);
void		print_chars(char *str);
void		print_2d(char **str); //!!!!!!
void		free_funct_tab(builtin_func **funct_tab);
void		free_diclst(t_diclst **begin);
void		malloc_error(t_minishell *msh);
void		parse_exec_cmd(t_minishell *msh);
#endif
