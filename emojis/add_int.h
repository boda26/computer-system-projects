#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int add_ints(int a, int b);
int add_intMixed(int *a, int b);
int add_intPtrs(int *a, int *b);
int *add_ints_alloc(int a, int b);

#ifdef __cplusplus
}
#endif