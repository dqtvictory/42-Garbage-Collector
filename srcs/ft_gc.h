/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdam <qdam@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 13:54:12 by qdam              #+#    #+#             */
/*   Updated: 2021/10/31 00:58:57 by qdam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GC_H
# define FT_GC_H

# include <stddef.h>

/********************  GLOBAL CONTEXT  ********************/

// Work just like `malloc`. Return a raw pointer of sz bytes, or NULL if sz is
// zero or the allocation fails. Returned pointer is managed under the global
// context.
void	*ft_gc_malloc(size_t sz);

// Free a managed pointer in the global context. If it doesn't exist in the
// global context or is NULL, do nothing.
void	ft_gc_free(void *p);

// Add a unmanaged pointer previously allocated with malloc or by another
// library to the global context. A dedicated clean-up function pointer can be
// passed as the second argument, or NULL to clean up using the default `free`
// function. Return 0 on success, -1 on fail.
int		ft_gc_manage(void *p, void (*free_func)(void *));

// Free all pointers managed under the global context. Attention: unassigned
// wild pointers are also freed. If this is not desired, they should be obtained
// by a context before this function is called.
void	ft_gc_free_all(void);

// Completely destroy all contexts created by users and all pointers managed
// under these contexts. The global context is left untouched. Equivalent to
// calling ft_gc_ctx_destroy(ctx) for each created context.
void	ft_gc_destroy_all_contexts(void);

/********************  LOCAL CONTEXT  ********************/

// Create a new allocation context. Pass 1 as argument to make the new
// context try to obtain wild pointers (if any), otherwise 0. Return the created
// context's pointer if creation succeeds, or NULL otherwise.
void	*ft_gc_ctx_new(int obtain_wild);

// Return a raw pointer of sz bytes, or NULL if sz is zero or the allocation
// fails. Returned pointer is managed under the context ctx. If ctx is NULL,
// pointer is managed by the global context, which is equivalent to calling
// `ft_gc_malloc(sz)`.
void	*ft_gc_ctx_malloc(void *ctx, size_t sz);

// Free the pointer p managed under the context ctx. If ctx is NULL, the global
// context is considered. If p doesn't exist in the context or is NULL, do
// nothing.
void	ft_gc_ctx_free(void *ctx, void *p);

// Add a unmanaged pointer previously allocated with malloc or by another
// library to the context ctx. If ctx is NULL, the global context is considered.
// A dedicated clean-up function pointer can be passed as the second argument,
// or NULL to clean up using the default `free` function. Return 0 on success,
// -1 on fail.
int		ft_gc_ctx_manage(void *ctx, void *p, void (*free_func)(void *));

// Free all pointers managed under ctx, but the context is not destroyed. If ctx
// is NULL, the global context is considered. Note: `ft_gc_ctx_free_all(NULL)`
// and `ft_gc_free_all()` do NOT have the same effect because of wild pointers.
// See documentation of `ft_gc_free_all()` for more information.
void	ft_gc_ctx_free_all(void *ctx);

// Destroy all resources associated with the context ctx, including all of its
// pointers. ctx being NULL is equivalent to calling `ft_gc_ctx_free_all(NULL)`
// since the global scope cannot be destroyed.
void	ft_gc_ctx_destroy(void *ctx);

// Release pointer p from context ctx. The pointer then becomes a "wild" pointer
// which can be obtained and managed by another context. If ctx is NULL, the
// global context is considered. If p doesn't exist in the context or is NULL,
// do nothing.
void	ft_gc_ctx_release(void *ctx, void *p);

// Release all pointers managed under ctx. These pointers then become "wild" and
// can be obtained and managed by another context. If ctx is NULL, the global
// context is considered.
void	ft_gc_ctx_release_all(void *ctx);

// Make ctx obtain and manage all current wild pointers. If ctx is NULL, the
// global context is considered.
void	ft_gc_ctx_obtain_wild(void *ctx);

// Transfer pointer p from ctx_src to ctx_des. If one of the contexts is NULL,
// it is considered the global context. If p doesn't exist in ctx_src or is
// NULL or ctx_src equals ctx_des, do nothing.
void	ft_gc_ctx_transfer(void *ctx_des, void *ctx_src, void *p);

// Transfer all pointers from ctx_src to ctx_des. If one of the contexts is NULL
// it is considered the global context. If ctx_src equals ctx_des, do nothing.
void	ft_gc_ctx_transfer_all(void *ctx_des, void *ctx_src);

#endif
