#include <stdio.h>
#include <stdlib.h>

// Add the numbers `a` and `b`, returning the result:
int add_ints(int a, int b) {
  return a + b;
}

// Add the contents of the pointer `a` with int `b`, returning the result:
int add_intMixed(int *a, int b) {
  return *a + b;
}

// Add the contents of the pointers `a` and `b` together, returning the result:
int add_intPtrs(int *a, int *b) {
  return *a + *b;
}

// Add the numbers `a` and `b`, returning the result as a pointer in new memory heap memory that you allocate:
int *add_ints_alloc(int a, int b) {
  int *result = malloc(sizeof(int));
  *result = a + b;
  return result;
}
