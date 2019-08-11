/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_apply_infix.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 04:44:50 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 04:44:52 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	bt_apply_infix(t_bt *root, void (*applyf)(void *))
{
	if (root)
	{
		bt_apply_infix(root->left, applyf);
		applyf(root->item);
		bt_apply_infix(root->right, applyf);
	}
}
