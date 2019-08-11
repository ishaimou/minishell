/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_apply_prefix.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 04:45:05 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 04:45:06 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	bt_apply_prefix(t_bt *root, void (*applyf)(void *))
{
	if (root)
	{
		applyf(root->item);
		bt_apply_prefix(root->left, applyf);
		bt_apply_prefix(root->right, applyf);
	}
}
