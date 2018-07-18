#include <stdio.h>
#include <stdlib.h>

#include "raii/raii.h"

void *safe_malloc(size_t size) {
  void * ptr = malloc(size);
  if(ptr == NULL) {
    perror("Error during malloc allocation");
    exit(EXIT_FAILURE);
  }
  return ptr;
}


int safe_return_example() {
  raii_with(int *someint, malloc(sizeof(int)), free) {
    if (someint == NULL){
      break; // immediately relinquishes this resource and continues after matching `}`.
    }

    *someint = 40;
    raii_with(int *otherint, safe_malloc(sizeof(int)), free) {
      *otherint = *someint + 2;
      printf("someint: %d!\n", *otherint);
      printf("otherint: %d!\n", *someint);

      // Note we don't call `safe_return *bar` directly,
      // since that would be a wild pointer dereference.
      int res = *otherint;
      safe_return res;
    }
  }

  return -1; // default value
}


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
