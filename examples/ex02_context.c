/*
 *	THIS EXAMPLE SHOWS THE SOME BASIC WAYS TO
 *	WORK WITH CONTEXTS
 */

#include "ft_gc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*read_files(void *ctx);
char	*transform_content(void *ctx, char *content);
void	perform_calculation(void *ctx, char *content);
void	free_then_exit(int exit_code);

// Concatenate two strings into one new allocated string using classical malloc
static char	*strcat_malloc(char *s1, char *s2)
{
	char	*cat = malloc(strlen(s1) + strlen(s2) + 1);
	if (cat)
	{
		strcpy(cat, s1);
		strcpy(cat + strlen(s1), s2);
	}
	return (cat);
}

int	main()
{
	void	*ctx_read_files, *ctx_transform_content, *ctx_perform_calculation;

	// Create the first context. Pass 1 to argument to obtain current wild pointers.
	// For now there is none, so 0.
	ctx_read_files = ft_gc_ctx_new(0);
	if (ctx_read_files == NULL)
	{
		fputs("Error creating ctx_read_files", stderr);
		free_then_exit(1);
	}
	char	*content = read_files(ctx_read_files);

	// Create the second context. This time we ask the new context to obtain wild pointers
	// which were released by the first context. By doing so, we can safely free the rest
	// of the pointers managed by the first context because we are done working with them.
	ctx_transform_content = ft_gc_ctx_new(1);
	if (ctx_transform_content == NULL)
	{
		fputs("Error creating ctx_transform_content", stderr);
		free_then_exit(1);
	}
	char	*content_trans = transform_content(ctx_transform_content, content);

	// Finally create the third context. It also needs pointers from the second context
	// but instead of requiring that the second releases them before the creation of the
	// third, we can also perform a transfer as seen below.
	ctx_perform_calculation = ft_gc_ctx_new(0);
	if (ctx_perform_calculation == NULL)
	{
		fputs("Error creating ctx_perform_calculation", stderr);
		free_then_exit(1);
	}
	ft_gc_ctx_transfer_all(ctx_perform_calculation, ctx_transform_content);
	// After doing a total transfer, the source context is empty, so no need to call
	// ft_gc_ctx_free_all

	perform_calculation(ctx_perform_calculation, content_trans);

	free_then_exit(0);
}

// Read some files into memory. Return the string that contains the content of the files read
char	*read_files(void *ctx)
{
	// Let's pretend below are some files
	char	*file1 = "This is some fake file that we do not actually open.\n";
	char	*file2 = "Please, just pretend that this is a file somewhere in your drive.\n";
	char	*file3 = "Ok I promise this is the last file ok?\n";

	// Now let's read the files into memory
	char	*f1, *f2, *f3;
	// Use ft_gc_ctx_malloc to have ctx manage the pointer
	f1 = ft_gc_ctx_malloc(ctx, strlen(file1) + 1);
	f2 = ft_gc_ctx_malloc(ctx, strlen(file2) + 1);
	f3 = ft_gc_ctx_malloc(ctx, strlen(file3) + 1);
	if (f1 == NULL || f2 == NULL || f3 == NULL)
	{
		fputs("Error ft_gc_ctx_malloc'ing in read_files", stderr);
		free_then_exit(1);	
	}
	strcpy(f1, file1);
	strcpy(f2, file2);
	strcpy(f3, file3);
	
	// Two malloc'ed pointers below can be managed and later freed by the context
	char	*tmp12 = strcat_malloc(f1, f2);
	char	*content = strcat_malloc(tmp12, f3);
	if (tmp12 == NULL || content == NULL)
	{
		fputs("Error classical malloc'ing in read_files", stderr);
		free_then_exit(1);	
	}
	ft_gc_ctx_manage(ctx, tmp12, NULL);
	ft_gc_ctx_manage(ctx, content, &free);

	printf("Files read successfully:\n%s\n", content);

	// We are done reading the files, but from now we only need one pointer to continue
	// working, the rest is no longer needed. So before freeing all pointers from this context, we
	// have to make sure that the desired pointer is released and become wild pointers
	// in order to be obtained by another context later on.
	ft_gc_ctx_release(ctx, content);
	
	// Though not required, freeing all pointers of a context after done working with them is
	// always recommended to prevent unused memory building up over time, which can also be
	// considered leaks, and these leaks are much more problematic than unfreed pointers at the
	// end of a program
	ft_gc_ctx_free_all(ctx);
	return content;
}

// From the content, find all lower-case vowels (a, e, i, o, u) and make them UPPER-CASE.
// Let's assume that the original content is important and that we do not want to directly modify it,
// we will allocate a new string to store the transformed content, then return it.
char	*transform_content(void *ctx, char *content)
{
	char	*content_trans = ft_gc_ctx_malloc(ctx, strlen(content) + 1);
	size_t	i;
	for (i = 0; content[i]; ++i)
	{
		if (content[i] == 'a' || content[i] == 'e' || content[i] == 'i' || content[i] == 'o' || content[i] == 'u')
			content_trans[i] = content[i] - 32;
		else
			content_trans[i] = content[i];
	}
	content_trans[i] = 0;
	printf("Content transformed into:\n%s\n", content_trans);

	// It's also possible to free one pointer from the context.
	ft_gc_ctx_free(ctx, content);
	// Since we are performing a transfer of pointers outside this function, we are not releasing anything here.
	// Returning directly
	return content_trans;
}

// Doing nothing more special than counting the number of vowels
void	perform_calculation(void *ctx, char *content)
{
	int	v = 0;

	// Pointer arithmetic also works as normal
	for (; *content; ++content)
		if (*content == 'A' || *content == 'E' || *content == 'I' || *content == 'O' || *content == 'U')
			++v;
	printf("Number of vowels is %d\n", v);

	// An alternative to calling ft_gc_ctx_free_all is destroying the context using `ft_gc_ctx_destroy`.
	// By doing so systematically, calling `ft_gc_destroy_all_contexts` at program's exit is no longer necessary.
	ft_gc_ctx_destroy(ctx);
}

void	free_then_exit(int exit_code)
{
	// Free all pointers in the global context as well as unassigned wild pointers
	ft_gc_free_all();
	// Destroy all created contexts
	ft_gc_destroy_all_contexts();
	exit(exit_code);
}
