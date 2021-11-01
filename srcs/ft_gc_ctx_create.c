/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_ctx_create.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdam <qdam@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 02:20:24 by qdam              #+#    #+#             */
/*   Updated: 2021/10/30 02:27:35 by qdam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gc_internal.h"

t_ctx	*ft_gc_ctx_new(int obtain_wild)
{
	t_ctx	*ctx;
	t_ctx	**p_local;

	ctx = malloc(sizeof(t_ctx));
	if (ctx)
	{
		_init_ctx(ctx);
		p_local = _get_local_start();
		ctx->next = *p_local;
		*p_local = ctx;
		if (obtain_wild)
			ft_gc_ctx_obtain_wild(ctx);
	}
	return (ctx);
}

t_ptr	ft_gc_ctx_malloc(t_ctx *ctx, size_t sz)
{
	t_ptr	mem;

	if (sz == 0)
		return (NULL);
	mem = malloc(sizeof(t_ptr) + sz);
	if (mem == NULL)
		return (NULL);
	if (ctx == NULL)
		ctx = _get_global();
	_insert_managed_ptr(ctx, mem);
	return (mem + sizeof(t_ptr));
}

int	ft_gc_ctx_manage(t_ctx *ctx, t_ptr p, void (*free_func)(t_ptr ))
{
	t_unman	*unman_data;

	if (p == NULL)
		return (0);
	if (ctx == NULL)
		ctx = _get_global();
	if (free_func == NULL)
		free_func = &free;
	unman_data = malloc(sizeof(t_unman));
	if (unman_data == NULL)
		return (-1);
	unman_data->addr = p;
	unman_data->free_func = free_func;
	unman_data->next = ctx->unman;
	ctx->unman = unman_data;
	return (0);
}
