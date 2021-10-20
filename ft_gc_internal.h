/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdam <qdam@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 13:57:18 by qdam              #+#    #+#             */
/*   Updated: 2021/10/20 10:27:13 by qdam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GC_INTERNAL_H
# define FT_GC_INTERNAL_H

# include <stdlib.h>

typedef void*	t_ptr;

t_ptr	ft_gc_malloc(size_t sz);
void	ft_gc_free(t_ptr p);
void	ft_gc_free_all(void);

t_ptr	*get_ptr_start(void);

#endif
