/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 04:45:35 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 04:45:36 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	bt_free(t_bt **root, void (*freef)(void **))
{
	t_bt *tmp;

	tmp = *root;
	if (!*root)
		return ;
	if (tmp->left)
		bt_free(&tmp->left, freef);
	if (tmp->right)
		bt_free(&tmp->right, freef);
	freef(&tmp->item);
	free(tmp);
	*root = NULL;
}
