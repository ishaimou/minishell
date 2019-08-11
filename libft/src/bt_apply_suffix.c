/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_apply_suffix.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 04:45:18 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 04:45:20 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	bt_apply_suffix(t_bt *root, void (*applyf)(void *))
{
	if (root)
	{
		bt_apply_prefix(root->left, applyf);
		bt_apply_prefix(root->right, applyf);
		applyf(root->item);
	}
}
