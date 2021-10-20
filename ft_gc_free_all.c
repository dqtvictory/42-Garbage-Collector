/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_free_all.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdam <qdam@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 15:26:37 by qdam              #+#    #+#             */
/*   Updated: 2021/10/20 10:24:41 by qdam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gc_internal.h"

void	ft_gc_free_all(void)
{
	t_ptr	*p_start;
	t_ptr	cur;
	t_ptr	tmp;

	p_start = get_ptr_start();
	cur = *p_start;
	while (cur)
	{
		tmp = cur;
		cur = *((t_ptr *)cur);
		free(tmp);
	}
	*p_start = NULL;
}
