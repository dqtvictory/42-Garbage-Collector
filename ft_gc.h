/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdam <qdam@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 13:54:12 by qdam              #+#    #+#             */
/*   Updated: 2021/10/20 00:24:24 by qdam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GC_H
# define FT_GC_H

# include <stddef.h>

// Work just like malloc. Returns a raw pointer if sz bytes can be allocated,
// or NULL if sz is zero or the allocation fails. Note: it is NOT safe to use
// `free` directly with the returned pointer, use `ft_gc_free` instead.
void	*ft_gc_malloc(size_t sz);

// Free the pointer returned by `ft_gc_malloc`. Passing a pointer that was not
// allocated using ft_gc_malloc or NULL will, fortunately, has no effect.
void	ft_gc_free(void *p);

// Clean up all allocations done by `ft_gc_malloc`.
void	ft_gc_free_all(void);

#endif
