/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdam <qdam@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 13:57:18 by qdam              #+#    #+#             */
/*   Updated: 2021/10/30 02:38:29 by qdam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GC_INTERNAL_H
# define FT_GC_INTERNAL_H

# include <stdlib.h>

typedef void*	t_ptr;

typedef struct s_unmanaged
{
	t_ptr				addr;
	void				(*free_func)(t_ptr);
	struct s_unmanaged	*next;
}	t_unman;

typedef struct s_context
{
	t_ptr				man;
	t_unman				*unman;
	struct s_context	*next;
}	t_ctx;

/********************  USER FUNCTIONS  ********************/

t_ptr	ft_gc_malloc(size_t sz);
void	ft_gc_free(t_ptr p);
int		ft_gc_manage(t_ptr p, void (*free_func)(t_ptr ));
void	ft_gc_free_all(void);
void	ft_gc_destroy_all_contexts(void);
t_ctx	*ft_gc_ctx_new(int obtain_wild);
t_ptr	ft_gc_ctx_malloc(t_ctx *ctx, size_t sz);
void	ft_gc_ctx_free(t_ctx *ctx, t_ptr p);
int		ft_gc_ctx_manage(t_ctx *ctx, t_ptr p, void (*free_func)(t_ptr ));
void	ft_gc_ctx_free_all(t_ctx *ctx);
void	ft_gc_ctx_destroy(t_ctx *ctx);
void	ft_gc_ctx_release(t_ctx *ctx, t_ptr p);
void	ft_gc_ctx_release_all(t_ctx *ctx);
void	ft_gc_ctx_obtain_wild(t_ctx *ctx);
void	ft_gc_ctx_transfer(t_ctx *ctx_des, t_ctx *ctx_src, t_ptr p);
void	ft_gc_ctx_transfer_all(t_ctx *ctx_des, t_ctx *ctx_src);

/********************  INTERNAL FUNCTIONS  ********************/
/*
 *	Note to developer: to avoid nasty bugs and to make program crashes when we
 *	do something stupid, ctx == NULL does NOT mean ctx is global, unlike user
 *	functions' implementation where ctx == NULL almost always means ctx is being
 *	referred to as the global context. I.e, the GC should crash when we forget
 *	to check for NULL pointer in user functions' implementation.
 *	In general, internal functions should not test for error, only direct
 *	implementations are expected. Error checking should be done before, i.e. in
 *	user functions. Once we enter our internal realm, things should already work
 */

// Init to zero all fields in context
void	_init_ctx(t_ctx *ctx);

// Get the pointer to the global context
t_ctx	*_get_global(void);

// Get the pointer to the wild pointers' context
t_ctx	*_get_wild(void);

// Get the pointer to the start of list of local contexts
t_ctx	**_get_local_start(void);

// Insert a managed pointer to the start of managed list in ctx
void	_insert_managed_ptr(t_ctx *ctx, t_ptr p);

// Insert a unmanaged pointer to the start of unmanaged list in ctx
void	_insert_unmanaged_ptr(t_ctx *ctx, t_unman *unman);

// Search for p in the managed list of ctx then return the pointer that preceeds
// it in the list. If not found, return NULL. If p doesn't have a predecessor,
// i.e. it's the first in the list, return itself.
t_ptr	_find_managed_ptr(t_ctx *ctx, t_ptr p);

// Search for p in the unmanaged pointer list of ctx and return the pointer to
// the structure that contains the predecessor of p. If not found, return NULL.
// If p is the first in the list, return its own structure. 
t_unman	*_find_unmanaged_ptr(t_ctx *ctx, t_ptr p);

#endif
