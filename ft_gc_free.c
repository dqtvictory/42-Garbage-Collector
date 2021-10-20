/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdam <qdam@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 15:00:42 by qdam              #+#    #+#             */
/*   Updated: 2021/10/20 10:26:32 by qdam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gc_internal.h"

static inline void	free_start(t_ptr *p_start)
{
	t_ptr	start;

	start = *p_start;
	*p_start = *((t_ptr *)start);
	free(start);
}

void	ft_gc_free(t_ptr p)
{
	t_ptr	*p_start;
	t_ptr	cur;
	t_ptr	tmp;

	if (p == NULL)
		return ;
	p_start = get_ptr_start();
	cur = *p_start;
	while (cur && cur + sizeof(t_ptr) != p)
	{
		tmp = cur;
		cur = *((t_ptr *)cur);
	}
	if (cur == *p_start)
		free_start(p_start);
	else if (cur != NULL)
	{
		*((t_ptr *)tmp) = *((t_ptr *)cur);
		free(cur);
	}
}
