#ifndef RAII_WITH_H
#define RAII_WITH_H

#include <stdlib.h>


/**
 * A pointer to a single resource accompanied with info to destruct it once it is no longer needed.
 */
struct raii_lifetime_elem {
  void *resource;
  void (*destructor)(void *);
};


/** Helper function that properly destructs the resource stored in a `raii_lifetime_elem` struct. */
void destruct_raii_lifetime(struct raii_lifetime_elem elem);

/**
 * A linked list of raii-structures. Used to build a statically-allocated 'stack' of nested resources,
 * so we can clean up all of them quickly (see `safe_return`).
 */
struct raii_lifetime_list_t {
  struct raii_lifetime_elem elem;
  struct raii_lifetime_list_t* next;
};

/**
 * This variable will be shadowed when `raii_with` is called, but first it becomes the tail of the innermore variable.
 * This means that we end up with a linked list of all resources that are currently in use in the current invocation of the function.
 * (with the innermost resource on top).
 **/
struct raii_lifetime_list_t *raii_lifetime_list;

/**
 * Macro to glue together two parts; mostly used to make unique labels and variable names
 * based on the current line number, by using __LINE__ as `y`
 *
 * The extra 'impl' redirect is there to make sure the arguments are macro-expanded before gluing them.
 */
#define raii_glueimpl(x, y) x ## y
#define raii_glue(x, y) raii_glueimpl(x, y)

/**
 * Used to quickly return (either a value or void) from a (deeply nested) `raii_with` statement.
 * !Important! Do not return the contents of a resource acquired from `raii_with` directly, but store these in a separate variable first!
 * This is necessary because the resources will first be destructed, so otherwise you end up referencing something that no longer exists.
 * (like dereferencing a NULL-pointer)
 */
#define safe_return                                   \
  while (raii_lifetime_list != NULL) {                \
    destruct_raii_lifetime(raii_lifetime_list->elem); \
    raii_lifetime_list = raii_lifetime_list->next;    \
  }                                                   \
  return                                              \

/**
 * Custom Control Structure Macro to provide Resource Acquisition Is Initialization (and Resource Relinquishment is Destruction).
 *
 * Use this to run a block of code with `var_decl` initialized to `init`, where at the end of the block (or at an earlier `safe_return`),
 * the passed `destr`-function will automatically be called with the given resource.
 *
 * Gotcha's:
 * 1. Do not use `return` from within `raii_with`, but only `safe_return`, because otherwise the destructors will not be run.
 * 2. Do not perform pointer-swaps with `var_decl`; the destructor will still be run on the original structure, because `raii` keeps its own reference to the resource.
 */
#define raii_with(var_decl, init, destr)                                \
  while(1) /* i.c.m. break on l.4, so we can jump past the user-supplied block */ \
    if(0)                                                               \
    raii_glue(__raii_with_finished, __LINE__):                              \
      break;                                                            \
    else                                                                \
      /* initialize _tmp lifetime list elem so replacement `raii_lifetime_list` can have previous one as tail. */ \
      for(struct raii_lifetime_list_t _tmp = {.elem.resource = init, .elem.destructor = destr, .next = raii_lifetime_list};;) \
        /* initialize user-supplied variable name */                    \
        for(var_decl = _tmp.elem.resource;;)                            \
          if (1) {                                                      \
            /* Fill `_tmp`'s tail before `raii_lifetime_list` is shadowed */ \
            _tmp.next = raii_lifetime_list;                             \
            goto raii_glue(__raii_with_setup, __LINE__);                    \
          } else                                                        \
          raii_glue(__raii_with_setup, __LINE__):                           \
            /* Shadow `raii_lifetime_list` with inner version */        \
            for(struct raii_lifetime_list_t *raii_lifetime_list = &_tmp;;) \
              if(1){                                                    \
                goto raii_glue(__raii_with_body, __LINE__);                 \
              } else                                                    \
                while (1) /* so break works as expected */              \
                  while (1) /*so continue works as expected */          \
                    if (1){                                             \
                      /*after the else-block (or break or continue), destruct and quit*/ \
                      destruct_raii_lifetime(raii_lifetime_list->elem); \
                      goto raii_glue(__raii_with_finished, __LINE__);       \
                    } else                                              \
                    raii_glue(__raii_with_body, __LINE__):


#endif // RAII_WITH_H
