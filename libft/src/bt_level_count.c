/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_level_count.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 04:59:07 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:19:05 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		bt_level_count(t_bt *root)
{
	int		ldepth;
	int		rdepth;

	if (!root)
		return (0);
	else
	{
		ldepth = bt_level_count(root->left);
		rdepth = bt_level_count(root->right);
		if (ldepth > rdepth)
			return (1 + ldepth);
		else
			return (1 + rdepth);
	}
}
