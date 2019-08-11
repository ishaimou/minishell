/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_search_item.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:19:14 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:19:17 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_bt	*bt_search_item(t_bt *root, void *dataref, int (*cmpf)(void *, void *))
{
	if (!root || !cmpf(dataref, root->item))
		return (root);
	if (cmpf(dataref, root->item) < 0)
		return (bt_search_item(root->left, dataref, cmpf));
	return (bt_search_item(root->right, dataref, cmpf));
}
