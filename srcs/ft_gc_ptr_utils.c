/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_ptr_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdam <qdam@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 14:32:57 by qdam              #+#    #+#             */
/*   Updated: 2021/10/30 01:59:13 by qdam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gc_internal.h"

void	_insert_managed_ptr(t_ctx *ctx, t_ptr p)
{
	*((t_ptr *)p) = ctx->man;
	ctx->man = p;
}

void	_insert_unmanaged_ptr(t_ctx *ctx, t_unman *unman)
{
	unman->next = ctx->unman;
	ctx->unman = unman;
}

t_ptr	_find_managed_ptr(t_ctx *ctx, t_ptr p)
{
	t_ptr	cur;

	if (p == ctx->man)
		return (p);
	cur = ctx->man;
	if (cur == NULL)
		return (NULL);
	while (*((t_ptr *)cur))
	{
		if (*((t_ptr *)cur) == p)
			return (cur);
		cur = *((t_ptr *)cur);
	}
	return (NULL);
}

t_unman	*_find_unmanaged_ptr(t_ctx *ctx, t_ptr p)
{
	t_unman	*cur;

	if (ctx->unman == NULL || p == ctx->unman->addr)
		return (ctx->unman);
	cur = ctx->unman;
	while (cur->next)
	{
		if (p == cur->next->addr)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}
