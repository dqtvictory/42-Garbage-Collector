/*
 *	THIS EXAMPLE SHOWS THE MOST BASIC AND SIMPLEST FUNCTIONALITY
 *	OF 42GC WHILE WORKING WITH SMALL PROGRAMS.
 */

#include "ft_gc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALLOC	10	// Let's limit ourself to 10 allocs

void	free_then_exit(int exit_code);

int	main(int ac, char **av)
{
	int	n;

	if (ac != 2)
		return printf("Pass a number from 1 to %d to argument\n", MAX_ALLOC) == 0;
	else if (atoi(av[1]) <= 0 || atoi(av[1]) > MAX_ALLOC)
		n = 10;
	else
		n = atoi(av[1]);
	
	// First create a pointer to store n strings, using classical malloc
	char	**str_array = malloc(sizeof(char *) * n);
	if (!str_array)
		exit(1);
	// Add the classic malloc'ed pointer to the GC so it knows to free it.
	// The free_function argument can be a dedicated function that frees the pointer.
	// In case of NULL, the GC automatically chooses `free`.
	ft_gc_manage(str_array, NULL);

	for (int i = 0; i < n; ++i)
	{
		// Now use the GC's malloc function to track the allocation
		str_array[i] = ft_gc_malloc(sizeof(char) * 50);
		// Check for allocation fails. If this is the case, exit by calling
		// `ft_gc_free_all` before `exit`
		if (str_array[i] == NULL)
			free_then_exit(1);
		sprintf(str_array[i], "This string is allocated and managed. i=%d", i);
		puts(str_array[i]);

		// It is also possible to manually free a pointer using `ft_gc_free`.
		// This works for pointers alloc'ed using both `ft_gc_malloc` and
		// the classical `malloc` (as long as it is tracked by the GC of course)
		if (i % 4 == 0)
		{
			ft_gc_free(str_array[i]);
			// Try accessing it again will produce undefined behaviors. Uncomment
			// lines below to see.
			// puts("Undefined behavior incoming");
			// puts(str_array[i]);
		}
	}

	free_then_exit(0);
}

void	free_then_exit(int exit_code)
{
	// Call this function before exitting to ensure zero leaks
	ft_gc_free_all();
	exit(exit_code);
}
