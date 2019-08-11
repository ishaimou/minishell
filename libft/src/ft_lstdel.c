/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:28:34 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:28:36 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void*, size_t))
{
	t_list	*current_node;

	if (alst && del)
	{
		while (*alst != NULL)
		{
			current_node = *alst;
			del((*alst)->content, (*alst)->content_size);
			*alst = (*alst)->next;
			free(current_node);
		}
	}
}
