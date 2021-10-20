/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdam <qdam@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 13:59:22 by qdam              #+#    #+#             */
/*   Updated: 2021/10/20 00:14:46 by qdam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gc_internal.h"

static inline void	append_to_list(t_ptr mem)
{
	t_ptr	*p_start;

	p_start = get_ptr_start();
	*((t_ptr *)mem) = *p_start;
	*p_start = mem;
}

t_ptr	ft_gc_malloc(size_t sz)
{
	t_ptr	mem;

	if (sz == 0)
		return (NULL);
	mem = malloc(sizeof(t_ptr) + sz);
	if (mem == NULL)
		return (NULL);
	append_to_list(mem);
	return (mem + sizeof(t_ptr));
}
