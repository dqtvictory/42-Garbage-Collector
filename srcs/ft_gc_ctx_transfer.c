/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_ctx_transfer.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdam <qdam@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 13:46:44 by qdam              #+#    #+#             */
/*   Updated: 2021/10/30 03:36:11 by qdam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gc_internal.h"

static inline int	_transferable(t_ctx **des, t_ctx **src)
{
	if (*des == *src)
		return (0);
	if (*des == NULL)
		*des = _get_global();
	if (*src == NULL)
		*src = _get_global();
	return (1);
}

static int	_transfer_managed_ptr(t_ctx *ctx_des, t_ctx *ctx_src, t_ptr p)
{
	t_ptr	prev_ptr;

	prev_ptr = _find_managed_ptr(ctx_src, p);
	if (prev_ptr == NULL)
		return (0);
	if (prev_ptr == p)
		ctx_src->man = *((t_ptr *)p);
	else
		*((t_ptr *)prev_ptr) = *((t_ptr *)p);
	_insert_managed_ptr(ctx_des, p);
	return (1);
}

static void	_transfer_unmanaged_ptr(t_ctx *ctx_des, t_ctx *ctx_src, t_ptr p)
{
	t_unman	*prev_unman;
	t_unman	*unman_p;

	prev_unman = _find_unmanaged_ptr(ctx_src, p);
	if (prev_unman == NULL)
		return ;
	if (prev_unman->addr == p)
	{
		unman_p = prev_unman;
		ctx_src->unman = unman_p->next;
	}
	else
	{
		unman_p = prev_unman->next;
		prev_unman->next = unman_p->next;
	}
	_insert_unmanaged_ptr(ctx_des, unman_p);
}

void	ft_gc_ctx_transfer(t_ctx *ctx_des, t_ctx *ctx_src, t_ptr p)
{
	if (p == NULL || _transferable(&ctx_des, &ctx_src) == 0)
		return ;
	if (_transfer_managed_ptr(ctx_des, ctx_src, p - sizeof(t_ptr)) == 0)
		_transfer_unmanaged_ptr(ctx_des, ctx_src, p);
}

void	ft_gc_ctx_transfer_all(t_ctx *ctx_des, t_ctx *ctx_src)
{
	t_ptr	man;
	t_unman	*unman;

	if (_transferable(&ctx_des, &ctx_src) == 0)
		return ;
	if (ctx_src->man)
	{
		man = ctx_src->man;
		while (*((t_ptr *)man))
			man = *((t_ptr *)man);
		*((t_ptr *)man) = ctx_des->man;
		ctx_des->man = ctx_src->man;
		ctx_src->man = NULL;
	}
	if (ctx_src->unman)
	{
		unman = ctx_src->unman;
		while (unman->next)
			unman = unman->next;
		unman->next = ctx_des->unman;
		ctx_des->unman = ctx_src->unman;
		ctx_src->unman = NULL;
	}
}
