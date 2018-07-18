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



int foo() {
  raii_with(int *foo, safe_malloc(sizeof(int)), free){
    *foo = 40;
    raii_with(int *bar, safe_malloc(sizeof(int)), free){
      *bar = *foo + 2;
      printf("Bar %d!\n", *bar);
      printf("Foo! %d\n", *foo);
      int res = *bar;
      safe_return res;
    }
  }
}

int main() {

  raii_with(int *myint, safe_malloc(sizeof(int)), free){

    *myint = 10;
    printf("myint is located at %p and contains: %d\n", myint, *myint);
    printf("%s\n", __func__);
  }

  int res = foo();
  printf("res: %d\n", res);

printf("Test\n");
}
