/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 04:44:54 by ishaimou          #+#    #+#             */
/*   Updated: 2020/02/02 07:18:23 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <setjmp.h>
# include <fcntl.h>
# include "libft.h"

# define TRUE 1
# define FALSE 0
# define ENV_LST 0
# define VAR_LST 1
# define NO_FILE_DIR 1
# define NO_PERM 2
# define NO_FOUND 3
# define BUILTIN_NUM 9
# define CWD_BUF_SIZE 2048
# define SIG_JMP 1337
# define PATH "./alias.config"
# define STDIN 0

static sigjmp_buf				g_sig_env;
static volatile sig_atomic_t	g_jmp_flag = 0;

typedef struct s_minishell	t_minishell;

typedef void	(*t_builtin_func) (t_minishell *, int);

typedef struct		s_diclst
{
	char			*name;
	char			*value;
	struct s_diclst	*next;
}					t_diclst;

struct				s_minishell
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
	t_builtin_func	*funct_tab;
};

void				builtin_cd(t_minishell *msh, int ind);
void				builtin_exit(t_minishell *msh, int ind);
void				builtin_echo(t_minishell *msh, int ind);
void				builtin_env(t_minishell *msh, int ind);
void				builtin_setenv(t_minishell *msh, int ind);
void				builtin_unsetenv(t_minishell *msh, int ind);
void				builtin_source(t_minishell *msh, int ind);
void				builtin_alias(t_minishell *msh, int ind);
void				builtin_help(t_minishell *msh, int ind);
void				init_msh(t_minishell *msh, int ac, char **av, char **env);
void				init_qparam(t_minishell *msh);
void				prompt_dir(t_minishell *msh);
int					is_builtin(t_minishell *msh, char *cmd_name);
t_diclst			*get_diclst_val(t_minishell *msh, char *name, int type);
void				add_diclst(t_minishell *msh, t_diclst **dic_lst,
		char *name, char *value);
void				set_envlst(t_minishell *msh, char **env);
char				**set_env(t_minishell *msh);
int					set_varlst(t_minishell *msh, char *arg);
void				handle_exp(t_minishell *msh);
void				simplify_cmd(char **cmd);
void				rm_trailing_slash(char **path);
char				*home_to_tild(t_minishell *msh, char *str);
void				free_dbl(char ***tab);
void				free_msh(t_minishell *msh);
void				free_funct_tab(t_builtin_func **funct_tab);
void				free_diclst(t_diclst **begin);
void				malloc_error(t_minishell *msh);
void				fork_error(t_minishell *msh);
void				gnl_error(void);
int					check_err(t_minishell *msh, int mode);
void				parse_exec_cmd(t_minishell *msh);
void				launch_cmd(t_minishell *msh, int ind);
void				read_cmd(t_minishell *msh);
void				realloc_args(t_minishell *msh);
void				split_n_join(t_minishell *msh);
char				**split_alias(t_minishell *msh);
int					fetch_alias(t_minishell *msh);
void				print_chars(char *str);

#endif
