/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_expmantis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 05:41:40 by ishaimou          #+#    #+#             */
/*   Updated: 2019/05/26 05:41:41 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

long				int_exp(long bin_exp, int bias)
{
	long			int_exp;
	long			ref;

	int_exp = 0;
	ref = (bias == D_BIAS) ? 2048 : 32768;
	while (ref >>= 1)
		int_exp += (bin_exp & ref);
	return (int_exp);
}

long long			int_mants(long bin_mants, int bias)
{
	long long		int_mants;
	int				ref;

	int_mants = 0;
	ref = (bias == D_BIAS) ? 53 : 63;
	while (--ref)
		int_mants += ((bin_mants >> ref) & 1);
	return (int_mants);
}
