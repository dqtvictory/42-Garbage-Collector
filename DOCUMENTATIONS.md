- [How it works](#how-it-works)
- [Memory and performance overhead](#memory-and-performance-overhead)
- [Context](#context)
- [Terms used](#terms-used)
- [Detailed functions](#detailed-functions)
		- [Global context functions](#global-context-functions)
		- [Local context functions](#local-context-functions)
- [Last notes](#last-notes)

# How it works
The 42GC library wraps around the classical `malloc` and `free` functions of C's `stdlib.h` in order to track and free properly all allocated pointers. Under the hood, allocated pointers are stored inside a context's linked list (more on contexts in the next sections) on the First In, Last Out (FILO) basis, just like a stack. The library's `free_all` functions (there are two) ensure that all pointers stored in the list are properly freed.

# Memory and performance overhead
Great convenience comes with some cost, so the design of the library is balanced between memory and performance overheads.

Memory-wise, each successful call to the library's own `malloc` functions (there are two) allocates the amount of memory requested, plus the size of one pointer (4 or 8 bytes) to point to the next allocation in the linked list.

Performance-wise, allocating using the library's `malloc` versus classical `malloc` is nearly identical. However, the library's version of `free` can be considerably slower than the classical one, as it has to search exhaustively in the list until it finds the correct pointer. If a wrong pointer is passed, the list is traversed entirely without any after-effect (so no double free is possible). Faster approach may be possible using a binary tree rather than the linked list, but that would double the memory overhead for almost no gain, since users of this library are **strongly discouraged** to manually free using one of the `free` functions (what is the point of using a GC?).

To tackle the risk of having memory stacking up during the program's runtime (which is considered a dangerous kind of leaks) while avoiding manual free, the concept of context is introduced.

# Context
One of the core concepts of 42GC is context. A context groups together a bunch of pointers that are needed in one smaller section/segmentation of the program and manage them. When a context has done serving its purpose, it can be freed and destroyed altogether with its pointers.

A quick example would be a program that reads the contents of some files, performs some parsing tasks, passes the parsed data to a graphic library, then finally transforms the data to graphics. At each step, dynamic allocations must be made but no longer needed at the step that follows. Using simply a bunch of the library's `malloc` and only one `free_all` at the program's exit point would prevent leaks at exit, but unnecessary memory consumption during the program's runtime can be enormous and dangerous as it may eats up all of the computer's resources. A recommended approach would be to create a context for each step, allocate memory into the context's management, perform tasks, *release* some pointers that are necessary for the next step, then finally free up all memory managed under the context. A new context can *obtain* and manage the previously released pointers. Released pointers are not affected by a `free_all` call of the context, and therefore can be passed from contexts to contexts without risk of losing data after context's deallocation. Alternatively, if the two contexts are created and not destroyed at a given moment, pointers can be *transfered* directly without having to release and obtain.

A context can also manage any pointers that are not returned by the library's `malloc`, as long as it is dynamically allocated and a dedicated function to free it is provided. This function can be the classical `free` if the pointer is allocated with the classical `malloc`, or anything else that has the same signature as `free` and properly deallocates the pointer. Note that 42GC cannot ensure the zero leaks principle for these pointers, as it has no control over the provided function except calling it.

Technically speaking, calling the library's `malloc` and `free` functions without attaching to any created context will act to the *global context* of the library. This global context is created, managed and deallocated automatically without programmers' intervention. Programmers can refer to this context by passing a `NULL` pointer into functions that require a pointer to a context (see section below).

For a clearer use of context, refer to [the second example](examples/ex02_context.c).

# Terms used
Some terms are specific to the library, including:
- **Context**: a group of pointers that are managed together. There are global and local contexts (created by programmers)
- **Managed pointer**: a pointer that is managed under a context, usually allocated by the library's `malloc` function
- **Unmanaged pointer**: opposite to a managed pointer. A unmanaged pointer can be made managed and therefore deallocated together with the already managed ones
- **Wild pointer**: a managed pointer but without a managing context. A managed pointer becomes a wild pointer after being released from a context

# Detailed functions
All functions offered by the 42GC library are prefixed by `ft_gc_`.

### Global context functions
- `void *ft_gc_malloc(size_t sz)`: Work just like `malloc`. The returned pointers (if not `NULL`) is managed under the global context
- `void ft_gc_free(void *p)`: Manually free the managed pointer `p` in the global context. If `p` is `NULL` or is not managed by the global context, do nothing (and therefore avoid double-freeing). *Frequent use of this function is NOT recommended*
- `int ft_gc_manage(void *p, void (*free_func)(void *))`: Try having the global context manage the unmanaged pointer `p` whose dedicated free function is `free_func`. Return 0 on success, otherwise -1. This function allocates a dedicated structure to manage the unmanaged pointer and therefore can fail if the allocation fails
- `void ft_gc_free_all(void)`: Free all pointers managed under the global context. Note that any wild pointers not obtained by a local context are also freed, because this function is designed to be called at the program's exit point
- `void ft_gc_destroy_all_contexts(void)`: Free all pointers in all **LOCAL** contexts and destroy the contexts. Pointers in the global context are not affected and must be freed using `ft_gc_free_all`

### Local context functions
The first few functions below are exactly the same as those of the global context, except that they require passing the pointer to the context being manipulated as the first argument. Passing `NULL` as the context's pointer will refer to the global context.
- `void *ft_gc_ctx_new(int obtain_wild)`: Create a new context and return its pointer if succeeds, or `NULL` otherwise. Pass a non-zero integer as argument to make the new context obtain the wild pointers, or 0 otherwise
- `void *ft_gc_ctx_malloc(void *ctx, size_t sz)`: Just like the corresponding global context function
- `void ft_gc_ctx_free(void *ctx, void *p)`: Just like the corresponding global context function
- `int ft_gc_ctx_manage(void *ctx, void *p, void (*free_func)(void *))`: Just like the corresponding global context function
- `void ft_gc_ctx_free_all(void *ctx)`: Just like the corresponding global context function
- `void ft_gc_ctx_destroy(void *ctx)`: Free all pointers of the context `ctx` and destroy any resources associated with the context
- `void ft_gc_ctx_release(void *ctx, void *p)`: Release pointer `p` from `ctx` and make it a wild pointer
- `void ft_gc_ctx_release_all(void *ctx)`: Release all pointers managed under `ctx` and make them wild pointers
- `void ft_gc_ctx_obtain_wild(void *ctx)`: Make `ctx` obtain all wild pointers
- `void ft_gc_ctx_transfer(void *ctx_des, void *ctx_src, void *p)`: Transfer `p` from a the context `ctx_src` to `ctx_des`. Do nothing if `p` is `NULL` or is not managed under `ctx_src`
- `void ft_gc_ctx_transfer_all(void *ctx_des, void *ctx_src)`: Transfer all pointers from `ctx_src` to `ctx_des`

# Last notes
C is a powerful language yet it cannot prevent programmers from stepping on their own toe, and neither can 42GC. There are far fewer ways to go right than to go wrong in C, so there are some rules to remember:
- Always NULL-check dynamic allocations
- Lost pointers are not problematic as long as it is managed by the GC
- This library is a GC, so avoid any unnecessary manual freeing and let the GC do its jobs
- Calling the classical `free` with the pointer returned by one of the library's `malloc` functions results in undefined behaviors (usually crash)
- If you are a 42 student, you are encouraged to thoroughly inspect the code then make changes to the code to fit your own purposes