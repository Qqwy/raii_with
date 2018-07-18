#include <stdio.h>
#include <stdlib.h>

void *safe_malloc(size_t size) {
  void * ptr = malloc(size);
  if(ptr == NULL) {
    perror("Error during malloc allocation");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

typedef struct {
  void *resource;
  void (*destructor)(void *);
} raii_with_lifetime_elem;

typedef struct raii_with_lifetime_list_t {
  raii_with_lifetime_elem elem;
  struct raii_with_lifetime_list_t* next;
} raii_with_lifetime_list_t;

raii_with_lifetime_list_t *raii_with_lifetime_list = NULL;


#define raii_with_glueimpl(x, y) x ## y
#define raii_with_glue(x, y) raii_with_glueimpl(x, y)


#define T_with(var_decl, init, destr)                              \
  while(1)                                                              \
    if(0)                                                               \
    raii_with_glue(__using_finished, __LINE__):                         \
      break;                                                            \
    else                                                                \
      for(raii_with_lifetime_list_t _tmp = {.elem.resource = init, .elem.destructor = destr, .next = raii_with_lifetime_list};;) \
        for(var_decl = _tmp.elem.resource;;)                                 \
          if (0) {                                                      \
          raii_with_glue(__using_cleanup, __LINE__):                    \
            _tmp.elem.destructor(_tmp.elem.resource);                   \
              goto raii_with_glue(__using_finished, __LINE__);          \
          } else                                                        \
            if (1) {                                                    \
              _tmp.next = raii_with_lifetime_list;                      \
                goto raii_with_glue(__using_setup, __LINE__);           \
            } else                                                      \
            raii_with_glue(__using_setup, __LINE__):                    \
              for(raii_with_lifetime_list_t raii_with_lifetime_list = _tmp;;goto raii_with_glue(__using_body, __LINE__)) \
                while (1)                                               \
                  if (1){                                               \
                    goto raii_with_glue(__using_cleanup, __LINE__);     \
                  } else                                                \
                  raii_with_glue(__using_body, __LINE__):               \


                    int main() {

                      T_with(int *myint, safe_malloc(sizeof(int)), free){
                        *myint = 10;
                        printf("myint is located at %p and contains: %d\n", myint, *myint);
                        printf("%s\n", __func__);
                      }

                      printf("Test\n");
                    }
