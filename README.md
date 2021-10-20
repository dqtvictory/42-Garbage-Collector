# 42 Garbage Collector (42GC)
## Introduction
This project provides a basic implementation of a Garbage Collector for the C programming language, that is normally already built into other higher level languages such as Java, C# or Python.

## What is a Garbage Collector (GC)
In simple words, a GC helps the programmer to clean up the memory that his/her program allocates dynamically, so he/she doesn't have to. In C, memory has to be managed manually using the `*alloc` family and `free`. Memory leaks occur when memory allocated using `*alloc` is not `free`'d. The 42GC aims to ease the programmer's life by `free`ing allocated memory automatically, while still giving him/her the control over the memory management tasks.

## Why 42
This project is made in order to, but not limited to, support other projects in C within the [42 school network](https://42.fr/)'s Common Core. The code conforms to the latest 42's norm (approved by [Norminette V3](https://github.com/42School/norminette)) and can be easily integrated into any 42 projects that allow `malloc` and `free`, by simply adding the source code to the user's `libft` (first 42 project).

## Start using 42GC
- Clone this repository to your C project's folder
- Include the `ft_gc.h` header file in your code
- If needed, add all source files (`ft_gc_*.c`) to your Makefile
- 42GC should compile normally with both `gcc` and `clang` compilers with usually imposed flags at 42 `-Wall -Werror -Wextra`

## Functions available
At the moment, 42GC offers 3 functions that manage memory allocations at the global scope of your program (more on scope later on). They are:
1. `void *ft_gc_malloc(size_t)`: a wrapper around the real `malloc` in `stdlib.h` that has the same functionality as the real one;
2. `void ft_gc_free(void *)`: a wrapper around the real `free` that also has the same functionality as the real function; and
3. `void ft_gc_free_all(void)`: free all resources allocated by `ft_gc_malloc`

## What 42GC has to offer
- The function `ft_gc_free` frees the memory allocated by `ft_gc_malloc`, just like `free` memory produced by `malloc`.
- Alternatively, you can forget completely about calling `ft_gc_free`and just call `ft_gc_free_all` at any point in the program, usually the exit point, to clean up the mess.
- `ft_gc_free_all` can be called as many times as needed, subsequent calls to `ft_gc_malloc` and `ft_gc_free` will still function correctly.

## How to use 42GC safely
C is a powerful language yet it cannot prevent the programmer from stepping on his/her own toe, and neither can 42GC. Therefore, to safely work with raw pointers using this project, some pre-cautions should be considered:

- NULL checking the pointer returned by `ft_gc_malloc`
```c
void *ptr = ft_gc_malloc(10);
if (ptr == NULL) // allocation fails
  do_something_when_alloc_fails();
```
- Prefer `malloc` and `free` if the memory allocated is being freed after a few lines of code later, and between these two calls there are a lot of other memory being allocated (due to 42GC's choice of implementation which causes worse performance)
```c
void *ptr = ft_gc_malloc(10);      // use malloc here...
use_ptr_and_alloc_1000_times(ptr);
alloc_100000_more_times();
ft_gc_free(ptr);                   // because too many allocs above
```
- It is NOT safe to `free` a pointer returned by `ft_gc_malloc`, because it will crash the program.
- `ft_gc_free_all` is great, but it's never a good idea to leave pointers hanging until the end after being use a while ago. Memory allocation can build up over time and take unnecessary amount of resource.
- You are free (and encouraged) to change the original code to fit your particuliar use (changing return type for example)

## Memory and performance overhead
Each successful call to `ft_gc_malloc` will allocate the amount of memory requested, plus the size of one pointer (4 or 8 bytes). This is due to the fact that 42GC stores allocated pointers using a linked list, and the extra pointer must be there to point to the next allocation in the list.

Performance-wise, since `ft_gc_malloc` doesn't cause too much lag over the normal `malloc`. However, `ft_gc_free` can be considerably slower than `free` since it has to search for the correct pointer in the list before being able to free it. The list is structured like a stack (FILO - first in last out), so the more recent the pointer is allocated, the faster `ft_gc_free` executes.

## You pay for the overhead, and you get...
- The convenience, of course
- Passing a wrong pointer to `ft_gc_free` will NOT cause the program to crash, including pointers not allocated by `ft_gc_malloc`, pointers already freed (double free), or NULL pointer. This may cause bugs that are hard to track (again, 42GC cannot prevent you from stepping on your toe)
- Normally lost pointers (happens when a pointer is no longer pointed to by a variable) can be freed using `ft_gc_free_all`
```c
void *ptr = ft_gc_malloc(10);
ptr = NULL;        // reference to the pointer returned above is lost
ft_gc_free(ptr);   // does nothing
ft_gc_free_all();  // 42GC still keeps the pointer alive, so it is freed at the end
```

## Allocation context

Although it is recommended that pointers should be freed once its purpose is over, `ft_gc_free` can be too manual and tedious, which defeats the convenience goal of 42GC. On the other hand, `ft_gc_free_all` may not always be possible to be called if the programmer is still having business with a few pointers but wishes to clean up the rest.

In a future version of 42GC, I will implement the concept of allocation **context** where a group of pointers are put together. Each context may serve a different purpose, and once done, the pointers can be freed and the context is destroyed with one function call.

In other words, a context can also be considered a scope whose life cycle can be longer than a classical scope (function or loop for example). In these terms, `ft_gt_malloc` allocates memory in the global scope.

## Reporting issues
Please report any issues with the project, especially norm errors, to the Issue section of this repository
