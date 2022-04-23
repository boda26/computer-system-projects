#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../add_int.h"
#include "lib/catch.hpp"

TEST_CASE("`add_ints` adds correctly", "[weight=1][part=1]") {
    int r = add_ints(4, 2);
    REQUIRE(r == 6);
}

TEST_CASE("`add_intMixed` adds correctly", "[weight=1][part=1]") {
    int *p1 = malloc(sizeof(int));
    *p1 = 42;
    int r = add_intMixed(p1, 8);
    REQUIRE(r == 50);
    free(p1);
}

TEST_CASE("`add_intPtrs` adds correctly", "[weight=1][part=1]") {
    int *p1 = malloc(sizeof(int));
    int *p2 = malloc(sizeof(int));
    *p1 = -3;
    *p2 = 6;
    int r = add_intPtrs(p1, p2);
    REQUIRE(r == 3);
    free(p1);
    free(p2);
}

TEST_CASE("`add_ints_alloc` adds correctly", "[weight=1][part=1]") {
    int *ptr = add_ints_alloc(3, 5);
    REQUIRE(ptr != NULL);
    REQUIRE(*ptr == 8);
    free(ptr);
}

TEST_CASE("`add_ints_alloc` allocates new memory", "[weight=1][part=1]") {
    int *ptr1 = add_ints_alloc(3, 5);
    int *ptr2 = add_ints_alloc(3, 5);
    REQUIRE(ptr1 != ptr2);
    free(ptr2);
    free(ptr1);
}
