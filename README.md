# **raii**

A simple library to provide RAII (or something very much like it) in standard-compliant C99, using `raii_with(resource, initializer, destructor) { ... }`-syntax:

```C
#include "raii/raii.h"

int main() {

  raii_with(int *myint, safe_malloc(sizeof(int)), free){

    *myint = 10;
    printf("myint is located at %p and contains: %d\n", myint, *myint);
    printf("%s\n", __func__);
  }

  int res = safe_return_example();
  printf("res: %d\n", res);

printf("Test\n");
}

```


## Huh?

Resource Acquisition Is Initialization (or, if you prefer, Resource Relinquishment is Destruction) 
is a pattern that C++ and some other languages use to ensure that a resource is automatically released once you are done
working with it.

The **raii** library for C99 tries to emulate this, by letting you write `raii_with` statements 
that are similar to the `with`-statements of e.g. Python.

The advantages over managing resources manually are:

- Manual management is error-prone:
  - Checking for proper initialization or early-fail on error requires a linear amount of cleanup lines per nested resource if writing this using if-statements.
  - The alternative, using `goto`s to labels at the end of the function, is prone to mistakes when new content is added to the function or the internal order of resources is swapped.
- Using an explicit lifetime-stack object means that you need to conceptually work with this lifetime-stack all the time.

Instead, **raii**:
- is built such that swapping the order of the resources will swap the order of the clean-up logic for you.
- Variables introduced using `raii_with` only exist in the block you provide. Referencing after the block will result in a compiler error, so that's a whole lot of bugs you don't need to worry about anymore!
- creates a hidden lifetime 'stack' (which is implemented as a linked-list), which you don't have to care about, because all you have to care about 
is that you can just use `safe_return` to return early from within a `raii_with`-block.
- Use `continue` or `break` to jump to the end of the current `raii_with`-block.

## How does it work?

**raii** uses a custom control structure macro to ensure you can pass any code block to the `raii_with`-statement.

For more information, you are encouraged to read the inline documentation in the `raii/raii.h` file.

For more information about writing Custom Control Structure Macro's, see my related library [exceptional](https://github.com/Qqwy/c_exceptional#how-does-it-work-or-demystifying-the-black-magic) that introduces block-style exceptions to C99,
and the there-linked further resources as well.


## Gotcha's 

1. Do not use `return` from within `raii_with`, but only `safe_return`, because otherwise the destructors will not be run.
2. Do not perform pointer-swaps with `var_decl`; the destructor will still be run on the original structure, because `raii` keeps its own reference to the resource.

I believe both of these things are very logical and easy to keep in mind. (resulting in **raii** being a near-leak-free abstraction). If you know of a way to further reduce these without introducing new ones, please share it!


## Acknowledgements

I'd like to thank:
- Simon Tatham for his [wonderful guide to creating your own Custom Control Structure Macros](https://www.chiark.greenend.org.uk/%7Esgtatham/mp/)
- tsoding/reximcut for the interesting approach to using lifetimes for resource management [youtube video](https://www.youtube.com/watch?v=ncRv9fCxt_w) and subsequent [reddit discussion](https://www.reddit.com/r/C_Programming/comments/8ytihi/how_i_manage_resources_in_pure_c_life_time/).
