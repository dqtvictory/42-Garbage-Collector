/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_ctx_delete.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdam <qdam@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 02:21:22 by qdam              #+#    #+#             */
/*   Updated: 2021/10/30 11:37:33 by qdam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gc_internal.h"

static int	_free_managed_ptr(t_ctx *ctx, t_ptr p)
{
	t_ptr	prev_ptr;

	prev_ptr = _find_managed_ptr(ctx, p);
	if (prev_ptr == NULL)
		return (0);
	if (prev_ptr == p)
		ctx->man = *((t_ptr *)p);
	else
		*((t_ptr *)prev_ptr) = *((t_ptr *)p);
	free(p);
	return (1);
}

static void	_free_unmanaged_ptr(t_ctx *ctx, t_ptr p)
{
	t_unman	*prev_unman;
	t_unman	*unman_p;

	prev_unman = _find_unmanaged_ptr(ctx, p);
	if (prev_unman == NULL)
		return ;
	if (prev_unman->addr == p)
	{
		unman_p = prev_unman;
		ctx->unman = unman_p->next;
	}
	else
	{
		unman_p = prev_unman->next;
		prev_unman->next = unman_p->next;
	}
	unman_p->free_func(unman_p->addr);
	free(unman_p);
}

void	ft_gc_ctx_free(t_ctx *ctx, t_ptr p)
{
	if (p == NULL)
		return ;
	if (ctx == NULL)
		ctx = _get_global();
	if (_free_managed_ptr(ctx, p - sizeof(t_ptr)) == 0)
		_free_unmanaged_ptr(ctx, p);
}

void	ft_gc_ctx_free_all(t_ctx *ctx)
{
	t_ptr	man_cur;
	t_ptr	man_tmp;
	t_unman	*unman_cur;
	t_unman	*unman_tmp;

	if (ctx == NULL)
		ctx = _get_global();
	man_cur = ctx->man;
	while (man_cur)
	{
		man_tmp = man_cur;
		man_cur = *((t_ptr *)man_cur);
		free(man_tmp);
	}
	unman_cur = ctx->unman;
	while (unman_cur)
	{
		unman_tmp = unman_cur;
		unman_cur = unman_cur->next;
		unman_tmp->free_func(unman_tmp->addr);
		free(unman_tmp);
	}
	ctx->man = NULL;
	ctx->unman = NULL;
}

void	ft_gc_ctx_destroy(t_ctx *ctx)
{
	t_ctx	**p_local;
	t_ctx	*cur;

	ft_gc_ctx_free_all(ctx);
	if (ctx && ctx != _get_wild())
	{
		p_local = _get_local_start();
		if (*p_local == ctx)
			*p_local = ctx->next;
		else
		{
			cur = *p_local;
			while (cur->next)
			{
				if (cur->next == ctx)
					break ;
				cur = cur->next;
			}
			cur->next = ctx->next;
		}
		free(ctx);
	}
}
