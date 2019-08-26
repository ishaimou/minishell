#include "libft.h"

int		get_argc(char **args)
{
	int		argc;

	argc = 0;
	if (args)
	{	
		while (*args)
		{
			argc++;
			args++;
		}
	}
	return (argc);
}

