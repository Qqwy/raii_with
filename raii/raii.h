#ifndef RAII_WITH_H
#define RAII_WITH_H

#include <stdlib.h>



struct raii_with_lifetime_elem {
  void *resource;
  void (*destructor)(void *);
} ;

struct raii_with_lifetime_list_t {
  struct raii_with_lifetime_elem elem;
  struct raii_with_lifetime_list_t* next;
};

void destruct_raii_with_lifetime(struct raii_with_lifetime_elem elem);


struct raii_with_lifetime_list_t *raii_with_lifetime_list;

#define raii_with_glueimpl(x, y) x ## y
#define raii_with_glue(x, y) raii_with_glueimpl(x, y)

#define safe_return                                             \
  while (raii_with_lifetime_list != NULL) {                     \
    destruct_raii_with_lifetime(raii_with_lifetime_list->elem);  \
    raii_with_lifetime_list = raii_with_lifetime_list->next;   \
  }                                                             \
  return                                                        \

#define raii_with(var_decl, init, destr)                                   \
  while(1)                                                              \
    if(0)                                                               \
    raii_with_glue(__using_finished, __LINE__):                         \
      break;                                                            \
    else                                                                \
      for(struct raii_with_lifetime_list_t _tmp = {.elem.resource = init, .elem.destructor = destr, .next = raii_with_lifetime_list};;) \
        for(var_decl = _tmp.elem.resource;;)                            \
          if (1) {                                                      \
            _tmp.next = raii_with_lifetime_list;                        \
            goto raii_with_glue(__using_setup, __LINE__);               \
          } else                                                        \
          raii_with_glue(__using_setup, __LINE__):                      \
            for(struct raii_with_lifetime_list_t *raii_with_lifetime_list = &_tmp;;) \
              if(1){                                                    \
                goto raii_with_glue(__using_body, __LINE__);            \
              } else                                                    \
                while (1)                                               \
                  if (1){                                               \
                    destruct_raii_with_lifetime(raii_with_lifetime_list->elem); \
                    goto raii_with_glue(__using_finished, __LINE__);    \
                  } else                                                \
                  raii_with_glue(__using_body, __LINE__):


#endif // RAII_WITH_H
