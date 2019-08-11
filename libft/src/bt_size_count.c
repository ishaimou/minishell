/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_size_count.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:19:23 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:19:24 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		bt_size_count(t_bt *root)
{
	if (!root)
		return (0);
	else
		return (1 + bt_size_count(root->left) + bt_size_count(root->right));
}
