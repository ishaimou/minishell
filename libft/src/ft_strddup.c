#include "libft.h"

char	**ft_strddup(char **str)
{
	char	**ddup;
	int		argc;
	int		i;

	ddup = NULL;
	if (str)
	{
		argc = get_argc(str);
		ddup = (char**)malloc(sizeof(char*) * (argc + 1));
		ddup[argc] = NULL;
		i = 0;
		while (str[i])
		{
			ddup[i] = ft_strdup(str[i]);
			i++;
		}
	}
	return (ddup);
}
