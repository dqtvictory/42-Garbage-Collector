/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_ctx_wild_manip.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdam <qdam@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 02:24:13 by qdam              #+#    #+#             */
/*   Updated: 2021/10/30 11:37:01 by qdam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gc_internal.h"

void	ft_gc_ctx_release(t_ctx *ctx, t_ptr p)
{
	ft_gc_ctx_transfer(_get_wild(), ctx, p);
}

void	ft_gc_ctx_release_all(t_ctx *ctx)
{
	ft_gc_ctx_transfer_all(_get_wild(), ctx);
}

void	ft_gc_ctx_obtain_wild(t_ctx *ctx)
{
	ft_gc_ctx_transfer_all(ctx, _get_wild());
}
