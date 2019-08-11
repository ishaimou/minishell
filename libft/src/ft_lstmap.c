/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:29:22 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:29:23 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*new_lst;
	t_list	*ind[2];

	if (!lst || !f)
		return (NULL);
	new_lst = (t_list*)malloc(sizeof(t_list));
	ind[0] = new_lst;
	if (!new_lst)
		return (NULL);
	while (lst->next)
	{
		new_lst->next = (t_list*)malloc(sizeof(t_list));
		ind[1] = (*f)(lst);
		new_lst->content = malloc(ind[1]->content_size);
		ft_memcpy(new_lst->content, ind[1]->content, ind[1]->content_size);
		new_lst->content_size = ind[1]->content_size;
		new_lst = new_lst->next;
		lst = lst->next;
	}
	new_lst->next = NULL;
	ind[1] = (*f)(lst);
	new_lst->content = malloc(ind[1]->content_size);
	ft_memcpy(new_lst->content, ind[1]->content, ind[1]->content_size);
	new_lst->content_size = ind[1]->content_size;
	return (ind[0]);
}
