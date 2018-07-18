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

#define raii_glueimpl(x, y) x ## y
#define raii_glue(x, y) raii_glueimpl(x, y)

#define safe_return                                             \
  while (raii_lifetime_list != NULL) {                     \
    destruct_raii_lifetime(raii_lifetime_list->elem);  \
    raii_lifetime_list = raii_lifetime_list->next;   \
  }                                                             \
  return                                                        \

#define raii_with(var_decl, init, destr)                                   \
  while(1)                                                              \
    if(0)                                                               \
    raii_glue(__using_finished, __LINE__):                         \
      break;                                                            \
    else                                                                \
      for(struct raii_lifetime_list_t _tmp = {.elem.resource = init, .elem.destructor = destr, .next = raii_lifetime_list};;) \
        for(var_decl = _tmp.elem.resource;;)                            \
          if (1) {                                                      \
            _tmp.next = raii_lifetime_list;                        \
            goto raii_glue(__using_setup, __LINE__);               \
          } else                                                        \
          raii_glue(__using_setup, __LINE__):                      \
            for(struct raii_lifetime_list_t *raii_lifetime_list = &_tmp;;) \
              if(1){                                                    \
                goto raii_glue(__using_body, __LINE__);            \
              } else                                                    \
                while (1)                                               \
                  if (1){                                               \
                    destruct_raii_lifetime(raii_lifetime_list->elem); \
                    goto raii_glue(__using_finished, __LINE__);    \
                  } else                                                \
                  raii_glue(__using_body, __LINE__):


#endif // RAII_WITH_H
