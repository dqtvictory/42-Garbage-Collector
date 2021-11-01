# 42 Garbage Collector (42GC)

## Introduction
This project provides a basic implementation of a Garbage Collector for the C programming language that is normally already built into other higher level languages such as Java, C# or Python.

## What is a Garbage Collector (GC)
In simple words, a GC helps programmers to clean up the memory that their program allocates dynamically, so they don't have to. In C, memory has to be managed manually using the `*alloc` functions family and `free`. Memory leaks occur when memory allocated using `*alloc` is not `free`'d. The 42GC library aims to ease programmers' life by `free`ing allocated memory automatically, while still giving them full control over memory management tasks.

## Why 42
This library is made in order to, but not limited to, support other projects in C within the [42 school network](https://42.fr/)'s Common Core. The code conforms to the latest 42 norm (approved by the [Norminette V3](https://github.com/42School/norminette)) and can be easily integrated into any 42 projects that allow `malloc` and `free`, by simply adding the source code to the user's `libft` (the first 42 project).

## Start using 42GC
- Clone this repository and copy all files from the `srcs` folder to your project's folder
- Include the `ft_gc.h` header file in your code
- Add all source files (`ft_gc_*.c`) to your Makefile
- Simply replace all your `malloc`'s with `ft_gc_malloc` everywhere in your project, then remove (or put into comments) all `free`'s. The removal of `free`'s is critical, otherwise passing a pointer returned by `ft_gc_malloc` to `free` will crash your program. Call `ft_gc_free_all()` **one single time** at the exit point of the program to free all dynamic allocations. That's it.

## Compile the library
- The source code in the `srcs` folder can be compiled using the normally imposed flags at 42 `-Wall -Werror -Wextra`
- Running `make` from the repository's root compiles the code to a static library. The resulting files `libftgc.a` and `ft_gc.h` are stored inside the `lib` folder after compilation
- Compilation is done using the `clang` compiler. If you prefer `gcc` or something else, change it in the Makefile's `CC` variable. For debugging, add `-g` to the `CFLAGS` variable
- Some examples are included as commented code. Running `make examples` will compile them to binaries and place them inside the folder `examples`

## Documentations
If your program is small, runs only for a short duration and consumes little memory, `ft_gc_malloc` and `ft_gc_free_all` are all you need. However, the library is much more capable than that. For detailed information and advanced features of 42GC, refer to the [Documentations](DOCUMENTATIONS.md).

## Reporting issues
Please report any issues with the project, especially norm errors, to the Issue section of this project.